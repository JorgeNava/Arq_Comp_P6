#include "LikeArduino.h"
#include <math.h>

void pinMode(unsigned char ubPort, unsigned char ubPin, unsigned char ubMode){
	unsigned int *pHelper, *pPCR, *pPDDR;
	unsigned int offset = (ubPort - 'A');
	unsigned int clock = FIRST_CLOCK_PORT;
	unsigned int bitHelper = 1;
	pHelper = SCGC5;
	// f(x) = 512 (2^offset)
	*pHelper = *pHelper | clock<<offset;//enciende el reloj del puerto
	//*pHelper = *pHelper | (FIRST_CLOCK_PORT*((int)pow(2.0, (double)offset)));
	pPCR = PCR + (0x1000 * offset) + (ubPin*4);// Se asigna la direccion de memoria inicial para los PCRs de los puertos
	*pPCR = GPIO;
	pPDDR = PDDR + (64 * offset);// Se le suma 64 por que la diferencia entre cada direccions de PDDr en hex es 40 = 64 en decimal
	bitHelper = bitHelper<<ubPin;// Hace la mascara para el OR con *pPDDR
	if(ubMode){
		*pPDDR = *pPDDR | bitHelper;
	}else{
		*pPDDR = *pPDDR & ~bitHelper;
	}
	return;
}
void digitalWrite(unsigned char ubPort, unsigned char ubPin, unsigned char ubValue){
	unsigned int offset = (ubPort - 'A');
	unsigned int *pPDOR;
	unsigned int bitHelper = 1;
	pPDOR =  PDOR + (64 * offset);// Se le suma 64 por que la diferencia entre cada direccions de PDOR en hex es 40 = 64 en decimal
	bitHelper = bitHelper<<ubPin;// Hace la mascara para el OR con *pPDDR
		if(ubValue){
			*pPDOR = *pPDOR | bitHelper;
		}else{
			*pPDOR = *pPDOR & ~bitHelper;
		}
	return;
}

unsigned char digitalRead(unsigned char ubPort, unsigned char ubPin){
	unsigned int offset = (ubPort - 'A');
	unsigned int *pPDIR;
	unsigned int bitHelper = 1;
	unsigned int returnValue;
	pPDIR =  PDIR + (64 * offset);// Se le suma 64 por que la diferencia entre cada direccions de PDOR en hex es 40 = 64 en decimal
	bitHelper = bitHelper<<ubPin;// Hace la mascara para el OR con *pPDDR

	returnValue = *pPDIR & bitHelper;

	return returnValue? 1:0;
}
