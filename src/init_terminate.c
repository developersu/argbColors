int init_sequence(){
	counter = 0;
	limit = 18; // Initialization flow contains 18 I/O events

	unsigned char message[64] = { 0xcc, 0x60, };
	if(64 != writeUsb(message))
		return 1;

	if(64 != readUsb())
		return 1;

	message[1] = 0x60;
	if(64 != writeUsb(message))
		return 1;

	if(64 != readUsb())
		return 1;

	message[1] = 0x34;
	if(64 != writeUsb(message))
		return 1;

	message[1] = 0x48;
	if(64 != writeUsb(message))
		return 1;

	if(64 != readUsb())
		return 1;

	message[1] = 0x61;
	if(64 != writeUsb(message))
		return 1;
	
	if(64 != readUsb())
		return 1;

	message[1] = 0x3c;
	if(64 != writeUsb(message))
		return 1;

	message[1] = 0x3e;
	if(64 != writeUsb(message))
		return 1;
	
	if(64 != readUsb())
		return 1;

	message[1] = 0x34;
	if(64 != writeUsb(message))
		return 1;

	message[1] = 0x3d;
	if(64 != writeUsb(message))
		return 1;

	message[1] = 0x3f;
	if(64 != writeUsb(message))
		return 1;

	if(64 != readUsb())
		return 1;

	message[1] = 0x34;
	if(64 != writeUsb(message))
		return 1;

	message[1] = 0x38;
	if(64 != writeUsb(message))
		return 1;
	
	printf("\rInitialization sequence sent\n");
	
	return 0;
}

int terminate_sequence(){
	counter = 0;
	limit = 1;

	unsigned char message[64] = { 0xcc, 0x47, 0x01, }; //cc4701
	if(64 != writeUsb(message))
		return 1;
	
	printf("\rTermination sequence sent\n");

	return 0;
}