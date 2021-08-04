#define OUTPUT 1
#define INPUT 0
#define FIRST_CLOCK_PORT 0x200
#define SCGC5 0x40048038
#define PCR 0x40049000
#define GPIO 0x100
#define PDDR 0x400FF014
#define ON 0
#define OFF 1
#define PDOR 0x400FF000
#define PDIR 0x400FF000

void pinMode(unsigned char ubPort, unsigned char ubPin, unsigned char ubMode);
void digitalWrite(unsigned char ubPort, unsigned char ubPin, unsigned char ubValue);
unsigned char digitalRead(unsigned char ubPort, unsigned char ubPin);
