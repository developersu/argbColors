 /**********************
 Dmitry Isaenko
 License: GNU GPL v.3
 redrise.ru, github.com/developersu
 2025, Russia
 ***********************/

#include <stdio.h>
#include <stdlib.h>
#include "libusb-1.0/libusb.h"

#include "device_configuration.h"
#include "init_terminate.c"
#include "commands.c"
#include "iousb.c"
#include "helpers.c"

int findDevice(libusb_device *dev){

    struct libusb_device_descriptor desc;

    int ret = libusb_get_device_descriptor(dev, &desc);
    if (ret < 0) {
        fprintf(stderr, "failed to get device descriptor");
        return 0;
    }

    if (desc.idVendor == VID && desc.idProduct == PID){
        printf("Device found: %04x:%04x\n", desc.idVendor, desc.idProduct);
        return 1;
    }

    return 0;
}

int main(int argc, char *argv[]) {

    libusb_context *context;
    int ret = libusb_init_context(&context, /*options=*/NULL, /*num_options=*/0);
    if (ret < 0){
        printf("libusb_init_context failure\n");
        return ret;
    }

    libusb_device **devs;
    ssize_t count = libusb_get_device_list(context, &devs);
    if (count < 0) {
        libusb_exit(NULL);
        printf("libusb_get_device_list failed\n");
        return 1;
    }
    
    // Get device list
    libusb_device* device = NULL;
    for (int i = 0; devs[i]; i++){
        if (findDevice(devs[i])){
            device = devs[i];
            break;
        }
    }

    if (device == NULL){
        libusb_exit(NULL);
        printf("Device not found\n");
        return 1;
    }

    // Free device list
    libusb_free_device_list(devs, 1);

    ret = libusb_open(device, &dev_handle);
    if (ret < 0) {
        libusb_exit(NULL);
        printf("libusb_open failed\n");
        return ret;
    }

    // We will rule, not kernel modules :D
    ret = libusb_set_auto_detach_kernel_driver(dev_handle, 1);
#ifdef DEBUG
    printf("libusb_set_auto_detach_kernel_driver - %d\n", ret);

    // Let's find out current CONFIGURATION
    int bConfigurationValue;
    ret = libusb_get_configuration(dev_handle, &bConfigurationValue);
    if (ret < 0) {
        libusb_exit(NULL);
        printf("libusb_get_configuration failed: %d\n", ret);
        return ret;
    }

    printf("Current bConfigurationValue: %d\n", bConfigurationValue);
#endif
    // Check if device used by kernel drivers already
    ret = libusb_kernel_driver_active(dev_handle, 1);
    printf("Kernel driver is%s", ret == 0?" not active\n":"active and ");
    
    // Active? Let's try to get control
    if(ret != LIBUSB_ERROR_NOT_SUPPORTED && ret < 0){
        ret = libusb_detach_kernel_driver(dev_handle, 1);
        if (ret != LIBUSB_ERROR_NOT_SUPPORTED && ret < 0) {
            libusb_exit(NULL);
            printf("detach kernel driver attempt failed: %d\n", ret);
            return ret;
        }
    }

    /*
    // Now let's reset device
    // Or just comment it out, since it's always this kinda shit appears: [25465.608100] usb 3-13: usbfs: interface 0 claimed by usbhid while 'argbColors' sets config #1
    // Either way it works
    ret = libusb_set_configuration(dev_handle, 1);
    if (ret < 0) {
        libusb_exit(NULL);
        printf("libusb_set_configuration failed: %d\n", ret);
        return ret;
    }
    */

    // Claim interface
    ret = libusb_claim_interface(dev_handle, 1);
    if (ret < 0) {
        libusb_exit(NULL);
        printf("libusb_claim_interface failed: %d\n", ret);
        return ret;
    }


    if (init_sequence()){
        printf("Initial sequence transfer failure\n");
        libusb_close(dev_handle);
        return -1;
    }

    if (impulse(0, 0xff, 0x2f, 0xff)){
        printf("Command transfer failure\n");
        libusb_close(dev_handle);
        return -1;
    }
/*
    // 0xff, 0x2f, 0xff <3
    if(staticColorSeparate(0xff, 0xff, 0x2b, 0x00)){
        printf("Command transfer failure\n");
        libusb_close(dev_handle);
        return -1;
    }
//*/
    if (terminate_sequence()){
        printf("Termination sequence transfer failure\n");
        libusb_close(dev_handle);
        return -1;
    }
    
    libusb_close(dev_handle);

    return 0;
}
