Comparing block sizes, write-allocate vs no-write-allocate, and write-through vs write-back:
We will compare the comands and results here. We vary the blocks per set and bytes per block, keeping the number
of sets constant. We also go through every combination of wa+wt, wa+wb, and nwa+wt. Fifo is used for all caches,
as we will compare eviction algorithms later. We examine caches with 256 sets with either 4 blocks of 16 bytes per block,
8 blocks per set with 8 bytes per block, or 16 blocks with 4 bytes per block.

RESULTS:
./csim 256 4 16 write-allocate write-through fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 314171
Load misses: 4026
Store hits: 188047
Store misses: 9439
Total cycles: 25475701

./csim 256 4 16 write-allocate write-back fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 314171
Load misses: 4026
Store hits: 188047
Store misses: 9439
Total cycles: 9699148

./csim 256 4 16 no-write-allocate write-through fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 311017
Load misses: 7180
Store hits: 163705
Store misses: 33781
Total cycles: 22945977

./csim 256 8 8 write-allocate write-through fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 312729
Load misses: 5468
Store hits: 179369
Store misses: 18117
Total cycles: 24825499

./csim 256 8 8 write-allocate write-back fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 312729
Load misses: 5468
Store hits: 179369
Store misses: 18117
Total cycles: 7709868

./csim 256 8 8 no-write-allocate write-through fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 308693
Load misses: 9504
Store hits: 160459
Store misses: 37027
Total cycles: 21977101

./csim 256 16 4 write-allocate write-through fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 312031
Load misses: 6166
Store hits: 169388
Store misses: 28098
Total cycles: 23555559

./csim 256 16 4 write-allocate write-back fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 312031
Load misses: 6166
Store hits: 169388
Store misses: 28098
Total cycles: 4812147

./csim 256 16 4 no-write-allocate write-through fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 307867
Load misses: 10330
Store hits: 159398
Store misses: 38088
Total cycles: 21110127

DISCUSSION:
As we can note, as we increase the number of blocks per set and decrease the number
of bytes per block, the total cycles decreases. However, the hit rate also decreases as we get more misses
as the blocks per set increases while bytes per block decreases for 256 sets. Ultimately, lower total cycles
is better for cache design, but the reason for the increase in hit rate while the cycles decrease is due to
locality. As bytes per blocks increases, misses get more expensive by that factor (16 bytes per block is 4x as expensive to miss
as 4 bytes per block). However, for 4 bytes per blocks the miss rate is less than 4x the miss rate of 16 bytes per block
(in fact it is less than 2x the miss rate), so that is why overall the cycle count is down. This implies that the locality
of the gcc.trace used is between 4 and 8 bytes because the cycle count still continues to go down while the miss rate increases.

Then, comparing the three different combinations, we can see that for all three numerical parameters,
write allocate and write back produces the lowest cycle count and the best hit rate! Along with the conclusion
above about the blocks per set and bytes per block, the best of these 9 combinations would be...
./csim 256 16 4 write-allocate write-back fifo < gcc.trace

Now, we will move to compare eviction types.

Comparing LRU vs. fifo:
We will keep the caches with write-allocate and write-back (best as concluded before), and compare LRU and fifo for
the 3 different numerical parameters from before.

RESULTS:

./csim 256 4 16 write-allocate write-back fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 314171
Load misses: 4026
Store hits: 188047
Store misses: 9439
Total cycles: 9699148

./csim 256 4 16 write-allocate write-back lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 314798
Load misses: 3399
Store hits: 188250
Store misses: 9236
Total cycles: 9197918

./csim 256 8 8 write-allocate write-back fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 312729
Load misses: 5468
Store hits: 179369
Store misses: 18117
Total cycles: 7709868

./csim 256 8 8 write-allocate write-back lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 313484
Load misses: 4713
Store hits: 179709
Store misses: 17777
Total cycles: 7370773

./csim 256 16 4 write-allocate write-back fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 312031
Load misses: 6166
Store hits: 169388
Store misses: 28098
Total cycles: 4812147

./csim 256 16 4 write-allocate write-back lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 312729
Load misses: 5468
Store hits: 169711
Store misses: 27775
Total cycles: 4652426

DISCUSSION:
For the cases above, lru outperforms fifo. This makes sense because if you have not accessed the
memory in a while, you probably won't access it again in a while, so that will decrease miss rate
because you will not be evicting a block that you will be using again. Meanwhile, with fifo if you load
a frequently used block first you will still end up evicting a highly used block and result with higher misses.

Direct mapped, set associative, and fully associative:
From above, we will use the best settings we found so far write allocate, write back, and lru. To find
the best cache between direct mapped, set assocative, and fully associative we will vary the numerical
parameters while keeping the cache size constant.

RESULTS:

Set associative:
./csim 256 16 4 write-allocate write-back lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 312729
Load misses: 5468
Store hits: 169711
Store misses: 27775
Total cycles: 4652426

Direct mapping:
./csim 4096 1 4 write-allocate write-back lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 310335
Load misses: 7862
Store hits: 168767
Store misses: 28719
Total cycles: 5120764

Fully associative:
./csim 1 4096 4 write-allocate write-back lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 312902
Load misses: 5295
Store hits: 169919
Store misses: 27567
Total cycles: 4587945

DISCUSSION:
Fully associative results in the best total cycle and hit rate, but this is not realistic. From a hardware
perspective, this would be a lot of parallel wiring so that would be impossible to implement in the real
world. Therefore, the set associative cache, which has only SLIGHTLY worse total cycles and hit rate, would
be the best cache to use. Direct mapping is far behind the other two.

CONCLUSTION:
A set associative cache with these parameters (./csim 256 16 4 write-allocate write-back lru < gcc.trace) results
in the best cycle count and thus has best overall effectiveness.

We contributed equally to this project. Dru focused more on the main function and the metrics, while Hannah focused more on the load and store implementation. 