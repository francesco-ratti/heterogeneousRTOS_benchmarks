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


static void prvTaskFour( void *pvParameters );
#include <stdio.h>


//static FAULTDETECTOR_region_t trainedRegions[FAULTDETECTOR_MAX_CHECKS][FAULTDETECTOR_MAX_REGIONS];
//static u8 n_regions[FAULTDETECTOR_MAX_CHECKS];


int main( void )
{
#ifdef configSCHEDULER_SOFTWARE
	xRTTaskCreate( prvTaskFour,
			( const char * ) "Four",
			configMINIMAL_STACK_SIZE,
			NULL,
			tskIDLE_PRIORITY,
			NULL,
			1, //deadline
			1, //period
			0,
			1
	); //wcet
#else
	xRTTaskCreate( prvTaskFour,
			( const char * ) "Four",
			configMINIMAL_STACK_SIZE,
			NULL,
			tskIDLE_PRIORITY,
			NULL,
			99997, //deadline
			99997, //period
			0,
			49999
	); //wcet
#endif
	//	xRTTaskCreate( prvTaskFour,
	//			( const char * ) "Four",
	//			configMINIMAL_STACK_SIZE,
	//			NULL,
	//			tskIDLE_PRIORITY,
	//			NULL,
	//			10, //deadline
	//			10, //period
	//			1,
	//			2,
	//			2*2
	//			); //wcet

	//	for (int i=0; i<FAULTDETECTOR_MAX_CHECKS; i++) {
	//		n_regions[i]=0;
	//		for (int j=0; j<FAULTDETECTOR_MAX_REGIONS; j++) {
	//			for (int k=0; k<=FAULTDETECTOR_MAX_AOV_DIM; k++) {
	//				trainedRegions[i][j].center[k]=0.0f;
	//				trainedRegions[i][j].max[k]=0.0f;
	//				trainedRegions[i][j].min[k]=0.0f;
	//			}
	//		}
	//
	//	}

/*	vTaskStartFaultDetector(
			//#ifdef trainMode
			//			0, //do not load from sd, load from supplied trainedRegions and n_regions instead
			//#else
			//			1,
			//#endif
			0,
			trainedRegions,
			n_regions);
	//	xil_printf("%u\n", sizeof(FAULTDETECTOR_testpointShortDescriptorStr));*/

	vTaskStartScheduler();

	//should never reach this point
	for( ;; );
}



#include "perf_timer.h"

static void prvTaskFour( void *pvParameters )
{
	for (;;) {
		vTaskJobEnd();
	}
}

