This repo contains an implementation of the Homa transport protocol as a Linux kernel module.

- For more information on Homa in general, see the [Homa
  Wiki](https://homa-transport.atlassian.net/wiki/spaces/HOMA).

- More information about this implementation and its performance are available in
  the paper [A Linux Kernel Implementation of the Homa Transport
  Protocol](https://www.usenix.org/system/files/atc21-ousterhout.pdf),
  which appeared in the USENIX Annual Technical Conference in July, 2021.

- A synopsis of the protocol implemented by this module is available in
  [protocol.md](https://github.com/PlatformLab/HomaModule/blob/master/protocol.md).

## Branch gso
To be able to benchmark and profile different module functionalities, I have created C micro-benchmark 
units in the _tests_ directory and linked them with a C++ code that registers and benchmarks those units 
using [Google's Benchmark library](https://github.com/google/benchmark). That way, we can write custom 
units and benchmark them, then profile the benchmarks in user space:

1. Add a unit benchmark to the test file (_unit_homa_offload.c_ in this case) in the _tests_ direcotry using the `BENCH_F` macro: 
```
BENCH_F(homa_offload, homa_gso_segment)
{
    mock_ipv6 = false;
    struct sk_buff *skb = mock_skb_new(&self->ip, &self->header.common,
     1400, 2000);
    struct sk_buff *segs = homa_gso_segment(skb, 0);
	kfree_skb(skb);
	kfree_skb(segs->next);
	kfree_skb(segs);
}
```
2. Register any subset of the benchmarks you want to run in the corresponding bench file (_bench_homa_offload.cc_ in this case) using the `BENCH` macro: 
```
BENCH(homa_offload, homa_gso_segment)
```
3. Finally, make and run: 
```
$ ./bench

2024-04-15T18:20:15+00:00
Running unknown
Run on (48 X 3000 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x24)
  L1 Instruction 32 KiB (x24)
  L2 Unified 1024 KiB (x24)
  L3 Unified 16896 KiB (x2)
Load Average: 1.00, 1.02, 1.01
--------------------------------------------------------------------------------
Benchmark                                      Time             CPU   Iterations
--------------------------------------------------------------------------------
wrapper_homa_offload_homa_gso_segment      21158 ns        21154 ns        33164
```

We can also profile any homa function, e.g. `homa_sendmsg`, on the fly using dynamic tracepints. First, we have to add a probe:
```
perf probe -v -m ./HomaModule/homa.ko --add="homa_sendmsg"
```

Then we should record while the module is live and tranmissions are occurring:
```
perf record -e probe:homa_sendmsg -a -g
```

To generate a report and inspect it(you may or may not need to specify the path the _kallsyms_ files): 
```
perf report -g "graph,0.5,caller" --kallsyms=/proc/kallsyms
```

Useful resources for the perf tool (besides the man pages of course):
- https://alex.dzyoba.com/blog/kernel-profiling/
- https://jvns.ca