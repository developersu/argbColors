unsigned char end_transaction[64] = { 0xcc, 0x28, 0xff, 0x07, }; //cc28ff07

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
    unsigned char dir1[64] = { 0xcc, 0x22, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x01, brightness, 0x00, blue, green, red };
    //cc 24 10 00 00 00 00 00 00 00 00 01__00______
    unsigned char dir2[64] = { 0xcc, 0x24, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x01, brightness, 0x00, blue, green, red };
    //cc 25 20 00 00 00 00 00 00 00 00 01__00______
    unsigned char dir3[64] = { 0xcc, 0x25, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x01, brightness, 0x00, blue, green, red };
    //cc 26 40 00 00 00 00 00 00 00 00 01__00______
    unsigned char dir4[64] = { 0xcc, 0x26, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x01, brightness, 0x00, blue, green, red };
    //cc 27 80 00 00 00 00 00 00 00 00 01__00______
    unsigned char dir5[64] = { 0xcc, 0x27, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x01, brightness, 0x00, blue, green, red };
    //cc 91 00 02 00 00 00 00 00 00 00 01__00______
    unsigned char dir6[64] = { 0xcc, 0x91, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x01, brightness, 0x00, blue, green, red };
    
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