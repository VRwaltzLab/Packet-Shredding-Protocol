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

## Why do Packet Shredding

In long distance time sensitive operations, Jitter is notable enough > 10ms that one might be able to improve it with a codec < 1 ms. 
Packet shredding is going to attempt to do that.
By using the independence of paths within a large network.
Will it be a notable improvement? *shrug*
