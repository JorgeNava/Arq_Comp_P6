/*
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file    Practica6.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL27Z644.h"
#include "fsl_debug_console.h"
#include "LikeArduino.h"
#include <time.h>
#define LIMIT_VALUE 10
#define MAX_TIME 160.0
/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */
void adcInit(void);

void adcInit(){
	int * p2SIM_SCGC5;
	int * p2SIM_SCGC6;
	int * p2PORTE_PCR21;
	int * p2ADC0_SC1A;
	int * p2ADC0_CFG1;
	int * p2ADC0_CFG2;

	// Enable SIM_SCGC5
	p2SIM_SCGC5 = 0x40048038;
	*p2SIM_SCGC5 = 0X2000;

	// Enable SIM_SCGC6 for ADC
	p2SIM_SCGC6 =  0X4004803C;
	*p2SIM_SCGC6 = 0X8000000;

	// Configure PTE20 as analog in MUX
	//p2PORTE_PCR20 = 0x4004D050;
	//*p2PORTE_PCR20 = 0x0;

	// Configure PTE21 as analog in MUX
	p2PORTE_PCR21 = 0x4004D054;
	*p2PORTE_PCR21 = 0x0;

	// Configure  ADC Configuration Register 1
	p2ADC0_CFG1 = 0x4003B008;
	*p2ADC0_CFG1 = 0x0;

	// Configure  ADC Configuration Register 2
	p2ADC0_CFG2 = 0x4003B00C;
	*p2ADC0_CFG2 = 0x0;

	// Configure ADC Status and Control Registers 1
	p2ADC0_SC1A = 0x4003B000;
	*p2ADC0_SC1A = 0x4;
}

int main(void) {

	adcInit();
	char * p2ADC0_RA;
	//clock_t time = clock();
	time_t tiempo;
	char limitBroken = 0;
	int actualState = 0;
	long counter;
	long timelapse = 9;


	// Configure ADC Data Result Register
	p2ADC0_RA = 0x4003B010;

	pinMode('A', 1, OUTPUT); //Button configuration

	pinMode('B', 18, OUTPUT); // Red
	pinMode('B', 19, OUTPUT); //Green
	pinMode('A', 13, OUTPUT); //Blue
	digitalWrite('B', 18, OFF);
	digitalWrite('B', 19, OFF);
	digitalWrite('A', 13, OFF);
	while(1){
		adcInit();
		timelapse++;
		printf("Valor temperatura: %d \n",*p2ADC0_RA);
//		printf("tiempo: %d \n",(clock() - time)/CLOCKS_PER_SEC);
//		printf("booleano: %d \n",(float)((clock() - time)/CLOCKS_PER_SEC) >= MAX_TIME);
//		printf("booleano float in: %d \n",((float)(clock() - time)/CLOCKS_PER_SEC) >= MAX_TIME);
//		printf("Tiempo trans: %d \n",((float)(clock() - time)));
//		printf("valor clock: %d \n",(float)clock());
//		printf("valor time: %d \n",time);
		//printf("Tiempo trans %d \n",timelapse);
		if (*p2ADC0_RA >= LIMIT_VALUE && !limitBroken){
			//printf("Se reinicia la cuenta\n");
			//time = clock();
			tiempo = time(NULL);
			limitBroken = 1;
			timelapse=0;
		}else if (*p2ADC0_RA >= LIMIT_VALUE && timelapse >= MAX_TIME){
			//BOUNCE LED FUNCTION
			//printf("%Boton> %d\n",digitalRead('A',1));
			while(!digitalRead('A',1)){
				counter = 1000000;
				//printf("LLegamos a prender los leds\n");
				digitalWrite('B', 18, OFF);
				digitalWrite('B', 19, OFF);
				digitalWrite('A', 13, OFF);
				switch(actualState){
					case 0:
						 //Red
						digitalWrite('B', 18, ON);
						break;
					case 1:
						//Green
						digitalWrite('B', 19, ON);
						break;
					case 2:
						//Blue
						digitalWrite('A', 13, ON);
						break;
					case 3:
						//White
						digitalWrite('B', 18, ON);
						digitalWrite('B', 19, ON);
						digitalWrite('A', 13, ON);
						break;
					case 4:
						//Blue
						digitalWrite('A', 13, ON);
						break;
					case 5:
						//Green
						digitalWrite('B', 19, ON);
						break;
					case 6:
						//Red
						digitalWrite('B', 18, ON);
						actualState = -1;
						break;

				}
				actualState ++;
				while(counter){
					//printf("Counter: %d\n", counter);
					counter--;
				}
			}
		} else if (*p2ADC0_RA < LIMIT_VALUE){
			limitBroken = 0;
		}
	}
    return 0 ;
}
