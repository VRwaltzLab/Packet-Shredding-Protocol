# Math
So we are assuming by default that each packet's delay follows some distribution D which is dependent
on the number of packets sent per frame n and bandwidth amount B

### Statistics:
|Location | upload ping | upload jitter | upload IQR | download ping | download jitter | download IQR | Notes |
| -- | -- | -- | -- | -- | -- | -- | -- |
| House Wifi in garage | 26ms | 8.2ms | 9.16ms | 78.9ms | 73.4ms | 52.3ms | no download |
| House Wifi in garage 2nd run| 26ms | 15ms | 6.2ms | 49.9ms | 63.8ms | 53.6ms | see csv |
| Tethered Hotspot in garage | 43.6ms | 5.9ms| 5.9ms | 42.6ms | 82.6ms | 3.6ms | see csv  |
| untethered Hotspot in garage | 36.6ms | 66.9ms | 25.2ms |40.5ms | 507ms| 14.6ms| see csv |

Hotspot tests reported around 1Mbps, Wifi reported 100Mbps 
Conclusion there was substantially more jitter & IQR in the fourth test to the third.
Thus, some part of the artifact is wifi being weird.
[https://radar.cloudflare.com/quality/us ]
Cloudflare lists for the US the following stats on 2024-4-30
Latency
36.5 ms (idle)
140.6 ms (loaded)
Jitter
8.9 ms (idle)
38.7 ms (loaded)
There may be a sampling bias. This might be sampling from people checking their internet quality only.
### Hard to refute assumption:
According to this [https://www.meter.net/tools/world-ping-test/], a large portion of the world has a ping of 100ms -200ms to my location. Comparing its unloaded latency to LA of 36ms or 11ms, to the above. This ping might depend heavily on internet traffic.
Assuming a 10% jitter to Round trip Time ratio, I care about 10% jitter on a 100ms connection, because 10ms while less than a beat is more than a frame at 120fps.
Assumption refutation for unloaded setting near midnight pacific time [https://docs.google.com/spreadsheets/d/1hQGThnwCvQz7-UHJyse2K0WiQhXg0L9oZysCIixtwkg/edit?usp=sharing]
Assumption confirmation for unloaded setting near 10am pacific time (second page of same document) R^2 > .25

## Why do Packet Shredding

In long distance time sensitive communications during congested timeframes, Jitter is notable enough > 10ms that one might be able to improve it with a codec < 0.1 ms. 
Packet shredding is going to attempt to do that by using the independence of paths within a large network.
Will it be a notable improvement? 
That will depend on how realistic the independence of packet delay assumption is.
It will also depend on how dependent the packet delay distribution is on bandwidth, packet count and packet size...
(smaller packets might have more paths and thus move faster over a network)
### Harder Math
#### Assumption 1:
Delay equation Citation [https://packetpushers.net/blog/average-network-delay/]
Total Delay = Processing Delay + Transmission Delay + Propagation Delay + Queuing Delay
#### Claim 2:
Processing delays are increased from O(h) to worst case O(hf) in a h hop f fragment setting.
Since each fragment has its own header. Some proprotion of the processing delay is proportional to bits and thus hb -> hb/f in the best case depending on if these things happen in parallel.
#### Claim 3:
Over a single path network, Packet shredding decreases Total Transmission delays.
Proof: Transmission delays are per bit delays at every link and so are proportional to the number of hops h  and number of bits but when a packet is broken into many fragments, those delays can be experienced in parallel ie. Fragment 1 can be transmited while fragment 2 is being recieved by a link. 
Best case O(b* max(h/f,1))
Worst case O(b*h) all of them keep sharing the same queue
#### Assumption 4:
Over a c-connected network, we expect behaviour to be similar to a c parallel paths with 1 sink an 1 source.
#### Claim 5:
Over a c-parallel path with sink and source network, when f =c and each fragment takes a different path:
The transmission Delay and processing Delay decrease because each bit has fewer bits in its fragment to wait on.
#### Conclusion 1:
In the network as a whole smaller packet sizes improves delays.
Essentially, there is a decrease in transmission delay proportional to h*(b-b/f) with an added header transmission delay p(f-1) where b byte message sent over f fragments with empty packet size(header size) p. And a unknown potential for causing its own queueing delay.
Example 100Mbps, 1MB message over 10 hops  64byte header
2 fragments decrease: 512KB(1024B/KB)(8b/B)(1/100Mbps)(1Mb/1024kb)(1k/1024b) = 40ms decrease
2 fragments increase: 64B(8b/B)(1/100Mbps)(1Mb/1024kb)(1k/1024b) = 4.8us (microseconds)
1024 fragment decrease: 1023KB(1024B/KB)(8b/B)(1/100Mbps)(1Mb/1024Kb)(1Kb/1024b) = 79.9ms decrease
1024 fragment increase: 64B(1023headers)(8b/B)(1/100Mbps)(1Mb/1024Kb)(1Kb/1024b) = 5.00ms increase



