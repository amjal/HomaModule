This repo is a fork. The original repo contains an implementation of the Homa transport protocol as a Linux kernel module.

- For more information on Homa in general, see the [Homa
  Wiki](https://homa-transport.atlassian.net/wiki/spaces/HOMA).

- More information about this implementation and its performance are available in
  the paper [A Linux Kernel Implementation of the Homa Transport
  Protocol](https://www.usenix.org/system/files/atc21-ousterhout.pdf),
  which appeared in the USENIX Annual Technical Conference in July, 2021.

- A synopsis of the protocol implemented by this module is available in
  [protocol.md](https://github.com/PlatformLab/HomaModule/blob/master/protocol.md).

## Branch multi-link
There is value in enabling Homa on Mininet: it allows researchers/developers to develop and test simple 
stuff against the module on their local systems with 0 cost and 0 waiting time (given the system has enough resources
to support the emulated network). A Mininet network creates virtual interfaces for its switches and its hosts
all sharing the resources (e.g. memory, CPU, and NICs) of the "main" host machine that is hosting the emulated network. 
So when we set up a Mininet network on the "main" host machine, we are essentially hosting multiple links that the 
"main" host machine's IP stack routes and sends packets to.
However, the current implmentation of Homa's Linux kernel module
assumes there is only one NIC on the host and creates one pacer thread of type _pacer_kthread_ to handle the NIC queues regardless of how
many NICs are on a host. 

Because of the mentioned single-NIC perception by the module, an issue arises when one NIC has a tx queue backlog that exceeds the `.net.homa.max_nic_queue_ns`
parameter; then no more packets are added to any other virtual queues until that one queue is cleared. This wastes bandwidth with
the virtual interfaces that are sharing the pacer thread. This [issue](https://github.com/PlatformLab/HomaModule/issues/53) currently exists and has not yet been addressed. 

This branch manipulates the _pacer_thread_ functionality to avoid virtual link starvation. But this is at the cost of 
SRPT on the sender side. A better solution would be to use Linux's queue disciplines instead of a custom pacer for Homa. 

Right now this branch is functional for Mininet setups. To be able to use the _cp_node_ programs to generate traffic using Mininet hosts you would have to modify the `/etc/hosts` file and add entries to alias node1, node2, ... with the appropriate IP addresses. 