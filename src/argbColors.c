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


#include "configuration.h"
#include "device_setup.c"
#include "init_terminate.c"
#include "commands.c"
#include "iousb.c"

unsigned int red, green, blue;

const char *argp_program_version = "argbColor 0.1";
const char *argp_program_bug_address = "https://github.com/developersu/argbColors/issues/";
static char doc[] = "-s COMMAND\n-e SOMETHING-SOMETHING";
static char doc1[] = "apply sync or separate command\vFIXME LATER ON PLEASE";

static struct argp_option options[] = {
  {"sync",       's', "COMMAND",   0, "Synchronized command" },
  {"separate",   'e', "COMMAND",   0, "Separate command(s)" },
  {"color",      'c', "RGB_COLOR", 0, "Define color (000000..ffffff)" },
  {"brightness", 'b', "VALUE",     0, "Define brightness (0..5)" },
  {"quiet",      'q', 0,           0, "Don't produce any output" },
  { 0 }
};

struct arguments{
  int quiet;
  char *sync;
  char *separate;
  char *color;
  char *brightness;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state){
    /* Get the input argument from argp_parse, which we
       know is a pointer to our arguments structure. */
    struct arguments *arguments = state->input;

    switch (key){ 
    case 'q':
        arguments->quiet = 1;
        break;
    case 's':
        arguments->sync = arg;
        break;
    case 'e':
        arguments->separate = arg;
        break;
    case 'c':
        arguments->color = arg;
        break;
    case 'b':
        arguments->brightness = arg;
        break;
    default:
        return ARGP_ERR_UNKNOWN;
    }  
    return 0;
}

static struct argp argp = { options, parse_opt, doc, doc1 };

int parse_color(char *color, unsigned int *red, unsigned int *green, unsigned int *blue ){
    printf("%s\n", color);
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
    else if (strcmp(directive, "off") == 0)
        return turnOffBacklightSync();
    else if (strcmp(directive, "off") == 0)
        return staticColorSync(red, green, blue);
    else{
        printf("Command not recognized\n");
        return staticColorSync(red, green, blue);       // TODO: refactor; leave information block and nothing instead
    }
}



int main(int argc, char *argv[]) {

    struct arguments arguments;
    /* Default values. */
    arguments.quiet = 0;
    arguments.sync = "-";
    arguments.separate = "-";
    arguments.color = "ff2fff";
    arguments.brightness = "5";

    /* Parse our arguments; every option seen by parse_opt will
        be reflected in arguments. */
    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    if (arguments.quiet)
        freopen("/dev/null", "a", stdout);

    if (parse_color(arguments.color, &red, &green, &blue) != 0){
        printf("Color parse failure\n");
        return -1;
    }
    
    int brightness = atoi(arguments.brightness);
    if (brightness > 5)
        brightness = 0;

    printf("%s\n", arguments.sync);
    
    // - - -
    int ret = configure_device();
    if (ret != 0){
        printf("%s - %d\n", libusb_error_name(ret), ret);
        return -1;
    }

    if (init_sequence()){
        printf("Initial sequence transfer failure\n");
        libusb_close(dev_handle);
        return -1;
    }
    // - - - - - - - - - - - - - -
    if (strcmp(arguments.sync, "-") != 0){              // Sync flow
        if (sync_flow(arguments.sync)){
            printf("Command transfer failure\n");
            libusb_close(dev_handle);
            return -1;
        }
    }
    else if (strcmp(arguments.separate, "-") != 0){    // Separate flow
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
    
    libusb_close(dev_handle);

    return 0;
}
