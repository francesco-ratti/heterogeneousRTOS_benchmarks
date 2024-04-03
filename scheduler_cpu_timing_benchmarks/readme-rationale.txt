this is a shorter version of the complete "reexec" case, completely equivalent but shorter to execute, altough it requires a modification to tasks.c.

the goal is to have as WC in one scheduler execution:
activation of ALL tasks
one WCET exceeded

The scheduler will always execute the FIRST job among all the highest priority ones -> only job 1 is executed (re-executed)

We measure the re-execution time and subtract the min from all the timing obtained with this code for the no-reexecution case.
