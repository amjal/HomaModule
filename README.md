This repo is a fork. The original repo contains an implementation of the Homa transport protocol as a Linux kernel module.

- For more information on Homa in general, see the [Homa
  Wiki](https://homa-transport.atlassian.net/wiki/spaces/HOMA).

- More information about this implementation and its performance are available in
  the paper [A Linux Kernel Implementation of the Homa Transport
  Protocol](https://www.usenix.org/system/files/atc21-ousterhout.pdf),
  which appeared in the USENIX Annual Technical Conference in July, 2021.

- A synopsis of the protocol implemented by this module is available in
  [protocol.md](https://github.com/PlatformLab/HomaModule/blob/master/protocol.md).

## Branch immediate-resend
This branch implements the following task mentioned in notes.txt: 
>Don't need to detect timeouts on servers; do it on clients, and if
a server gets a RESEND for a request whose incoming message isn't
complete, it can use the incoming RESEND request to issue RESENDs
back to the client.

We have to consider different possible scenarios and just checking if the message is not in `RPC_OUTGOING`
state and immediately issueing a RESEND will lead to network over-utilization in cases such as those where
the total message sizes incoming to a server exceed the `.net.homa.max_incoming` parameter. The solution 
is to reuse the funcionality in [`homa_get_resend`](https://github.com/PlatformLab/HomaModule/blob/a3fb70e6bd3439e39696b783e3831bbd23787c17/homa_incoming.c#L301) 
to check if there are gaps or ungranted seq numbers in the incoming message. Find the pull request [here](https://github.com/PlatformLab/HomaModule/pull/54) and the commit that resolves it [here](https://github.com/PlatformLab/HomaModule/commit/1391cd7396b203a1ab2eaeda5c2a28eb36c4301c).