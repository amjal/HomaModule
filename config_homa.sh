#!/bin/bash

NR_HOSTS=14

# Default value is 25000
sysctl .net.homa.link_mbps=100

# Default value is 50 us
sysctl .net.homa.poll_usecs=5

# Default value is 8
sysctl .net.homa.max_overcommit=$(( $NR_HOSTS * 8 ))
#sysctl .net.homa.max_overcommit=140

# Default is 400000
sysctl .net.homa.max_incoming=$(( $NR_HOSTS * 400000 ))

# Default is 50/1000
sysctl .net.homa.grant_fifo_fraction=50

# Default is 50/1000
sysctl .net.homa.pacer_fifo_fraction=50

# Default is 5
sysctl .net.homa.timeout_resends=100

# Default is 10000, modified to get more and smaller packets
# this way we can keep the retransmission timeout lower as 
# queueing delay is directly proportional to packet size
sysctl .net.homa.max_gso_size=1500

# Default is 15
sysctl .net.homa.resend_ticks=500

# Default is 10 
sysctl .net.homa.resend_interval=250


# Default is 10000
sysctl .net.homa.unsched_bytes=33000

# Default is 10000
sysctl .net.homa.window=33000

#sysctl .net.homa.verbose=10

echo "Remember to set high priority for Homa kernel module using:
		chrt -p 99 [PID]"


# The way the two parameters resend_ticks and resend_interval cooperate is 
# the module first counts until resend_ticks. Then it requests resend. Then
# every millisecond or tick after that it can potentially send another 
# resend request because it does not reset the resend_tick counter. But 
# it actually does not request resend every tick after that but every 
# once in resend_interval ticks. This goes one until for some reason such as 
# receiving a grant on the client side the resend_tick interval is reset and 
# then eveyrthing goes back to the beginning: a counter needs to reach 
# resend_ticks for the first resend request and then every resend_interval
# after that subsequent resend requests are sent. 
