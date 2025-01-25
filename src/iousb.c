int writeUsb(unsigned char *message){
#ifndef DEBUG
    printf("\r%02d / %02d (write)", ++counter, limit);
#endif
#ifdef DEBUG
    print_array(message, 64);
#endif
    int ret = libusb_control_transfer(dev_handle, 0x21, 9, 0x03cc, 1, message, 64, TIMEOUT);
    if(ret != 64){
        printf("\nFailure: %d\n", ret); 
        print_array(message, 64);
    }
    return ret;
}

int readUsb(){
#ifndef DEBUG
    printf("\r%02d / %02d (read)", ++counter, limit);
#endif
    
    unsigned char buffer[64] = {};
    int ret = libusb_control_transfer(dev_handle, 0xa1, 1, 0x03cc, 1, buffer, 64, TIMEOUT);
    if(ret != 64){
        printf("\nFailure: %d\n", ret); 
        print_array(buffer, 64);
    }
    
#ifdef DEBUG
    printf("INPUT REQUEST\n");
#endif

    return ret;
}