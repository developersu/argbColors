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

enum synchronized_commands { SYNC_COLOR, SYNC_WAVE, SYNC_WAVE2, SYNC_OFF, SPEC_IMPULSE } 
synchronized_commands;

struct separate_c{
    char *command1, *command2, *command3, *command4, *command5, *command6; 
    int red1, red2, red3, red4, red5, red6;
    int green1, green2, green3, green4, green5, green6; 
    int blue1, blue2, blue3, blue4, blue5, blue6; 
    int brightness1, brightness2, brightness3, brightness4, brightness5, brightness6;
    int intensity1, intensity2, intensity3, intensity4, intensity5, intensity6; 
} sc;

separate_commands_set s;
enum synchronized_commands sync_cmd;

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

int prepare_sync(char* command){
    if (strcmp(command, "") == 0)
        sync_cmd = SYNC_COLOR;
    else if (strcmp(command, "wave") == 0)
        sync_cmd = SYNC_WAVE;
    else if (strcmp(command, "wave2") == 0)
        sync_cmd = SYNC_WAVE2;
    else if (strcmp(command, "color") == 0)
        sync_cmd = SYNC_COLOR;
    else if (strcmp(command, "off") == 0)
        sync_cmd = SYNC_OFF;
    else if (strcmp(command, "impulse") == 0)
        sync_cmd = SPEC_IMPULSE;
    else{
        printf("Invalid command \"%s\"\n"
            "Allowed: color wave wave2 off\n", command);
        return -1;
    }
    return 0;
}

int make_separate_command(separate_commands_set *s, int dev_number, char *command, 
                          int red, int green, int blue, 
                          int brightness, int intensity){
    if (strcmp(command, "color") == 0){
        if (staticOneColor(dev_number, s, brightness, red, green, blue))
            return -1;
    }
    else if (strcmp(command, "off") == 0){
        if (staticOff(dev_number, s))
            return -1;
    }      
    else if (strcmp(command, "impulse") == 0){
        if (staticImpulse(dev_number, s, intensity, red, green, blue))
            return -1;
    }
    else if (strcmp(command, "flash") == 0){
        if (staticFlash(dev_number, s, brightness, intensity, red, green, blue))
            return -1;
    }
    else if (strcmp(command, "flash2") == 0){
        if (staticFlash2(dev_number, s, brightness, intensity, red, green, blue))
            return -1;
    }
    else if (strcmp(command, "cycle") == 0){
        if (staticCycle(dev_number, s, intensity, brightness))
            return -1;
    }
    else{
        printf("Invalid command --z%d=%s\n"
                "Allowed: color off impulse flash flash2 cycle\n", dev_number, command);
        return -1;
    }

    return 0;
}

int make_separate_commands_set(){
    if (make_separate_command(&s, 1, sc.command1, sc.red1, sc.green1, sc.blue1, sc.brightness1, sc.intensity1))
        return -1;
    if (make_separate_command(&s, 2, sc.command2, sc.red2, sc.green2, sc.blue2, sc.brightness2, sc.intensity2))
        return -1;
    if (make_separate_command(&s, 3, sc.command3, sc.red3, sc.green3, sc.blue3, sc.brightness3, sc.intensity3))
        return -1;
    if (make_separate_command(&s, 4, sc.command4, sc.red4, sc.green4, sc.blue4, sc.brightness4, sc.intensity4))
        return -1;
    if (make_separate_command(&s, 5, sc.command5, sc.red5, sc.green5, sc.blue5, sc.brightness5, sc.intensity5))
        return -1;
    if (make_separate_command(&s, 6, sc.command6, sc.red6, sc.green6, sc.blue6, sc.brightness6, sc.intensity6))
        return -1;

    return 0;
}

int sync_flow(unsigned int red, unsigned int green, unsigned int blue){
    switch (sync_cmd){
        case SYNC_COLOR:
            return staticColorSync(red, green, blue);
        case SYNC_WAVE:
            return wave1();
        case SYNC_WAVE2:
            return wave2();
        case SYNC_OFF:
            return turnOffBacklightSync();
        case SPEC_IMPULSE:
            make_separate_command(&s, 1, "impulse", red, green, blue, 4, 4);
            make_separate_command(&s, 2, "impulse", red, green, blue, 4, 4);
            make_separate_command(&s, 3, "impulse", red, green, blue, 4, 4);
            make_separate_command(&s, 4, "impulse", red, green, blue, 4, 4);
            make_separate_command(&s, 5, "impulse", red, green, blue, 4, 4);
            make_separate_command(&s, 6, "impulse", red, green, blue, 4, 4);
            return runStaticCommand(s);
        default:
            return -1;
    }
}

int main(int argc, char *argv[]) {
    struct arguments arguments;
    /* Defaults. */
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
    arguments.z4 = arguments.z5 = arguments.z6 = "???";

    argp_parse(&argp, argc, argv, 0, 0, &arguments);
    
    if (arguments.quiet)
        freopen("/dev/null", "a", stdout);

    if (arguments.sync && arguments.separate){
        printf("Only one option must be defined: '-s' or '-e'\n");
        return -1;
    }
    
    unsigned int red, green, blue, brightness;
    parse_color(arguments.color, &red, &green, &blue);
    brightness = parse_brightness(arguments.brightness);

    if (arguments.separate == 1){
        sc.command1 = arguments.z1;
        sc.command2 = arguments.z2;
        sc.command3 = arguments.z3;
        sc.command4 = arguments.z4;
        sc.command5 = arguments.z5;
        sc.command6 = arguments.z6;

        parse_color(arguments.c1, &sc.red1, &sc.green1, &sc.blue1);
        parse_color(arguments.c2, &sc.red2, &sc.green2, &sc.blue2);
        parse_color(arguments.c3, &sc.red3, &sc.green3, &sc.blue3);
        parse_color(arguments.c4, &sc.red4, &sc.green4, &sc.blue4);
        parse_color(arguments.c5, &sc.red5, &sc.green5, &sc.blue5);
        parse_color(arguments.c6, &sc.red6, &sc.green6, &sc.blue6);
        
        sc.brightness1 = parse_brightness(arguments.b1);
        sc.brightness2 = parse_brightness(arguments.b2);
        sc.brightness3 = parse_brightness(arguments.b3);
        sc.brightness4 = parse_brightness(arguments.b4);
        sc.brightness5 = parse_brightness(arguments.b5);
        sc.brightness6 = parse_brightness(arguments.b6);
        
        sc.intensity1 = parse_brightness(arguments.i1);
        sc.intensity2 = parse_brightness(arguments.i2);
        sc.intensity3 = parse_brightness(arguments.i3);
        sc.intensity4 = parse_brightness(arguments.i4);
        sc.intensity5 = parse_brightness(arguments.i5);
        sc.intensity6 = parse_brightness(arguments.i6);

        if (make_separate_commands_set(&sc))
            return -1;
    }
    else if (prepare_sync(arguments.args[0]))
        return -1;

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
    if (arguments.separate == 1){                 // Separate flow
        if (runStaticCommand(s)){
            printf("Command transfer failure\n");
            libusb_close(dev_handle);
            return -1;
        }
    }
    else if (sync_flow(red, green, blue)){       // Sync & not-defined
        printf("Command transfer failure\n");
        libusb_close(dev_handle);
        return -1;
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
