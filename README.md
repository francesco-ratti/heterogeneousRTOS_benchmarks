Source code used to perform all the benchmarks in [HeterogeneousRTOS](https://github.com/francesco-ratti/heterogeneousRTOS) paper experimental evaluation.

<h3>Folder meaning</h3>
<strong>faultdet_performance_measurement_standalone</strong>: stand-alone Eclipse C Project for executing fault detector detection performances benchmarks.

<strong>faultdet_timing_benchmarks</strong>: benchmarks for measuring fault detection online (when a false positive is detected) and offline training time, for both fault detector on FPGA and the software one.

<strong>faultdet_timing_sources</strong>: fault detector (FPGA, HLS) source code, optimised for specific "AOV" sizes (depending on the benchmark) and regions number and Vivado HW platform project (only fault detector - scheduler removed to save resources) for measuring the fault detection timing with "faultdet_timing_benchmarks".

<strong>scheduler_fpga_sw_timing_heterogenousRTOS_mods</strong>: tasks and HeterogeneousRTOS source code modifications to measure the scheduler overhead for the FPGA version and for the software version. In particular:\
<strong>measure_ctx_switch_overhead_fpga</strong>: context switch overhead for the scheduler on FPGA.\
<strong>measure_sw_scheduler_overhead</strong>: (scheduler execution and context switch overhead) (atomic) measurement for software scheduler.\
<strong>measure_jobend_ovh</strong>: overhead to signal when a job completes (both for FPGA and for software scheduler).\
<strong>measure_measurement_overhead_ctx_switch_fpga_and_sw</strong>: measurement overhead when measuring context switch overhead for the FPGA scheduler and for the software scheduler.

<strong>scheduler_schedules_simulations</strong>: task sets to test the scheduler and measure timing.

<h3>Related repositories:</h3>

[HeterogeneousRTOS Source Code](https://github.com/francesco-ratti/heterogeneousRTOS)\
[HeterogeneousRTOS Vivado Platform](https://github.com/francesco-ratti/heterogeneousRTOS_HW)\
[Fault Detector Vitis HLS Project](https://github.com/francesco-ratti/heterogeneousRTOS_faultDetector_HLS)\
[Data processing and analysis scripts](https://github.com/francesco-ratti/heteregeneousRTOS_benchmarks_data_analysis_scripts)
