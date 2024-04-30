# Packet-Shredding-Protocol
Trading more bandwidth for lower jitter and latency
## Basic Idea
1. Alice has 1 big message
2. Alice breaks it into k small packets
3. Alice spreads that data over n small packet via an error correcting code.
4. Alice sends all n packets to Bob.
5. Bob waits for only k packets from alice.
6. Bob recovers the k small packets Alice originally had by interpreting the late packets as erasures in the error correcting code
7. Bob reorganizes the data into the original 1 big message
## Key Assumptions
1. Packets vary in size.
2. Packets can take different routes through a network; therefore, packets have noteworthily independent delays.
3. Packet jitter, and median lag are higher than desired for chronologically-sensitive applications.
