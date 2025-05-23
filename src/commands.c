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
    
    printf("One color %02x%02x%02x (Sync) applied\n", red, green, blue);
    
    return 0;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
typedef enum{
    EFFECT_COLOR,
    EFFECT_OFF,
    EFFECT_IMPULSE,
    EFFECT_FLASH,
    EFFECT_DOUBLE_FLASH,
    EFFECT_CYCLE
} STATIC_EFFECT;

typedef struct {
    STATIC_EFFECT name1;
    STATIC_EFFECT name2;
    STATIC_EFFECT name3;
    STATIC_EFFECT name4;
    STATIC_EFFECT name5;
    STATIC_EFFECT name6;
    unsigned char dir1[64];
    unsigned char dir2[64];
    unsigned char dir3[64];
    unsigned char dir4[64];
    unsigned char dir5[64];
    unsigned char dir6[64];
} separate_commands_set;

void print_ext_report(STATIC_EFFECT effect){
    switch (effect)
    {
    case EFFECT_COLOR:
        printf("* One color\n");
        break;
    case EFFECT_OFF:
        printf("* Off\n");
        break;
    case EFFECT_IMPULSE:
        printf("* Impulse\n");
        break;
    case EFFECT_FLASH:
        printf("* Flash\n");
        break;
    case EFFECT_DOUBLE_FLASH:
        printf("* Double flash\n");
        break;
    case EFFECT_CYCLE:
        printf("* Cycle\n");
        break;
    }
}

int runStaticCommand(separate_commands_set c_set){
    if (prefix())
        return 1;

    counter = 0;
    limit = 7;
    
    if(64 != writeUsb(c_set.dir1))
        return 1;

    if(64 != writeUsb(c_set.dir2))
        return 1;
    
    if(64 != writeUsb(c_set.dir3))
        return 1;

    if(64 != writeUsb(c_set.dir4))
        return 1;

    if(64 != writeUsb(c_set.dir5))
        return 1;

    if(64 != writeUsb(c_set.dir6))
        return 1;

    if(64 != writeUsb(end_transaction))
        return 1;

    printf("Commands sent:\n");
    print_ext_report(c_set.name1);
    print_ext_report(c_set.name2);
    print_ext_report(c_set.name3);
    print_ext_report(c_set.name4);
    print_ext_report(c_set.name5);
    print_ext_report(c_set.name6);
    
    return 0;
}

int staticOneColor(int dev_number, separate_commands_set *s, int brightness, unsigned char red, unsigned char green, unsigned char blue){
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
            printf("Brightness must be in range of 0..4\n");
            return 1;
    }

    switch (dev_number){
        case 1: //cc 22 04 00 00 00 00 00 00 00 00 01 __ 00 __ __ __ 
            s->name1 = EFFECT_COLOR;
            memcpy(s->dir1, (unsigned char [64]){ 0xcc, 0x22, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x01, brgt, 0x00, blue, green, red, }, 64);
            break;
        case 2: //cc 24 10 00 00 00 00 00 00 00 00 01 __ 00 __ __ __ 
            s->name2 = EFFECT_COLOR;
            memcpy(s->dir2, (unsigned char [64]){ 0xcc, 0x24, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x01, brgt, 0x00, blue, green, red, }, 64);
            break;
        case 3: //cc 25 20 00 00 00 00 00 00 00 00 01 __ 00 __ __ __
            s->name3 = EFFECT_COLOR;
            memcpy(s->dir3, (unsigned char [64]){ 0xcc, 0x25, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x01, brgt, 0x00, blue, green, red, }, 64);
            break;
        case 4: //cc 26 40 00 00 00 00 00 00 00 00 01 __ 00 __ __ __ 
            s->name4 = EFFECT_COLOR;
            memcpy(s->dir4, (unsigned char [64]){ 0xcc, 0x26, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x01, brgt, 0x00, blue, green, red, }, 64);
            break;
        case 5: //cc 27 80 00 00 00 00 00 00 00 00 01 __ 00 __ __ __ 
            s->name5 = EFFECT_COLOR;
            memcpy(s->dir5, (unsigned char [64]){ 0xcc, 0x27, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x01, brgt, 0x00, blue, green, red, }, 64);
            break;
        case 6: //cc 91 00 02 00 00 00 00 00 00 00 01 __ 00 __ __ __ 
            s->name6 = EFFECT_COLOR;
            memcpy(s->dir6, (unsigned char [64]){ 0xcc, 0x91, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x01, brgt, 0x00, blue, green, red, }, 64);
            break;
        default:
            printf("Device number must be in range of 1..6\n");
            return 1;
    } 

    return 0;
}

int staticOff(int dev_number, separate_commands_set *s){
    switch (dev_number){
        case 1: //cc 22 04 00 00 00 00 00 00 00 00 01 ff
            s->name1 = EFFECT_OFF;
            memcpy(s->dir1, (unsigned char [64]){ 0xcc, 0x22, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x01, 0xff, }, 64);
            break;
        case 2: //cc 24 10 00 00 00 00 00 00 00 00 01 ff
            s->name2 = EFFECT_OFF;
            memcpy(s->dir2, (unsigned char [64]){ 0xcc, 0x24, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x01, 0xff, }, 64);
            break;
        case 3: //cc 25 20 00 00 00 00 00 00 00 00 01 ff
            s->name3 = EFFECT_OFF;
            memcpy(s->dir3, (unsigned char [64]){ 0xcc, 0x25, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x01, 0xff, }, 64);
            break;
        case 4: //cc 26 40 00 00 00 00 00 00 00 00 01 ff
            s->name4 = EFFECT_OFF;
            memcpy(s->dir4, (unsigned char [64]){ 0xcc, 0x26, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x01, 0xff, }, 64);
            break;
        case 5: //cc 27 80 00 00 00 00 00 00 00 00 01 ff
            s->name5 = EFFECT_OFF;
            memcpy(s->dir5, (unsigned char [64]){ 0xcc, 0x27, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x01, 0xff, }, 64);
            break;
        case 6: //cc 91 00 02 00 00 00 00 00 00 00 01 ff
            s->name6 = EFFECT_OFF;
            memcpy(s->dir6, (unsigned char [64]){ 0xcc, 0x91, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x01, 0xff, }, 64);
            break;
        default:
            printf("Device number must be in range of 1..6\n");
            return 1;
    }
        
    return 0;
}

int staticImpulse(int dev_number, separate_commands_set *s, int intensity, unsigned char red, unsigned char green, unsigned char blue){
    unsigned char intensity0[6] = { 0x40, 0x06, 0x40, 0x06, 0x20, 0x03 }; // 400640062003
    unsigned char intensity1[6] = { 0xb0, 0x04, 0xb0, 0x04, 0xf4, 0x01 }; // b004b004f401
    unsigned char intensity2[6] = { 0x84, 0x03, 0x84, 0x03, 0xc2, 0x01 }; // 84038403c201
    unsigned char intensity3[6] = { 0xbc, 0x02, 0xbc, 0x02, 0x5e, 0x01 }; // bc02bc025e01
    unsigned char intensity4[6] = { 0xf4, 0x01, 0xf4, 0x01, 0xfa, 0x00 }; // f401f401fa00

    unsigned char* I;

    switch (intensity){
        case 0:
            I = intensity0;
            break;
        case 1:
            I = intensity1;
            break;
        case 2:
            I = intensity2;
            break;
        case 3:
            I = intensity3;
            break;
        case 4:
            I = intensity4;
            break;
        default:
            printf("Intensity must be in range of 0..4\n");
            return 1;
    }

    switch (dev_number){
        case 1: //cc 22 04 00 00 00 00 00 00 00 00 02 64 00 __ __ __ 00 00 00 00 00 __ __ __ __ __ __ 00 00 00 01
            s->name1 = EFFECT_IMPULSE;
            memcpy(s->dir1, (unsigned char [64]){  0xcc, 0x22, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x02, 0x64, 0x00, blue, green, 
                                                   red,  0x00, 0x00, 0x00, 0x00, 0x00, I[0], I[1],   I[2], I[3], I[4], I[5], 0x00, 0x00, 0x00, 0x01, }, 64);
            break;
        case 2: //cc 24 10 00 00 00 00 00 00 00 00 02 64 00 __ __ __ 00 00 00 00 00 __ __ __ __ __ __ 00 00 00 01
            s->name2 = EFFECT_IMPULSE;
            memcpy(s->dir2, (unsigned char [64]){  0xcc, 0x24, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x02, 0x64, 0x00, blue, green, 
                                                   red,  0x00, 0x00, 0x00, 0x00, 0x00, I[0], I[1],   I[2], I[3], I[4], I[5], 0x00, 0x00, 0x00, 0x01, }, 64);
            break;
        case 3: //cc 25 20 00 00 00 00 00 00 00 00 02 64 00 __ __ __ 00 00 00 00 00 __ __ __ __ __ __ 00 00 00 01
            s->name3 = EFFECT_IMPULSE;
            memcpy(s->dir3, (unsigned char [64]){  0xcc, 0x25, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x02, 0x64, 0x00, blue, green, 
                                                   red,  0x00, 0x00, 0x00, 0x00, 0x00, I[0], I[1],   I[2], I[3], I[4], I[5], 0x00, 0x00, 0x00, 0x01, }, 64);
            break;
        case 4: //cc 26 40 00 00 00 00 00 00 00 00 02 64 00 __ __ __ 00 00 00 00 00 __ __ __ __ __ __ 00 00 00 01
            s->name4 = EFFECT_IMPULSE;
            memcpy(s->dir4, (unsigned char [64]){  0xcc, 0x26, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x02, 0x64, 0x00, blue, green, 
                                                   red,  0x00, 0x00, 0x00, 0x00, 0x00, I[0], I[1],   I[2], I[3], I[4], I[5], 0x00, 0x00, 0x00, 0x01, }, 64);
            break;
        case 5: //cc 27 80 00 00 00 00 00 00 00 00 02 64 00 __ __ __ 00 00 00 00 00 __ __ __ __ __ __ 00 00 00 01
            s->name5 = EFFECT_IMPULSE;
            memcpy(s->dir5, (unsigned char [64]){  0xcc, 0x27, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x02, 0x64, 0x00, blue, green, 
                                                   red,  0x00, 0x00, 0x00, 0x00, 0x00, I[0], I[1],   I[2], I[3], I[4], I[5], 0x00, 0x00, 0x00, 0x01, }, 64);
            break;
        case 6: //cc 91 00 02 00 00 00 00 00 00 00 02 64 00 __ __ __ 00 00 00 00 00 __ __ __ __ __ __ 00 00 00 01
            s->name6 = EFFECT_IMPULSE;
            memcpy(s->dir6, (unsigned char [64]){  0xcc, 0x91, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x02, 0x64, 0x00, blue, green, 
                                                   red,  0x00, 0x00, 0x00, 0x00, 0x00, I[0], I[1],   I[2], I[3], I[4], I[5], 0x00, 0x00, 0x00, 0x01, }, 64);
            break;
        default:
            printf("Device number must be in range of 1..6\n");
            return 1;
    }

    return 0;
}
// flash - 1 = brighness; 2 - frequency

int staticFlash(int dev_number, separate_commands_set *s, int brightness, int frequency, unsigned char red, unsigned char green, unsigned char blue){

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
            printf("Brightness must be in range of 0..4\n");
            return 1;
    }

    unsigned char F[2];

    switch (frequency){
        case 0:
            F[0] = 0x60; // 6009
            F[1] = 0x09;
            break;
        case 1:
            F[0] = 0xd0; // d007
            F[1] = 0x07;
            break;
        case 2:
            F[0] = 0x40; // 4006
            F[1] = 0x06;
            break;
        case 3:
            F[0] = 0xb0; // b004
            F[1] = 0x04;
            break;
        case 4:
            F[0] = 0x20; // 2003
            F[1] = 0x03;
            break;
        default:
            printf("Frequency must be in range of 0..4\n");
            return 1;
    }

    switch (dev_number){
        case 1: //cc 22 04 00 00 00 00 00 00 00 00 03 __ 00 __ __ __ 00 00 00 00 00 64 00 64 00 __ __ 00 00 00 01 01
            s->name1 = EFFECT_FLASH;
            memcpy(s->dir1, (unsigned char [64]){ 0xcc, 0x22, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x03, brgt, 0x00, blue, green, 
                                                  red,  0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x00,   0x64, 0x00, F[0], F[1], 0x00, 0x00, 0x00, 0x01, 
    			                                  0x01, }, 64); 
            break;
        case 2: //cc 24 10 00 00 00 00 00 00 00 00 03 __ 00 __ __ __ 00 00 00 00 00 64 00 64 00 __ __ 00 00 00 01 01
            s->name2 = EFFECT_FLASH;
            memcpy(s->dir2, (unsigned char [64]){ 0xcc, 0x24, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x03, brgt, 0x00, blue, green, 
                                                  red,  0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x00,   0x64, 0x00, F[0], F[1], 0x00, 0x00, 0x00, 0x01, 
                                                  0x01, }, 64);
            break;
        case 3: //cc 25 20 00 00 00 00 00 00 00 00 03 __ 00 __ __ __ 00 00 00 00 00 64 00 64 00 __ __ 00 00 00 01 01
            s->name3 = EFFECT_FLASH;
            memcpy(s->dir3, (unsigned char [64]){ 0xcc, 0x25, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x03, brgt, 0x00, blue, green, 
                                                  red,  0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x00,   0x64, 0x00, F[0], F[1], 0x00, 0x00, 0x00, 0x01, 
                                                  0x01, }, 64); 
            break;
        case 4: //cc 26 40 00 00 00 00 00 00 00 00 03 __ 00 __ __ __ 00 00 00 00 00 64 00 64 00 __ __ 00 00 00 01 01
            s->name4 = EFFECT_FLASH;
            memcpy(s->dir4, (unsigned char [64]){ 0xcc, 0x26, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x03, brgt, 0x00, blue, green, 
                                                  red,  0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x00,   0x64, 0x00, F[0], F[1], 0x00, 0x00, 0x00, 0x01, 
                                                  0x01, }, 64); 
            break;
        case 5: //cc 27 80 00 00 00 00 00 00 00 00 03 __ 00 __ __ __ 00 00 00 00 00 64 00 64 00 __ __ 00 00 00 01 01
            s->name5 = EFFECT_FLASH;
            memcpy(s->dir5, (unsigned char [64]){ 0xcc, 0x27, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x03, brgt, 0x00, blue, green, 
                                                  red,  0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x00,   0x64, 0x00, F[0], F[1], 0x00, 0x00, 0x00, 0x01, 
                                                  0x01, }, 64); 
            break;
        case 6: //cc 91 00 02 00 00 00 00 00 00 00 03 __ 00 __ __ __ 00 00 00 00 00 64 00 64 00 __ __ 00 00 00 01 01
            s->name6 = EFFECT_FLASH;
            memcpy(s->dir6, (unsigned char [64]){ 0xcc, 0x91, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x03, brgt, 0x00, blue, green, 
                                                  red,  0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x00,   0x64, 0x00, F[0], F[1], 0x00, 0x00, 0x00, 0x01, 
                                                  0x01, }, 64); 
            break;
        default:
            printf("Device number must be in range of 1..6\n");
            return 1;
    }

    return 0;
}

int staticFlash2(int dev_number, separate_commands_set *s, int brightness, int frequency, unsigned char red, unsigned char green, unsigned char blue){
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
            printf("Brightness must be in range of 0..4\n");
            return 1;
    }
    
    unsigned char F[2];

    switch (frequency){
        case 0:
            F[0] = 0x28; // 280a
            F[1] = 0x0a;
            break;
        case 1:
            F[0] = 0x98; // 9808
            F[1] = 0x08;
            break;
        case 2:
            F[0] = 0x08; // 0807
            F[1] = 0x07;
            break;
        case 3:
            F[0] = 0x78; // 7805
            F[1] = 0x05;
            break;
        case 4:
            F[0] = 0xe8; // e803
            F[1] = 0x03;
            break;
        default:
            printf("Frequency must be in range of 0..4\n");
            return 1;
    }

    switch (dev_number){
        case 1: //cc 22 04 00 00 00 00 00 00 00 00 03 __ 00 __ __ __ 00 00 00 00 00 64 00 64 00 __ __ 00 00 00 01 02
            s->name1 = EFFECT_DOUBLE_FLASH;
            memcpy(s->dir1, (unsigned char [64]){ 0xcc, 0x22, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x03, brgt, 0x00, blue, green, 
                                             red, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x00,    0x64, 0x00, F[0], F[1], 0x00, 0x00, 0x00, 0x01, 
                                             0x02, }, 64); 
            break;
        case 2: //cc 24 10 00 00 00 00 00 00 00 00 03 __ 00 __ __ __ 00 00 00 00 00 64 00 64 00 __ __ 00 00 00 01 02
            s->name2 = EFFECT_DOUBLE_FLASH;
            memcpy(s->dir2, (unsigned char [64]){ 0xcc, 0x24, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x03, brgt, 0x00, blue, green, 
                                             red, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x00,    0x64, 0x00, F[0], F[1], 0x00, 0x00, 0x00, 0x01, 
                                             0x02, }, 64); 
            break;
        case 3: //cc 25 20 00 00 00 00 00 00 00 00 03 __ 00 __ __ __ 00 00 00 00 00 64 00 64 00 __ __ 00 00 00 01 02
            s->name3 = EFFECT_DOUBLE_FLASH;
            memcpy(s->dir3, (unsigned char [64]){ 0xcc, 0x25, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x03, brgt, 0x00, blue, green, 
                                             red, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x00,    0x64, 0x00, F[0], F[1], 0x00, 0x00, 0x00, 0x01, 
                                             0x02, }, 64); 
            break;
        case 4: //cc 26 40 00 00 00 00 00 00 00 00 03 __ 00 __ __ __ 00 00 00 00 00 64 00 64 00 __ __ 00 00 00 01 02
            s->name4 = EFFECT_DOUBLE_FLASH;
            memcpy(s->dir4, (unsigned char [64]){ 0xcc, 0x26, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x03, brgt, 0x00, blue, green, 
                                             red, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x00,    0x64, 0x00, F[0], F[1], 0x00, 0x00, 0x00, 0x01, 
                                             0x02, }, 64); 
            break;
        case 5: //cc 27 80 00 00 00 00 00 00 00 00 03 __ 00 __ __ __ 00 00 00 00 00 64 00 64 00 __ __ 00 00 00 01 02
            s->name5 = EFFECT_DOUBLE_FLASH;
            memcpy(s->dir5, (unsigned char [64]){ 0xcc, 0x27, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x03, brgt, 0x00, blue, green, 
                                             red, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x00,    0x64, 0x00, F[0], F[1], 0x00, 0x00, 0x00, 0x01, 
                                             0x02, }, 64); 
            break;
        case 6: //cc 91 00 02 00 00 00 00 00 00 00 03 __ 00 __ __ __ 00 00 00 00 00 64 00 64 00 __ __ 00 00 00 01 02
            s->name6 = EFFECT_DOUBLE_FLASH;
            memcpy(s->dir6, (unsigned char [64]){ 0xcc, 0x91, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x03, brgt, 0x00, blue, green, 
                                             red, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x00,    0x64, 0x00, F[0], F[1], 0x00, 0x00, 0x00, 0x01, 
                                             0x02, }, 64); 
            break;
        default:
            printf("Device number must be in range of 1..6\n");
            return 1;
    }

    return 0;
}

int staticCycle(int dev_number, separate_commands_set *s, int intensity, int brightness){
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
            printf("Brightness must be in range of 0..4\n");
            return 1;
    }

    unsigned char intensity0[4] = { 0x78, 0x05, 0xb0, 0x04 }; // 7805b004
    unsigned char intensity1[4] = { 0x52, 0x03, 0xee, 0x02 }; // 5203ee02
    unsigned char intensity2[4] = { 0x26, 0x02, 0xc2, 0x01 }; // 2602c201
    unsigned char intensity3[4] = { 0x58, 0x02, 0x90, 0x01 }; // 58029001
    unsigned char intensity4[4] = { 0x90, 0x01, 0xc8, 0x00 }; // 9001c800

    unsigned char* I;
    switch (intensity){
        case 0:
            I = intensity0;
            break;
        case 1:
            I = intensity1;
            break;
        case 2:
            I = intensity2;
            break;
        case 3:
            I = intensity3;
            break;
        case 4:
            I = intensity4;
            break;
        default:
            printf("Intensity must be in range of 0..4\n");
            return 1;
    }
    
    switch (dev_number){
        case 1: //cc 22 04 00 00 00 00 00 00 00 00 04 __ 00 fd 00 fe 00 00 00 00 00 __ __ __ __ 00 00 00 00 07
            s->name1 = EFFECT_CYCLE;
            memcpy(s->dir1, (unsigned char [64]){ 0xcc, 0x22, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x04, brgt, 0x00, 0xfd, 0x00, 
                               0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, I[0], I[1],   I[2], I[3], 0x00, 0x00, 0x00, 0x00, 0x07, }, 64);
            break;
        case 2: //cc 24 10 00 00 00 00 00 00 00 00 04 __ 00 fd 00 fe 00 00 00 00 00 __ __ __ __ 00 00 00 00 07
            s->name2 = EFFECT_CYCLE;
            memcpy(s->dir2, (unsigned char [64]){ 0xcc, 0x24, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x04, brgt, 0x00, 0xfd, 0x00, 
                               0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, I[0], I[1],   I[2], I[3], 0x00, 0x00, 0x00, 0x00, 0x07, }, 64);
            break;
        case 3: //cc 25 20 00 00 00 00 00 00 00 00 04 __ 00 fd 00 fe 00 00 00 00 00 __ __ __ __ 00 00 00 00 07         
            s->name3 = EFFECT_CYCLE;                         
            memcpy(s->dir3, (unsigned char [64]){ 0xcc, 0x25, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x04, brgt, 0x00, 0xfd, 0x00, 
                               0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, I[0], I[1],   I[2], I[3], 0x00, 0x00, 0x00, 0x00, 0x07, }, 64);
            break;
        case 4: //cc 26 40 00 00 00 00 00 00 00 00 04 __ 00 fd 00 fe 00 00 00 00 00 __ __ __ __ 00 00 00 00 07
            s->name4 = EFFECT_CYCLE;
            memcpy(s->dir4, (unsigned char [64]){ 0xcc, 0x26, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x04, brgt, 0x00, 0xfd, 0x00, 
                               0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, I[0], I[1],   I[2], I[3], 0x00, 0x00, 0x00, 0x00, 0x07, }, 64);
            break;
        case 5: //cc 27 80 00 00 00 00 00 00 00 00 04 __ 00 fd 00 fe 00 00 00 00 00 __ __ __ __ 00 00 00 00 07 
            s->name5 = EFFECT_CYCLE;
            memcpy(s->dir5, (unsigned char [64]){ 0xcc, 0x27, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x04, brgt, 0x00, 0xfd, 0x00, 
                               0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, I[0], I[1],   I[2], I[3], 0x00, 0x00, 0x00, 0x00, 0x07, }, 64);
            break;
        case 6: //cc 91 00 02 00 00 00 00 00 00 00 04 __ 00 fd 00 fe 00 00 00 00 00 __ __ __ __ 00 00 00 00 07
            s->name6 = EFFECT_CYCLE;
            memcpy(s->dir6, (unsigned char [64]){ 0xcc, 0x91, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x04, brgt, 0x00, 0xfd, 0x00, 
                               0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, I[0], I[1],   I[2], I[3], 0x00, 0x00, 0x00, 0x00, 0x07, }, 64);
            break;
        default:
            printf("Device number must be in range of 1..6\n");
            return 1;
    }

    return 0;
}