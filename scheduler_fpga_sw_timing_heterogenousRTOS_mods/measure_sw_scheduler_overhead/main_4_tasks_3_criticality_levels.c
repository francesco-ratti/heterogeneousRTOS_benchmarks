//TICKS 20Hz


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

#include "perf_timer.h"

static void prvTaskOne( void *pvParameters );
static void prvTaskTwo( void *pvParameters );
static void prvTaskThree( void *pvParameters );
static void prvTaskFour( void *pvParameters );
#include <stdio.h>

//static FAULTDETECTOR_region_t trainedRegions[FAULTDETECTOR_MAX_CHECKS][FAULTDETECTOR_MAX_REGIONS];
//static u8 n_regions[FAULTDETECTOR_MAX_CHECKS];

int main( void )
{
	xRTTaskCreate( prvTaskOne,
			( const char * ) "One",
			configMINIMAL_STACK_SIZE,
			NULL,
			tskIDLE_PRIORITY,
			NULL,
			12, //deadline
			12, //period
			2,
			1,
			2,
			3
	); //wcet
	xRTTaskCreate( prvTaskTwo,
			( const char * ) "Two",
			configMINIMAL_STACK_SIZE,
			NULL,
			tskIDLE_PRIORITY,
			NULL,
			12, //deadline
			12, //period
			2,
			1,
			2,
			3
	); //wcet
	xRTTaskCreate( prvTaskThree,
			( const char * ) "Three",
			configMINIMAL_STACK_SIZE,
			NULL,
			tskIDLE_PRIORITY,
			NULL,
			12, //deadline
			12, //period
			2,
			1,
			2,
			3
	); //wcet
	xRTTaskCreate( prvTaskFour,
			( const char * ) "Four",
			configMINIMAL_STACK_SIZE,
			NULL,
			tskIDLE_PRIORITY,
			NULL,
			12, //deadline
			12, //period
			2,
			1,
			2,
			3
	); //wcet
//	xRTTaskCreate( prvTaskOne,
//			( const char * ) "One",
//			configMINIMAL_STACK_SIZE,
//			NULL,
//			tskIDLE_PRIORITY,
//			NULL,
//			20, //deadline
//			20, //period
//			0,
//			2
//	); //wcet
//	xRTTaskCreate( prvTaskTwo,
//			( const char * ) "Two",
//			configMINIMAL_STACK_SIZE,
//			NULL,
//			tskIDLE_PRIORITY,
//			NULL,
//			10, //deadline
//			10, //period
//			0,
//			1
//	); //wcet
//	xRTTaskCreate( prvTaskThree,
//			( const char * ) "Three",
//			configMINIMAL_STACK_SIZE,
//			NULL,
//			tskIDLE_PRIORITY,
//			NULL,
//			5, //deadline
//			5, //period
//			0,
//			1
//	); //wcet
//	xRTTaskCreate( prvTaskFour,
//			( const char * ) "Four",
//			configMINIMAL_STACK_SIZE,
//			NULL,
//			tskIDLE_PRIORITY,
//			NULL,
//			12, //deadline
//			12, //period
//			0,
//			2
//	); //wcet


//	vTaskStartFaultDetector(
//			//#ifdef trainMode
//			//			0, //do not load from sd, load from supplied trainedRegions and n_regions instead
//			//#else
//			//			1,
//			//#endif
//			0,
//			trainedRegions,
//			n_regions);
//	//	xil_printf("%u\n", sizeof(FAULTDETECTOR_testpointShortDescriptorStr));
//
	vTaskStartScheduler();

	//should never reach this point
	for( ;; );
}

#include "perf_timer.h"

/*-----------------------------------------------------------*/
static void prvTaskOne( void *pvParameters )
{
	for (;;) {
//		xil_printf("One %u\n", get_clock_L());
		xil_printf("%u\n", get_clock_L());
		if (get_clock_U()!=0)
			xil_printf("err up not 0");

		for(;;) {}
//		vTaskJobEnd();
	}
}

/*-----------------------------------------------------------*/
static void prvTaskTwo( void *pvParameters )
{
	for (;;) {
//		xil_printf("Two %u\n", get_clock_L());
		xil_printf("%u\n", get_clock_L());
		if (get_clock_U()!=0)
			xil_printf("err up not 0");

		for(;;) {}
//		vTaskJobEnd();
	}
}

static void prvTaskThree( void *pvParameters )
{
	for (;;) {
//		xil_printf("Three %u\n", get_clock_L());
		xil_printf("%u\n", get_clock_L());
		if (get_clock_U()!=0)
			xil_printf("err up not 0");

		for(;;) {}
//		vTaskJobEnd();
	}
}

static void prvTaskFour( void *pvParameters )
{
	for (;;) {
//		xil_printf("Four %u\n", get_clock_L());
		xil_printf("%u\n", get_clock_L());
		if (get_clock_U()!=0)
			xil_printf("err up not 0");

		for(;;) {}
//		vTaskJobEnd();
	}
}

