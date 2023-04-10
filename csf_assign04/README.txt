CONTRIBUTIONS

Hannah wrote most of the code for parsort including the merge, parallelism, and error handling. Dru helped clear warnings, collected the data, and wrote the report.

REPORT
For each threshold, parsort was run 10+ times on copies of the 16MB of random data. The lowest time is then taken to be
the most representative time, with the assumption that the large deviance between runs of the same threshold is due to
other processes' usage of the CPU, so that the lowest time is the time resulting from the lowest interference from
outside sources.

Data:
The result from parsorts of different threshold are as follows:
time ./parsort /tmp/$(whoami)/test_16M.in 2097152
real    0m0.373s
user    0m0.356s
sys     0m0.016s

time ./parsort /tmp/$(whoami)/test_16M.in 1048576
real    0m0.214s
user    0m0.361s
sys     0m0.023s

time ./parsort /tmp/$(whoami)/test_16M.in 524288
real    0m0.171s
user    0m0.414s
sys     0m0.036s

time ./parsort /tmp/$(whoami)/test_16M.in 262144
real    0m0.164s
user    0m0.465s
sys     0m0.050s

time ./parsort /tmp/$(whoami)/test_16M.in 131072
real    0m0.149s
user    0m0.475s
sys     0m0.082s

time ./parsort /tmp/$(whoami)/test_16M.in 65536
real    0m0.145s
user    0m0.511s
sys     0m0.083s

time ./parsort /tmp/$(whoami)/test_16M.in 32768
real    0m0.153s
user    0m0.515s
sys     0m0.125s

time ./parsort /tmp/$(whoami)/test_16M.in 16384
real    0m0.168s
user    0m0.531s
sys     0m0.177s

Analysis:

The real time each threshold takes generally decreases with increasing the threshold (increasing parallelism), with the decreases
getting smaller over time (diminishing returns).The reason for diminishing returns is because as more proccesses are done in
parallel, overhead is added for switching between proccesses on the same core and the increased time needed to manage more proccesses.
Also, more merges must happen when more chunks of data are partitioned apart, adding to the time and decreasing gains in efficiency.
In addition, after a threshold of 65536 (32 parallel sequential sorts), the time starts to increase. This is because a CPU 
only has so many cores, which means it can only run so many processes in parallel (atthe same time). After the CPU becomes
saturated with simultaneous proccesses, whenever a new process is added, the CPU must switch between the proccesses
(adding to the over head as process switching is expensive). Thus, parallelism in this program is beneficial, up until
the CPU becomes too saturated and needs to constantly spend time switching between proccesses, slowing down
the overall speed. The CPU has context switching overhead which is where the CPU needs to save the current state of the process
it is doing before loading in the state of the other process it wants to do, so the switching back and forth can be much slower
than just going through and finishing one process before going onto the next one. Seeing as for this program, 32 parallel sorts
is the best case, the CPU probably has 32 cores since any more processes start to slow down (indicating increased overhead from
switching).

For this program, the sorts on the different partitions of data (2^n partitions for n forks) can be sorted in parallel as the
data each sort works on and accesses are independent. Thus, the OS kernel can schedule all the sorting of left and right halves
in parrallel, and after the left and right chunks are sorted in parrallel, they are merged. This program is sped up with
parallelism as it allows the sorting of data simultaneously, but after saturating the CPU or adding too many merge processes
that are not parallelized, program no longer speeds up from increased parallelism and starts to suffer.