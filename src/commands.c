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

int staticColorSeparate(int brightness, unsigned char red, unsigned char green, unsigned char blue){
    
    unsigned char brgt;

    switch (brightness){
        case 0:
            brgt = 0x1a;
            break;
        case 1:
            brgt = 0x4d;
            break;
        case 2:
            brgt = 0x80;
            break;
        case 3:
            brgt = 0xb3;
            break;
        case 4:
            brgt = 0xff;
            break;
        default:
            printf("Brightness must be defined in range of 0..4\n");
            return 1;
    }

    if (prefix())
        return 1;

    counter = 0;
    limit = 7;
    //cc 22 04 00 00 00 00 00 00 00 00 01 __ 00 __ __ __ 
    unsigned char dir1[64] = { 0xcc, 0x22, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x01, brgt, 0x00, blue, green, red, };
    //cc 24 10 00 00 00 00 00 00 00 00 01 __ 00 __ __ __ 
    unsigned char dir2[64] = { 0xcc, 0x24, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x01, brgt, 0x00, blue, green, red, };
    //cc 25 20 00 00 00 00 00 00 00 00 01 __ 00 __ __ __ 
    unsigned char dir3[64] = { 0xcc, 0x25, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x01, brgt, 0x00, blue, green, red, };
    //cc 26 40 00 00 00 00 00 00 00 00 01 __ 00 __ __ __ 
    unsigned char dir4[64] = { 0xcc, 0x26, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x01, brgt, 0x00, blue, green, red, };
    //cc 27 80 00 00 00 00 00 00 00 00 01 __ 00 __ __ __ 
    unsigned char dir5[64] = { 0xcc, 0x27, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x01, brgt, 0x00, blue, green, red, };
    //cc 91 00 02 00 00 00 00 00 00 00 01 __ 00 __ __ __ 
    unsigned char dir6[64] = { 0xcc, 0x91, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x01, brgt, 0x00, blue, green, red, };
    
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
    
    printf("One color (Separate) applied\n");
    
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

    printf("Turn off backlight (Separate) applied\n");
    
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

    printf("Turn off backlight (Sync) applied\n");
    
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

    printf("Wave (Sync) applied\n");
    
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

    printf("Wave 2 (Sync) applied\n");
    
    return 0;
}

int staticColorSync(unsigned char red, unsigned char green, unsigned char blue){
    if (prefix())
        return 1;

    counter = 0;
    limit = 2;
    
    //cc 20 ff 07 00 00 00 00 00 00 00 01 ff 00 __ __ __
    unsigned char dir1[64] = { 0xcc, 0x20, 0xff, 0x07, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x01, 0xff, 0x00, blue, green, red, };

    if(64 != writeUsb(dir1))
        return 1;

    if(64 != writeUsb(end_transaction))
        return 1;
    
    printf("One color (Sync) applied\n");
    
    return 0;
}

int impulse(int intensity, unsigned char red, unsigned char green, unsigned char blue){

    unsigned char *I;

    switch (intensity){
        case 0:
            I = (unsigned char [6]) { 0x40, 0x06, 0x40, 0x06, 0x20, 0x03 }; // 400640062003
            break;
        case 1:
            I = (unsigned char [6]) { 0xb0, 0x04, 0xb0, 0x04, 0xf4, 0x01 }; // b004b004f401
            break;
        case 2:
            I = (unsigned char [6]) { 0x84, 0x03, 0x84, 0x03, 0xc2, 0x01 }; // 84038403c201
            break;
        case 3:
            I = (unsigned char [6]) { 0xbc, 0x02, 0xbc, 0x02, 0x5e, 0x01 }; // bc02bc025e01
            break;
        case 4:
            I = (unsigned char [6]) { 0xf4, 0x01, 0xf4, 0x01, 0xfa, 0x00 }; // f401f401fa00
            break;
        default:
            printf("Intensity must be defined in range of 0..4\n");
            return 1;
    }

    if (prefix())
        return 1;

    counter = 0;
    limit = 7;

    //cc22040000000000000000026400______0000000000____________00000001
    unsigned char dir1[64] = {  0xcc, 0x22, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x02, 0x64, 0x00, blue, green, 
                                 red, 0x00, 0x00, 0x00, 0x00, 0x00, I[0], I[1],   I[2], I[3], I[4], I[5], 0x00, 0x00, 0x00, 0x01, };
    //cc24100000000000000000026400______0000000000____________00000001
    unsigned char dir2[64] = {  0xcc, 0x24, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x02, 0x64, 0x00, blue, green, 
                                 red, 0x00, 0x00, 0x00, 0x00, 0x00, I[0], I[1],   I[2], I[3], I[4], I[5], 0x00, 0x00, 0x00, 0x01, };
    //cc25200000000000000000026400______0000000000____________00000001
    unsigned char dir3[64] = {  0xcc, 0x25, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x02, 0x64, 0x00, blue, green, 
                                 red, 0x00, 0x00, 0x00, 0x00, 0x00, I[0], I[1],   I[2], I[3], I[4], I[5], 0x00, 0x00, 0x00, 0x01, };
    //cc26400000000000000000026400______0000000000____________00000001  
    unsigned char dir4[64] = {  0xcc, 0x26, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x02, 0x64, 0x00, blue, green, 
                                 red, 0x00, 0x00, 0x00, 0x00, 0x00, I[0], I[1],   I[2], I[3], I[4], I[5], 0x00, 0x00, 0x00, 0x01, };
    //cc27800000000000000000026400______0000000000____________00000001
    unsigned char dir5[64] = {  0xcc, 0x27, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x02, 0x64, 0x00, blue, green, 
                                 red, 0x00, 0x00, 0x00, 0x00, 0x00, I[0], I[1],   I[2], I[3], I[4], I[5], 0x00, 0x00, 0x00, 0x01, };
    //cc91000200000000000000026400______0000000000____________00000001
    unsigned char dir6[64] = {  0xcc, 0x91, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x02, 0x64, 0x00, blue, green, 
                                 red, 0x00, 0x00, 0x00, 0x00, 0x00, I[0], I[1],   I[2], I[3], I[4], I[5], 0x00, 0x00, 0x00, 0x01, };

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

    printf("Impulse (Separate) applied\n");
    
    return 0;
}
// flash - 1 = brighness; 2 - frequency

int flash(int brightness, int frequency, unsigned char red, unsigned char green, unsigned char blue){

    unsigned char brgt;

    switch (brightness){
        case 0:
            brgt = 0x1a;
            break;
        case 1:
            brgt = 0x4d;
            break;
        case 2:
            brgt = 0x80;
            break;
        case 3:
            brgt = 0xb3;
            break;
        case 4:
            brgt = 0xff;
            break;
        default:
            printf("Brightness must be defined in range of 0..4\n");
            return 1;
    }

    unsigned char *F;

    switch (frequency){
        case 0:
            F = (unsigned char [2]) { 0x60, 0x09 }; // 6009
            break;
        case 1:
            F = (unsigned char [2]) { 0xd0, 0x07 }; // d007
            break;
        case 2:
            F = (unsigned char [2]) { 0x40, 0x06 }; // 4006
            break;
        case 3:
            F = (unsigned char [2]) { 0xb0, 0x04 }; // b004
            break;
        case 4:
            F = (unsigned char [2]) { 0x20, 0x03 }; // 2003
            break;
        default:
            printf("Frequency must be defined in range of 0..4\n");
            return 1;
    }

    // cc2204000000000000000003__00______000000000064006400____0000000101
    unsigned char dir1[64] = { 0xcc, 0x22, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x03, brgt, 0x00, blue, green, 
                               red,  0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x00,   0x64, 0x00, F[0], F[1], 0x00, 0x00, 0x00, 0x01, 
    			               0x01, }; 
    // cc2410000000000000000003__00______000000000064006400____0000000101
    unsigned char dir2[64] = { 0xcc, 0x24, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x03, brgt, 0x00, blue, green, 
                               red,  0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x00,   0x64, 0x00, F[0], F[1], 0x00, 0x00, 0x00, 0x01, 
    			               0x01, }; 
    // cc2520000000000000000003__00______000000000064006400____0000000101
    unsigned char dir3[64] = { 0xcc, 0x25, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x03, brgt, 0x00, blue, green, 
                               red,  0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x00,   0x64, 0x00, F[0], F[1], 0x00, 0x00, 0x00, 0x01, 
    			               0x01, }; 
    // cc2640000000000000000003__00______000000000064006400____0000000101
    unsigned char dir4[64] = { 0xcc, 0x26, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x03, brgt, 0x00, blue, green, 
                               red,  0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x00,   0x64, 0x00, F[0], F[1], 0x00, 0x00, 0x00, 0x01, 
    			               0x01, }; 
    // cc2780000000000000000003__00______000000000064006400____0000000101
    unsigned char dir5[64] = { 0xcc, 0x27, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x03, brgt, 0x00, blue, green, 
                               red,  0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x00,   0x64, 0x00, F[0], F[1], 0x00, 0x00, 0x00, 0x01, 
    			               0x01, }; 
    // cc9100020000000000000003__00______000000000064006400____0000000101
    unsigned char dir6[64] = { 0xcc, 0x91, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x03, brgt, 0x00, blue, green, 
                               red,  0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x00,   0x64, 0x00, F[0], F[1], 0x00, 0x00, 0x00, 0x01, 
    			               0x01, }; 

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

    printf("Flash (Separate) applied\n");
    
    return 0;
}

int doubleFlash(int brightness, int frequency, unsigned char red, unsigned char green, unsigned char blue){

    unsigned char brgt;

    switch (brightness){
        case 0:
            brgt = 0x1a;
            break;
        case 1:
            brgt = 0x4d;
            break;
        case 2:
            brgt = 0x80;
            break;
        case 3:
            brgt = 0xb3;
            break;
        case 4:
            brgt = 0xff;
            break;
        default:
            printf("Brightness must be defined in range of 0..4\n");
            return 1;
    }
    
    unsigned char *F;

    switch (frequency){
        case 0:
            F = (unsigned char [2]) { 0x28, 0x0a }; // 280a
            break;
        case 1:
            F = (unsigned char [2]) { 0x98, 0x08 }; // 9808
            break;
        case 2:
            F = (unsigned char [2]) { 0x08, 0x07 }; // 0807
            break;
        case 3:
            F = (unsigned char [2]) { 0x78, 0x05 }; // 7805
            break;
        case 4:
            F = (unsigned char [2]) { 0xe8, 0x03 }; // e803
            break;
        default:
            printf("Frequency must be defined in range of 0..4\n");
            return 1;
    }

    //cc2204000000000000000003__00______000000000064006400____0000000102
    unsigned char dir1[64] = { 0xcc, 0x22, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x03, brgt, 0x00, blue, green, 
                               red, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x00,    0x64, 0x00, F[0], F[1], 0x00, 0x00, 0x00, 0x01, 
                               0x02, }; 
    //cc2410000000000000000003__00______000000000064006400____0000000102
    unsigned char dir2[64] = { 0xcc, 0x24, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x03, brgt, 0x00, blue, green, 
                               red, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x00,    0x64, 0x00, F[0], F[1], 0x00, 0x00, 0x00, 0x01, 
                               0x02, }; 
    //cc2520000000000000000003__00______000000000064006400____0000000102
    unsigned char dir3[64] = { 0xcc, 0x25, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x03, brgt, 0x00, blue, green, 
                               red, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x00,    0x64, 0x00, F[0], F[1], 0x00, 0x00, 0x00, 0x01, 
                               0x02, }; 
    //cc2640000000000000000003__00______000000000064006400____0000000102
    unsigned char dir4[64] = { 0xcc, 0x26, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x03, brgt, 0x00, blue, green, 
                               red, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x00,    0x64, 0x00, F[0], F[1], 0x00, 0x00, 0x00, 0x01, 
                               0x02, }; 
    //cc2780000000000000000003__00______000000000064006400____0000000102
    unsigned char dir5[64] = { 0xcc, 0x27, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x03, brgt, 0x00, blue, green, 
                               red, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x00,    0x64, 0x00, F[0], F[1], 0x00, 0x00, 0x00, 0x01, 
                               0x02, }; 
    //cc9100020000000000000003__00______000000000064006400____0000000102
    unsigned char dir6[64] = { 0xcc, 0x91, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x03, brgt, 0x00, blue, green, 
                               red, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x00,    0x64, 0x00, F[0], F[1], 0x00, 0x00, 0x00, 0x01, 
                               0x02, }; 

    counter = 0;
    limit = 7;

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

    printf("Double flash (Separate) applied\n");
    
    return 0;
}

int cycle(int intensity, int brightness){

    unsigned char brgt;

    switch (brightness){
        case 0:
            brgt = 0x1a;
            break;
        case 1:
            brgt = 0x4d;
            break;
        case 2:
            brgt = 0x80;
            break;
        case 3:
            brgt = 0xb3;
            break;
        case 4:
            brgt = 0xff;
            break;
        default:
            printf("Brightness must be defined in range of 0..4\n");
            return 1;
    }

    unsigned char *I;

    switch (intensity){
        case 0:
            I = (unsigned char [4]) { 0x78, 0x05, 0xb0, 0x04 }; // 7805b004
            break;
        case 1:
            I = (unsigned char [4]) { 0x52, 0x03, 0xee, 0x02 }; // 5203ee02
            break;
        case 2:
            I = (unsigned char [4]) { 0x26, 0x02, 0xc2, 0x01 }; // 2602c201
            break;
        case 3:
            I = (unsigned char [4]) { 0x58, 0x02, 0x90, 0x01 }; // 58029001
            break;
        case 4:
            I = (unsigned char [4]) { 0x90, 0x01, 0xc8, 0x00 }; // 9001c800
            break;
        default:
            printf("Intensity must be defined in range of 0..4\n");
            return 1;
    }
    
    counter = 0;
    limit = 7;

    //cc2204000000000000000004__00fd00fe0000000000________0000000007
    unsigned char dir1[64] = { 0xcc, 0x22, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x04, brgt, 0x00, 0xfd, 0x00, 
                               0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, I[0], I[1],   I[2], I[3], 0x00, 0x00, 0x00, 0x00, 0x07, };
    //cc2410000000000000000004__00fd00fe0000000000________0000000007
    unsigned char dir2[64] = { 0xcc, 0x24, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x04, brgt, 0x00, 0xfd, 0x00, 
                               0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, I[0], I[1],   I[2], I[3], 0x00, 0x00, 0x00, 0x00, 0x07, };
    //cc2520000000000000000004__00fd00fe0000000000________0000000007                               
    unsigned char dir3[64] = { 0xcc, 0x25, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x04, brgt, 0x00, 0xfd, 0x00, 
                               0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, I[0], I[1],   I[2], I[3], 0x00, 0x00, 0x00, 0x00, 0x07, };
    //cc2640000000000000000004__00fd00fe0000000000________0000000007
    unsigned char dir4[64] = { 0xcc, 0x26, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x04, brgt, 0x00, 0xfd, 0x00, 
                               0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, I[0], I[1],   I[2], I[3], 0x00, 0x00, 0x00, 0x00, 0x07, };
    //cc2780000000000000000004__00fd00fe0000000000________0000000007 
    unsigned char dir5[64] = { 0xcc, 0x27, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x04, brgt, 0x00, 0xfd, 0x00, 
                               0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, I[0], I[1],   I[2], I[3], 0x00, 0x00, 0x00, 0x00, 0x07, };
    //cc9100020000000000000004__00fd00fe0000000000________0000000007
    unsigned char dir6[64] = { 0xcc, 0x91, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x04, brgt, 0x00, 0xfd, 0x00, 
                               0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, I[0], I[1],   I[2], I[3], 0x00, 0x00, 0x00, 0x00, 0x07, };

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

    printf("Impulse (Separate) applied\n");
    
    return 0;
}