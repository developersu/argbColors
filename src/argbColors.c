 /**********************
 Dmitry Isaenko
 License: GNU GPL v.3
 redrise.ru, github.com/developersu
 2025, Russia
 ***********************/

#include <stdio.h>
#include <stdlib.h>
#include "libusb-1.0/libusb.h"

#include "configuration.h"
#include "device_setup.c"
#include "init_terminate.c"
#include "commands.c"

int main(int argc, char *argv[]) {
    
//    if (argc < 2){
//        printf("Missing arguments ('argbColors --help' for help)\n");
//        return -1;
//    }
    
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
    // 0xff, 0x2f, 0xff <3
    if(staticColorSeparate(0xff, 0xff, 0x2b, 0x00)){
        printf("Command transfer failure\n");
        libusb_close(dev_handle);
        return -1;
    }

    if (terminate_sequence()){
        printf("Termination sequence transfer failure\n");
        libusb_close(dev_handle);
        return -1;
    }
    
    libusb_close(dev_handle);

    return 0;
}
