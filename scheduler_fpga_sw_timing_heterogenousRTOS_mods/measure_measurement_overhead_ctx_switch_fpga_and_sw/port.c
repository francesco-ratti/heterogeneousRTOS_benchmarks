//called by the ASM function executed on FPGA scheduler interrupt
//it parses the data provided by the FPGA scheduler and prepares the system for the context switch
void xPortScheduleNewTask(void)
{
	SCHEDULER_ACKInterrupt((void *) SCHEDULER_BASEADDR);

	xil_printf("%u\n", get_clock_L());
	if (get_clock_U()!=0)
		xil_printf("err up not 0");
	perf_reset_clock();
}

