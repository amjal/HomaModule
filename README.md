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
From notes.txt, the aim of this branch is to:
>Improve software GSO by making segments refer to the initial large buffer instead of copying. 

To be able to benchmark and profile the `homa_gso_segment` function, I have created C micro-benchmark 
units in the _tests_ directory and linked them with a C++ code that registers and benchmarks those units 
using [Google's Benchmark library](https://github.com/google/benchmark). That way, we can write custom units and benchmark them, then profile the benchmarks.

We can also profile the `homa_gso_segment` function on the fly using dynamic tracepints. First, we have to add a probe event:
```
perf probe -v -m ./HomaModule/homa.ko --add="homa_gso_segment"
```

Then we can record while some transmission is occuring:
```
perf record -e probe:homa_gso_segment -a -g
```

To generate the report (you may or may not need to specify the path the _kallsyms_ files): 
```
perf report -g "graph,0.5,caller" --kallsyms=/proc/kallsyms
```

The `homa_gso_segment` function is the `gso_segment` handler for Homa registered as a callback through this code in homa_offload.c: 
```
static const struct net_offload homa_offload = {
	.callbacks = {
		.gso_segment	=	homa_gso_segment,
		.gro_receive	=	homa_gro_receive,
		.gro_complete	=	homa_gro_complete,
	},
};
```