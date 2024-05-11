Source code used to perform all the benchmarks in [HeterogeneousRTOS](https://github.com/francesco-ratti/heterogeneousRTOS) paper experimental evaluation.

<h3>Folder meaning</h3>
faultdet_performance_measurement_standalone: stand-alone Eclipse C Project for executing fault detector detection performances benchmarks.

faultdet_timing_benchmarks: benchmarks for measuring fault detection online (when a false positive is detected) and offline training time, for both fault detector on FPGA and the software one.

faultdet_timing_sources: fault detector (FPGA, HLS) source code, optimised for specific "AOV" sizes (depending on the benchmark) and regions number and Vivado HW platform project (only fault detector - scheduler removed to save resources) for measuring the fault detection timing with "faultdet_timing_benchmarks".

scheduler_fpga_sw_timing_heterogenousRTOS_mods: tasks and HeterogeneousRTOS source code modifications to measure the scheduler overhead on FPGA (communication overhead in the context switch, communication overhead when a job completes, etc) and with the software scheduler (overhead when a job completes to mark it as ended in the scheduler data structures, etc.).
measure_ctx_switch_overhead_fpga: context switch overhead for the scheduler on FPGA.\
measure_sw_scheduler_overhead: (scheduler execution and context switch overhead) (atomic) measurement for software scheduler.\
measure_jobend_ovh: overhead to signal when a job completes (both for FPGA and for software scheduler).\
measure_measurement_overhead_ctx_switch_fpga_and_sw: measurement overhead when measuring context switch overhead for the FPGA scheduler and for the software scheduler.\

scheduler_schedules_simulations: task sets to test the scheduler and measure timing.

<h3>Related repositories:</h3>

[HeterogeneousRTOS Source Code](https://github.com/francesco-ratti/heterogeneousRTOS)\
[HeterogeneousRTOS Vivado Platform](https://github.com/francesco-ratti/heterogeneousRTOS_HW)\
[Fault Detector Vitis HLS Project](https://github.com/francesco-ratti/heterogeneousRTOS_faultDetector_HLS)\
[Benchmarks source code and experimental evaluation artifacts](https://github.com/francesco-ratti/heterogeneousRTOS_benchmarks)\
[Data processing and analysis scripts](https://github.com/francesco-ratti/heteregeneousRTOS_benchmarks_data_analysis_scripts)
