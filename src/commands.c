unsigned char end_transaction[64] = { 0xcc, 0x28, 0xff, 0x07, }; //cc28ff07
unsigned char end_alt_transaction[64] = { 0xcc, 0x28, 0x04, };   //cc2804

int prefix(){
    counter = 0;
    limit = 12; // flow contains 12 I/O events
    
    unsigned char message[64] = {0xcc, 0x20, };
    if(64 != writeUsb(message))
        return 1;

    message[1] = 0x21;
    if(64 != writeUsb(message))
        return 1;
    
    message[1] = 0x22;
    if(64 != writeUsb(message))
        return 1;
        
    message[1] = 0x23;
    if(64 != writeUsb(message))
        return 1;
        
    message[1] = 0x24;
    if(64 != writeUsb(message))
        return 1;
    
    message[1] = 0x25;
    if(64 != writeUsb(message))
        return 1;
        
    message[1] = 0x26;
    if(64 != writeUsb(message))
        return 1;
    
    message[1] = 0x27;
    if(64 != writeUsb(message))
        return 1;
    
    message[1] = 0x90;
    if(64 != writeUsb(message))
        return 1;
        
    message[1] = 0x91;
    if(64 != writeUsb(message))
        return 1;
                
    message[1] = 0x92;
    if(64 != writeUsb(message))
        return 1;

    if(64 != writeUsb(end_transaction))
        return 1;
    
    return 0;
}

int staticColorSeparate(unsigned char brightness, unsigned char red, unsigned char green, unsigned char blue){
    if (prefix())
        return 1;

    counter = 0;
    limit = 7;
    //cc 22 04 00 00 00 00 00 00 00 00 01__00______ 
    unsigned char dir1[64] = { 0xcc, 0x22, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x01, brightness, 0x00, blue, green, red, };
    //cc 24 10 00 00 00 00 00 00 00 00 01__00______
    unsigned char dir2[64] = { 0xcc, 0x24, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x01, brightness, 0x00, blue, green, red, };
    //cc 25 20 00 00 00 00 00 00 00 00 01__00______
    unsigned char dir3[64] = { 0xcc, 0x25, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x01, brightness, 0x00, blue, green, red, };
    //cc 26 40 00 00 00 00 00 00 00 00 01__00______
    unsigned char dir4[64] = { 0xcc, 0x26, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x01, brightness, 0x00, blue, green, red, };
    //cc 27 80 00 00 00 00 00 00 00 00 01__00______
    unsigned char dir5[64] = { 0xcc, 0x27, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x01, brightness, 0x00, blue, green, red, };
    //cc 91 00 02 00 00 00 00 00 00 00 01__00______
    unsigned char dir6[64] = { 0xcc, 0x91, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x01, brightness, 0x00, blue, green, red, };
    
    if(64 != writeUsb(dir1))
        return 1;

    if(64 != writeUsb(dir2))
        return 1;
    
    if(64 != writeUsb(dir3))
        return 1;

    if(64 != writeUsb(dir4))
        return 1;

    if(64 != writeUsb(dir5))
        return 1;

    if(64 != writeUsb(dir6))
        return 1;

    if(64 != writeUsb(end_transaction))
        return 1;
    
    printf("\rStatic Color (Separate) sequence sent\n");
    
    return 0;
}

int turnOffBacklight(){
    if (prefix())
        return 0;
    
    counter = 0;
    limit = 7;
    
    //cc 22 04 00 00 00 00 00 00 00 00 01 ff
    unsigned char dir1[64] = { 0xcc, 0x22, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x01, 0xff, };
    //cc 24 10 00 00 00 00 00 00 00 00 01 ff
    unsigned char dir2[64] = { 0xcc, 0x24, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x01, 0xff, };
    //cc 25 20 00 00 00 00 00 00 00 00 01 ff
    unsigned char dir3[64] = { 0xcc, 0x25, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x01, 0xff, };
    //cc 26 40 00 00 00 00 00 00 00 00 01 ff
    unsigned char dir4[64] = { 0xcc, 0x26, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x01, 0xff, };
    //cc 27 80 00 00 00 00 00 00 00 00 01 ff
    unsigned char dir5[64] = { 0xcc, 0x27, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x01, 0xff, };
    //cc 91 00 02 00 00 00 00 00 00 00 01 ff
    unsigned char dir6[64] = { 0xcc, 0x91, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x01, 0xff, };

    if(64 != writeUsb(dir1))
        return 1;

    if(64 != writeUsb(dir2))
        return 1;
    
    if(64 != writeUsb(dir3))
        return 1;

    if(64 != writeUsb(dir4))
        return 1;

    if(64 != writeUsb(dir5))
        return 1;

    if(64 != writeUsb(dir6))
        return 1;

    if(64 != writeUsb(end_transaction))
        return 1;

    printf("\rTurn off backlight (Separate) sequence sent\n");
    
    return 0;
}

int turnOffBacklightSync(){
    if (prefix())
        return 0;
    
    counter = 0;
    limit = 2;
    
    //cc 20 ff 07 00 00 00 00 00 00 00 01 ff
    unsigned char dir1[64] = { 0xcc, 0x20, 0xff, 0x07, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x01, 0xff, };

    if(64 != writeUsb(dir1))
        return 1;

    if(64 != writeUsb(end_transaction))
        return 1;

    printf("\rTurn off backlight (Sync) sequence sent\n");
    
    return 0;
}

int wave1(){
    //cc 20 ff 07 00 00 00 00 00 00 00 01 ff ← turns off backlight
    //cc28ff07
    turnOffBacklightSync();
    
    counter = 0;
    limit = 2;

    //cc 22 00 00 00 00 00 00 00 00 00 09 64 00 00 00 00 00 00 00 00 00 26 02 00 00 00 00 00 00 07
    unsigned char dir1[64] = { 0xcc, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x09, 0x64, 0x00, 0x00, 0x00,
                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x26, 0x02,   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, };

    if(64 != writeUsb(dir1))
        return 1;

    //cc2804
    if(64 != writeUsb(end_alt_transaction))
        return 1;

    unsigned char dir1[64] = { 0xcc, };

    printf("\rWave 1 (Sync) sequence sent\n");
    
    return 0;
}

int wave2(){
    //cc 20 ff 07 00 00 00 00 00 00 00 01 ff ← turns off backlight
    //cc28ff07
    turnOffBacklightSync();
    
    counter = 0;
    limit = 2;
    
    //cc 22 00 00 00 00 00 00 00 00 00 0a 64 00 00 00 00 00 00 00 00 00 7d 00 00 00 00 00 00 00 20
    unsigned char dir1[64] = { 0xcc, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x0a, 0x64, 0x00, 0x00, 0x00,
                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7d, 0x00,   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, };

    if(64 != writeUsb(dir1))
        return 1;

    //cc2804
    if(64 != writeUsb(end_alt_transaction))
        return 1;

    printf("\rWave 2 (Sync) sequence sent\n");
    
    return 0;
}