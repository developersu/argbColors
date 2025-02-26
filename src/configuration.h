#define nDEBUG

void print_array(unsigned char* buf, int size);
int writeUsb(unsigned char *message);
int readUsb();
int configure_device();

libusb_device_handle* dev_handle;
const uint16_t VID = 0x048d;
const uint16_t PID = 0x5711;
unsigned int TIMEOUT = 2000;
int counter; 
int limit;