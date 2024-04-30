# Packet-Shredding-Protocol
Trading more bandwidth for lower jitter and latency

## Basic Idea
1. Alice has 1 big message: m (an 2Lk byte message)
2. Alice breaks it into k small packets: M (a k by L matrix of 2 bytes) 
3. Alice spreads that data over n small packet via an error correcting code: N = EM (a n by L matrix of 2 bytes)
4. Alice signs each small packet with a symmetric key: S (each row of N then gets signed)
5. Alice sends all n packets to Bob.
6. Bob waits for only k correctly signed packets from alice: R (each packet is a row R) 
7. Bob recovers the k small packets Alice originally had by interpreting the late (or incorrectly signed) packets as erasures in the error correcting code: T = E'R 
8. Bob reorganizes the data into the original 1 big message t
E is a n by k matrix that evaluates the k term polynomial at n points.
E' is a k by k matrix that interpolates the k term polynomial at the k locations based on which packets get through.
If the process works m == t.

## Error Correcting Code
This first version of this protocol will use the Reed Solomon Code of size (n,k) over GF(2^16).
With that choice of a finite field n is at most 2^16.
By locking the number of packets we wait for to the size of the code, we are asserting no error occur in the packets, only the late, incorrectly signed, or missing packets occur as errasures.
If the symmetric key is powerful enough to prevent eavesdropping, it should be powerful enought to detect errors.
If we use only errasures, we can do klogk interpolation and nlogn evaluation in order to decode and encode the data.

## Key Assumptions
1. Packets vary in size.
2. Packets can take different routes through a network; therefore, packets have noteworthily independent delays.
3. Packet jitter, and median lag are higher than desired for chronologically-sensitive applications.
