This repo is a fork. The original repo contains an implementation of the Homa transport protocol as a Linux kernel module.

- For more information on Homa in general, see the [Homa
  Wiki](https://homa-transport.atlassian.net/wiki/spaces/HOMA).

- More information about this implementation and its performance are available in
  the paper [A Linux Kernel Implementation of the Homa Transport
  Protocol](https://www.usenix.org/system/files/atc21-ousterhout.pdf),
  which appeared in the USENIX Annual Technical Conference in July, 2021.

- A synopsis of the protocol implemented by this module is available in
  [protocol.md](https://github.com/PlatformLab/HomaModule/blob/master/protocol.md).

### Branch Guide
Keeping track of the work I've done on Homa:
- **multi-link:** Modifies Homa so we can run experiments on a Mininet setup without the pacer hindering host throughput. 
- **immediate-resend:** Modifies the server so it immediately issues RESENDs in cases the clients request RESENDs for incomplete incoming messages. 
- **micro-benchmark:** Incorporates [Google's Benchmark tool](https://github.com/google/benchmark) into Homa's unit test functions for fine-grained benchmarking of the module in user space.

For more information refer to the README.md on each branch. 