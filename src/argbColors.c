 /**********************
 Dmitry Isaenko
 License: GNU GPL v.3
 redrise.ru, github.com/developersu
 2025, Russia
 ***********************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <argp.h>
#include <regex.h>

#include "libusb-1.0/libusb.h"

#include "argbColors.h"
#include "configuration.h"
#include "device_setup.c"
#include "init_terminate.c"
#include "commands.c"
#include "iousb.c"

unsigned int red, green, blue;

int parse_color(char *color, unsigned int *red, unsigned int *green, unsigned int *blue ){
    if (strlen(color) != 6)
        return -1;

    regex_t reg;
    regcomp(&reg, "^[0-9a-fA-F]{6}$", REG_EXTENDED);
    if (regexec(&reg, color, 0, NULL, 0) != 0)
        return -1;
    regfree(&reg);
    
    if (sscanf(color, "%2x%2x%2x", red, green, blue) == 3)
        return 0;
        
    return -1;
}

int sync_flow(char* directive){
    if (strcmp(directive, "wave") == 0)
        return wave1();
    else if (strcmp(directive, "wave2") == 0)
        return wave2();
    else if (strcmp(directive, "color") == 0)
        return staticColorSync(red, green, blue);
    else if (strcmp(directive, "off") == 0)
        return turnOffBacklightSync();
    else{
        printf("Command not recognized\n"
               "Possible values are: color wave wave2 off\n");

        return staticColorSync(red, green, blue);       // TODO: refactor; leave information block and nothing instead
    }
}

int separate_flow(){

}


int main(int argc, char *argv[]) {

    struct arguments arguments;
    /* Default values. */
    arguments.quiet = 0;
    arguments.sync = 0;
    arguments.separate = 0;
    arguments.color = "ff2fff";
    arguments.brightness = "5";

    /* Parse our arguments; every option seen by parse_opt will
        be reflected in arguments. */
    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    if (arguments.quiet)
        freopen("/dev/null", "a", stdout);

    if (arguments.sync && arguments.separate){
        printf("Only one option must be defined: '-s' or '-e'\n");        
        return -1;
    }
    /*
    if (arguments.separate && ){
        printf("In separate flow each zone has to be defined: '-z1=color -z2=impulse ... -z6=color'\n");        
        return -1;
    }
    */
    if (parse_color(arguments.color, &red, &green, &blue) != 0){
        printf("Color parse failure\n");
        return -1;
    }
            
            int brightness = atoi(arguments.brightness);
            if (brightness > 5)
                brightness = 0;

    // - - -
    int ret = configure_device();
    if (ret != 0){
        printf("%s - %d\n", libusb_error_name(ret), ret);
        return -1;
    }
    if (verbose_output)
        printf("Device configuration complete\n");

    if (init_sequence()){
        printf("Initial sequence transfer failure\n");
        libusb_close(dev_handle);
        return -1;
    }
    if (verbose_output)
        printf("Initialization sequence sent\n");

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    if (arguments.sync == 1){                          // Sync flow
        if (sync_flow(arguments.args[0])){
            printf("Command transfer failure\n");
            libusb_close(dev_handle);
            return -1;
        }
    }
    else if (arguments.separate == 0){                 // Separate flow
        if(staticColorSync(red, green, blue)){         // TODO: FIX!
            printf("Command transfer failure\n");
            libusb_close(dev_handle);
            return -1;
        }

        /*
            if (cycle(0, 4)){
                printf("Command transfer failure\n");
                libusb_close(dev_handle);
                return -1;
            }

            if (impulse(0, 0xff, 0x2f, 0xff)){
                printf("Command transfer failure\n");
                libusb_close(dev_handle);
                return -1;
            }
        //*/
    }
    else{
        if(staticColorSync(red, green, blue)){          // Executed neither for sync, nor for separate => set single color
            printf("Command transfer failure\n");
            libusb_close(dev_handle);
            return -1;
        }
    }
    
    if (terminate_sequence()){
        printf("Termination sequence transfer failure\n");
        libusb_close(dev_handle);
        return -1;
    }
    if (verbose_output)
        printf("Termination sequence sent\n");

    libusb_close(dev_handle);

    return 0;
}
