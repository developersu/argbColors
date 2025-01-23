void print_array(unsigned char* buf, int size){

    int j = 0;
	for(int i = 0; i < size; i++) {
		printf("%02x ", buf[i]);

		if (++j % 8 == 0)
    		printf(" ");

		if (j % 16 == 0)
    		printf("\n");
	} 
	printf("\n");
}