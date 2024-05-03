# Math Motivating Packet Shredding
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
Assumption confirmation for unloaded setting near 10am pacific time (second page of same document) R^2 > .24

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
#### Assumption 6:
Transmission delay = packet size /transmission speed
#### Example 7:
100Mbps, 1MB message over 10 hops  64byte header
2 fragments decrease: 512KB(1024B/KB)(8b/B)(1/100Mbps)(1Mb/1024kb)(1k/1024b) = 40ms decrease
2 fragments increase: 64B(8b/B)(1/100Mbps)(1Mb/1024kb)(1k/1024b) = 4.8us (microseconds)
1024 fragment decrease: 1023KB(1024B/KB)(8b/B)(1/100Mbps)(1Mb/1024Kb)(1Kb/1024b) = 79.9ms decrease
1024 fragment increase: 64B(1023headers)(8b/B)(1/100Mbps)(1Mb/1024Kb)(1Kb/1024b) = 5.00ms increase
#### Conclusion 8:
In the network as a whole smaller packet sizes improves delays.
Essentially, there is a decrease in transmission delay proportional to h*(b-b/f) with an added header transmission delay p(f-1) where b byte message sent over f fragments with empty packet size(header size) p. And a unknown potential for causing its own queueing delay. This [paper](https://link.springer.com/chapter/10.1007/978-981-10-2035-3_14) explores the effect, but has calculation errors, and I don't trust its optimality claims as its assumptions are overly simplistic.
#### Conjecture/Hypothesis 9:
Transmission delays might account for most of the loaded vs unloaded latency.
#### Claim 10:
When n > k,  n-k late packets can be ignored that is the point of the encoding scheme.
This means that in a congested environment more congestion can be potentially dodged.
Specifically the probability of q percentile or better latency = B(n,q/100) >=k
The thing that makes this hard to analyze is that only congestion based variance will be effected by this.
Variance which happens to all the packets will not be minimized.
#### Claim 11:
As n grows, the congestion variance should shrink as paths can be utilized in more nuanced ways because of more discretization; but also as the median (for example n = 2k) of a large sample stabilizes [https://stats.stackexchange.com/questions/45124/central-limit-theorem-for-sample-medians].
The distribution of sample medians is connected to our case as a set of packets sent at one time are basically a convenience sample of the delay under those network and load conditions.
Since it is a convenience sample we have to worry about errors more than usual, but we can still assume with enough parallelism/independence in the network that the distribution is roughly normal.
As n/k grows, more late packets can be ignored and a better percentile is available.
#### Conclusion 12:
As n grows long range congestion jitter should shrink..
#### Should we use packet shredding under VoIP? Q13
Probably not for a latency speed up:
38 Bytes L1 and L2 , 20 bytes IPv4, 8 bytes for UDP, packet shredding 16 bytes, RTP 12 bytes VoIP payload 160 bytes
172 payload, 82 header:
r transmission speed, h hops: Max speed up = (172(1-1/N)h -82N)/r
for hops = 8, maximum is at N = 4 -> 704 bytes speed up
At  Gigabit speeds, speedup is negligible.
At  1 Megabit speeds, speedup is 5ms. Thus competes with the codec time for wether or not to do it.
#### Should we use packet shredding under a Music over IP? Q14
48khz sampling rate, 16bit samples, 30ms sample period:
48khz(1000hz/khz)(1s/1000ms)-> (48samples/ms)
(48samples/ms)(30ms)(2B/sample)-> 2,880B payload 82B header
hops = 8, (2880(1-1/N)h - 82N)/r
N = 6 -> 18,708B speedup ->  18708B(8b/B)(1s/Gigabit)(1Gb/1024Mb)(1Mb/1024kb)(1Kb/1024b) -> 0.1ms speedup at Gigabit rates.
#### Conclusion 15:
This is much more a successful argument against jumbograms than an argument to do anything fancy.
#### Should we use packet shredding ever under IPv4? Q16
64KB packet; 82B header:
r transmission speed, h hops: Max speed up = (64(1024)(1-1/N)h -82N)/r
16 hops -> N = 112 ->64KB(1024B/KB)(111/112)(16hops) - 82B(112fragments) = 1,030,029B
->7.67ms at 1 gigabit transmission speeds
#### REMARK 17
Ethernet varies from 1Mbps to 400Gbps. So all these calculations become dependent on that, in the delay context.
Furthermore, different hops have different trsansmission speeds, so the total transmission delay speedup is not (h/r)(message_size)(1-1/n), but something more complicated.
#### If we are transmitting object transforms over the network, are we trying to transmit jumbograms? Q18
30 transforms(3 scale + 3 orientation + 3 position)(4B floating point) = 1080B, so probably not.
### Conclusion for the VRwaltzLab:
Some users will have a bad enough connection in a specific way (one of the hops near their location has low transmission speed) that packet shredding (or the finer packetization and error correction) could be worth it. Some users pairs will have a long enough connection that congestion jitter will be significant enough to be minimized by packet shredding (sending n, and waiting for k < n ). These two use case are tenuous enough though that this protocol ought to determine whether or not it will be beneficial in the setup or resetup phase.
