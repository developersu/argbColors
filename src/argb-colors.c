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

#include "argb-colors.h"
#include "configuration.h"
#include "device_setup.c"
#include "init_terminate.c"
#include "commands.c"
#include "iousb.c"

void parse_color(char *color, unsigned int *red, unsigned int *green, unsigned int *blue ){
    if (strlen(color) != 6){
        printf("Incorrect color: \"%s\". Using default: \"ffffff\"\n", color);
        *red = *green = *blue = 0xff;
        return;
    }

    regex_t reg;
    regcomp(&reg, "^[0-9a-fA-F]{6}$", REG_EXTENDED);
    if (regexec(&reg, color, 0, NULL, 0) != 0){
        printf("Incorrect color: \"%s\". Using default: \"ffffff\"\n", color);
        *red = *green = *blue = 0xff;
        return;
    }
    regfree(&reg);
    
    if (sscanf(color, "%2x%2x%2x", red, green, blue) == 3)
        return;

    printf("Incorrect color: \"%s\". Using default: \"ffffff\"\n", color);
    *red = *green = *blue = 0xff;
}

int parse_brightness(char *brightness){
    int ret = atoi(brightness);
    if (ret > 5)
        return 0;
    return ret;
}

int sync_flow(char* command, unsigned int red, unsigned int green, unsigned int blue){
    if (strcmp(command, "wave") == 0)
        return wave1();
    else if (strcmp(command, "wave2") == 0)
        return wave2();
    else if (strcmp(command, "color") == 0)
        return staticColorSync(red, green, blue);
    else if (strcmp(command, "off") == 0)
        return turnOffBacklightSync();
    else{
        printf("Invalid command \"%s\"\n"
               "Allowed: color wave wave2 off\n", command);

        return staticColorSync(red, green, blue);       // TODO: refactor; leave information block and nothing instead
    }
}

int separate_flow(char *command1, int red1, int green1, int blue1, int brightness1, int intensity1,
                  char *command2, int red2, int green2, int blue2, int brightness2, int intensity2,
                  char *command3, int red3, int green3, int blue3, int brightness3, int intensity3,
                  char *command4, int red4, int green4, int blue4, int brightness4, int intensity4,
                  char *command5, int red5, int green5, int blue5, int brightness5, int intensity5,
                  char *command6, int red6, int green6, int blue6, int brightness6, int intensity6 ){
    
    if (strcmp(command1, "color") == 0)
        return staticColorSeparate(brightness1, red1, green1, blue1);
    else if (strcmp(command1, "off") == 0)
        return turnOffBacklight();
    else if (strcmp(command1, "impulse") == 0)
        return impulse(intensity1, red1, green1, blue1);
    else if (strcmp(command1, "flash") == 0)
        return flash(brightness1, intensity1, red1, green1, blue1);
    else if (strcmp(command1, "flash2") == 0)
        return doubleFlash(brightness1, intensity1, red1, green1, blue1);
    else if (strcmp(command1, "cycle") == 0)
        return cycle(intensity1, brightness1);
    else{
        printf("Invalid command \"%s\"\n"
                "Allowed: color off impulse flash flash2 cycle\n", command1);
        return staticColorSync(red1, green1, blue1);        // TODO: FIX ME
    }



    //FIXME
}

int main(int argc, char *argv[]) {

    struct arguments arguments;
    /* Default values. */
    arguments.quiet = 0;
    arguments.sync = 0;
    arguments.separate = 0;
    arguments.c1 = arguments.c2 = arguments.c3 = arguments.c4 = 
    arguments.c5 = arguments.c6 = arguments.color = "ff2f00";
    arguments.i1 = arguments.i2 = arguments.i3 = arguments.i4 = 
    arguments.i5 = arguments.i6 = "4";
    arguments.b1 = arguments.b2 = arguments.b3 = arguments.b4 = 
    arguments.b5 = arguments.b6 = arguments.brightness = "4";
    arguments.z1 = arguments.z2 = arguments.z3 = 
    arguments.z4 = arguments.z5 = arguments.z6 = "-";

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
    unsigned int red, green, blue,
                 red1, green1, blue1,
                 red2, green2, blue2,
                 red3, green3, blue3,
                 red4, green4, blue4,
                 red5, green5, blue5,
                 red6, green6, blue6,
                 brightness, 
                 brightness1, brightness2, brightness3,
                 brightness4, brightness5, brightness6,
                 intensity1, intensity2, intensity3,
                 intensity4, intensity5, intensity6;

    parse_color(arguments.color, &red, &green, &blue);

    brightness = parse_brightness(arguments.brightness);

    if (arguments.separate == 1){
        parse_color(arguments.c1, &red1, &green1, &blue1);
        parse_color(arguments.c2, &red2, &green2, &blue2);
        parse_color(arguments.c3, &red3, &green3, &blue3);
        parse_color(arguments.c4, &red4, &green4, &blue4);
        parse_color(arguments.c5, &red5, &green5, &blue5);
        parse_color(arguments.c6, &red6, &green6, &blue6);
        
        brightness1 = parse_brightness(arguments.b1);
        brightness2 = parse_brightness(arguments.b2);
        brightness3 = parse_brightness(arguments.b3);
        brightness4 = parse_brightness(arguments.b4);
        brightness5 = parse_brightness(arguments.b5);
        brightness6 = parse_brightness(arguments.b6);
        
        intensity1 = parse_brightness(arguments.i1);
        intensity2 = parse_brightness(arguments.i2);
        intensity3 = parse_brightness(arguments.i3);
        intensity4 = parse_brightness(arguments.i4);
        intensity5 = parse_brightness(arguments.i5);
        intensity6 = parse_brightness(arguments.i6);
    }

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
        if (sync_flow(arguments.args[0], red, green, blue)){
            printf("Command transfer failure\n");
            libusb_close(dev_handle);
            return -1;
        }
    }
    else if (arguments.separate == 1){                 // Separate flow
        if (separate_flow(arguments.z1, red1, green1, blue1, brightness1, intensity1,
                        arguments.z2, red2, green2, blue2, brightness2, intensity2,
                        arguments.z3, red3, green3, blue3, brightness3, intensity3,
                        arguments.z4, red4, green4, blue4, brightness4, intensity4,
                        arguments.z5, red5, green5, blue5, brightness5, intensity5,
                        arguments.z6, red6, green6, blue6, brightness6, intensity6)){
            printf("Command transfer failure\n");
            libusb_close(dev_handle);
            return -1;
        }
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
