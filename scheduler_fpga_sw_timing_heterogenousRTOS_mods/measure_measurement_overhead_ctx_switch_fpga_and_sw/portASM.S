
/******************************************************************************
 * vPortHandleNewTask is used to perform a context switch when a new task is provided by the FPGA scheduler (hardware scheduler interrupt handler).
 *****************************************************************************/
.type vPortHandleNewTask, %function
vPortHandleNewTask:
	//start timer for measure context switch overhead for scheduler on FPGA
	subs	lr, lr, #4			/* adjust return */
	portSAVE_CONTEXT
	push    {r1}
	push    {r2}
	ldr     r1, =9
	ldr     r2, =0xF8F00208
        str	r1, [r2]
	pop     {r2}
	pop     {r1}
	//stop timer
	push    {r1}
	push    {r2}
	ldr     r1, =0
	ldr     r2, =0xF8F00208
    	str	r1, [r2]
    	pop     {r2}
    	pop     {r1}
	LDR R0, xPortScheduleNewTaskConst
	BLX	R0
	portRESTORE_CONTEXT

