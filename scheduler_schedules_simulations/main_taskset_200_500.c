/*
    Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
    Copyright (c) 2012 - 2022 Xilinx, Inc. All Rights Reserved.
	SPDX-License-Identifier: MIT


    http://www.FreeRTOS.org
    http://aws.amazon.com/freertos


    1 tab == 4 spaces!
 */

#include "FreeRTOS.h"
#include "task.h"
#include "xil_printf.h"

#include "portable.h"

#include "math.h"
#include "perf_timer.h"

static void prvTaskOne( void *pvParameters );
static void prvTaskTwo( void *pvParameters );

#include <stdio.h>

unsigned int task1tims[500];
int t1idxctr=-1;


unsigned int task2tims[500];
int t2idxctr=-1;


//static FAULTDETECTOR_region_t trainedRegions[FAULTDETECTOR_MAX_CHECKS][FAULTDETECTOR_MAX_REGIONS];
//static u8 n_regions[FAULTDETECTOR_MAX_CHECKS];

int main( void )
{
	//	xRTTaskCreate( prvTaskOne,
	//			( const char * ) "One",
	//			configMINIMAL_STACK_SIZE,
	//			NULL,
	//			tskIDLE_PRIORITY,
	//			NULL,
	//			60000, //deadline
	//			60000, //period
	//			1,
	//			22000
	//	); //wcet

	xRTTaskCreate( prvTaskOne,
			( const char * ) "One",
			configMINIMAL_STACK_SIZE,
			NULL,
			tskIDLE_PRIORITY,
			NULL,
			500, //deadline
			500, //period
			0,
			200
	); //wcet
	xRTTaskCreate( prvTaskTwo,
			( const char * ) "Two",
			configMINIMAL_STACK_SIZE,
			NULL,
			tskIDLE_PRIORITY,
			NULL,
			900, //deadline
			900, //period
			0,
			500
	); //wcet

//	xRTTaskCreate( prvTaskOne,
//			( const char * ) "One",
//			configMINIMAL_STACK_SIZE,
//			NULL,
//			tskIDLE_PRIORITY,
//			NULL,
//			600, //deadline
//			600, //period
//			1,
//			220,
//			440
//	); //wcet
//	xRTTaskCreate( prvTaskTwo,
//			( const char * ) "Two",
//			configMINIMAL_STACK_SIZE,
//			NULL,
//			tskIDLE_PRIORITY,
//			NULL,
//			900, //deadline
//			900, //period
//			0,
//			200
//	); //wcet

	for(int i=0; i<500000000; i++){}


	//	vTaskStartFaultDetector(
	//			//#ifdef trainMode
	//			//			0, //do not load from sd, load from supplied trainedRegions and n_regions instead
	//			//#else
	//			//			1,
	//			//#endif
	//			0,
	//			trainedRegions,
	//			n_regions);
	//	//	xil_xil_printf("%u\n", sizeof(FAULTDETECTOR_testpointShortDescriptorStr));
	//

	for (int i=0; i<500; i++) {
		task1tims[i]=0;
		task2tims[i]=0;
	}
	perf_reset_and_start_clock();
	vTaskStartScheduler();


	//should never reach this point
	for( ;; );
}

#include "perf_timer.h"



/*-----------------------------------------------------------*/

static void prvTaskOne( void *pvParameters )
{
	for (;;) {
		//		xPortSchedulerDisableIntr();

		//		fflush(stdout);
		unsigned int time=get_clock_L();

		//		j1ctr++;
		t1idxctr++;
		task1tims[t1idxctr]=time;
		t1idxctr++;

		//		sxil_printf(task1vals[t1idxctr], "T1 S J%u %u\n", j1ctr, time);

//		if (t1idxctr==15) {
//			for(;;);
//		}

		if (t1idxctr>=499 || get_clock_U()!=0) {
			portDISABLE_INTERRUPTS();
			xPortSchedulerDisableIntr();

			int i = 0, j = 0;

			while (i <= t1idxctr && j <= t2idxctr) {
				if (task1tims[i] < task2tims[j]) {
					if (i%2==0)
						xil_printf("T1;S;%u;%u\n", i/2, task1tims[i]);//, task1tims[i]*1000/325);
					else
						xil_printf("T1;E;%u;%u\n", i/2, task1tims[i]);//, task1tims[i]*1000/325, (task1tims[i]-task1tims[i-1])*1000/325);
					i++;
				}
				else {
					if (j%2==0)
						xil_printf("T2;S;%u;%u\n", j/2, task2tims[j]);//, task2tims[j]*1000/325);
					else
						xil_printf("T2;E;%u;%u\n", j/2, task2tims[j]);//, task2tims[j]*1000/325, (task2tims[j]-task2tims[j-1])*1000/325);
					j++;
				}
			}

			while (i<=t1idxctr) {
				if (i%2==0)
					xil_printf("T1;S;%u;%u\n", i/2, task1tims[i]);//, task1tims[i]*1000/325);
				else
					xil_printf("T1;E;%u;%u\n", i/2, task1tims[i]);//, task1tims[i]*1000/325, (task1tims[i]-task1tims[i-1])*1000/325);
				i++;
			}

			while (j<=t2idxctr) {
				if (j%2==0)
					xil_printf("T2;S;%u;%u\n", j/2, task2tims[j]);//, task2tims[j]*1000/325);
				else
					xil_printf("T2;E;%u;%u\n", j/2, task2tims[j]);//, task2tims[j]*1000/325, (task2tims[j]-task2tims[j-1])*1000/325);
				j++;
			}

			//			for (int i=0; i<t1idxctr; i++) {
			//				if (i%2==0)
			//					xil_printf("T1 START J%u TIME %u, IN NS %u\n", i/2, task1tims[i], task1tims[i]*1000/325);
			//				else
			//					xil_printf("T1 END J%u TIME %u, IN NS %u, EXECTIME %u\n", i/2, task1tims[i], task1tims[i]*1000/325, (task1tims[i]-task1tims[i-1])*1000/325);
			//			}
			//
			//			for (int i=0; i<t2idxctr; i++) {
			//				if (i%2==0)
			//					xil_printf("T2 START J%u TIME %u, IN NS %u\n", i/2, task2tims[i], task2tims[i]*1000/325);
			//				else
			//					xil_printf("T2 END J%u TIME %u, IN NS %u, EXECTIME %u\n", i/2, task2tims[i], task2tims[i]*1000/325, (task2tims[i]-task2tims[i-1])*1000/325);
			//			}
			for(;;);
		}


		for(int i=0;i<610;i++) {}

//		for(int i=0;i<680;i++) {}

		//		sxil_printf(task1vals[t1idxctr], "T1 E J%u %u\n", j1ctr, get_clock_L());

		task1tims[t1idxctr]=get_clock_L();

		vTaskJobEnd();
	}
}


/*-----------------------------------------------------------*/


static void prvTaskTwo( void *pvParameters )
{
	for (;;) {
		//		xPortSchedulerDisableIntr();

		//		fflush(stdout);
		unsigned int time=get_clock_L();

		//		j1ctr++;
		t2idxctr++;
		task2tims[t2idxctr]=time;
		t2idxctr++;

		//		sxil_printf(task1vals[t1idxctr], "T1 S J%u %u\n", j1ctr, time);

		if (t1idxctr>=499 || get_clock_U()!=0) {
			portDISABLE_INTERRUPTS();
			xPortSchedulerDisableIntr();

			int i = 0, j = 0;

			while (i <= t1idxctr && j <= t2idxctr) {
				if (task1tims[i] < task2tims[j]) {
					if (i%2==0)
						xil_printf("T1;S;%u;%u\n", i/2, task1tims[i]);//, task1tims[i]*1000/325);
					else
						xil_printf("T1;E;%u;%u\n", i/2, task1tims[i]);//, task1tims[i]*1000/325, (task1tims[i]-task1tims[i-1])*1000/325);
					i++;
				}
				else {
					if (j%2==0)
						xil_printf("T2;S;%u;%u\n", j/2, task2tims[j]);//, task2tims[j]*1000/325);
					else
						xil_printf("T2;E;%u;%u\n", j/2, task2tims[j]);//, task2tims[j]*1000/325, (task2tims[j]-task2tims[j-1])*1000/325);
					j++;
				}
			}

			while (i<=t1idxctr) {
				if (i%2==0)
					xil_printf("T1;S;%u;%u\n", i/2, task1tims[i]);//, task1tims[i]*1000/325);
				else
					xil_printf("T1;E;%u;%u\n", i/2, task1tims[i]);//, task1tims[i]*1000/325, (task1tims[i]-task1tims[i-1])*1000/325);
				i++;
			}

			while (j<=t2idxctr) {
				if (j%2==0)
					xil_printf("T2;S;%u;%u\n", j/2, task2tims[j]);//, task2tims[j]*1000/325);
				else
					xil_printf("T2;E;%u;%u\n", j/2, task2tims[j]);//, task2tims[j]*1000/325, (task2tims[j]-task2tims[j-1])*1000/325);
				j++;
			}

			//			for (int i=0; i<t1idxctr; i++) {
			//				if (i%2==0)
			//					xil_printf("T1 START J%u TIME %u, IN NS %u\n", i/2, task1tims[i], task1tims[i]*1000/325);
			//				else
			//					xil_printf("T1 END J%u TIME %u, IN NS %u, EXECTIME %u\n", i/2, task1tims[i], task1tims[i]*1000/325, (task1tims[i]-task1tims[i-1])*1000/325);
			//			}
			//
			//			for (int i=0; i<t2idxctr; i++) {
			//				if (i%2==0)
			//					xil_printf("T2 START J%u TIME %u, IN NS %u\n", i/2, task2tims[i], task2tims[i]*1000/325);
			//				else
			//					xil_printf("T2 END J%u TIME %u, IN NS %u, EXECTIME %u\n", i/2, task2tims[i], task2tims[i]*1000/325, (task2tims[i]-task2tims[i-1])*1000/325);
			//			}
			for(;;);
		}

		for(int i=0;i<1500;i++) {}
//		for(int i=0;i<610;i++) {}


		//		sxil_printf(task1vals[t1idxctr], "T1 E J%u %u\n", j1ctr, get_clock_L());

		task2tims[t2idxctr]=get_clock_L();

		vTaskJobEnd();
	}
}

