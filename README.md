# Packet-Shredding-Protocol
Trading more bandwidth for lower jitter and latency

## Basic Idea
1. 1 message becomes k packets via subdivision
2. k packets become n packets via an error correcting code
3. n packets sent
4. k packets waited for to come in correct.
5. k packets decoded into original k packets
6. 1 message reassembled via concatenation.

## Error Correcting Code
This first version of this protocol will use the Reed Solomon Code of size (n,k) over GF(2^16).
With that choice of a finite field n is at most 2^16.
By locking the number of packets we wait for to the size of the code, we are asserting no error occur in the packets, only the late, incorrectly signed, or missing packets occur as errasures.
If the symmetric key is powerful enough to prevent eavesdropping, it should be powerful enought to detect errors.
If we use only errasures, we can do klogk interpolation and nlogk evaluation in order to decode and encode the data.

## Key Assumptions
1. Packets vary in size.
2. Packets can take different routes through a network; therefore, packets have noteworthily independent delays.
3. Packet jitter, and median lag are higher than desired for chronologically-sensitive applications.
