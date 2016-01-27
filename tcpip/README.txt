test code to try out tcp/ip performance over wifi on the Red Pitaya (RP)

the RP is setup as an wifi access point at IP 10.1.1.1

compile & start the code on the RP specifying a tcp port:

gcc server.c -o server; ./server 1234

on the host machine, run python client.py, making sure that the TCP_PORT value matches the one used on the server


server.c will send out buffers of increasing sizes a number of times (see MIN_DATA_SIZE & MAX_DATA_SIZE and TRIES)

client.py will receive these buffers, check the values of the first and last 4 bytes of each, and, after server.c has sent all of the buffers, print out a table of buffer sizes vs. transfer speeds (in bytes/second).


at a very short distance, with the RP setup as an 802.11g 2.4ghz access point using wpa2 (with a theoretical limit of 54mbps), the transfer speed averaged 3 megabytes / second (e.g., 24mbps) between the RP and a recent macbook pro.

disabling wpa2 made for a marginal performance in speed: about 4% faster. see below.

----
example run -- unprotected network
1000 2811600.86742
2000 3016226.32283
3000 3249802.93913
4000 3705297.37848
5000 3522147.86742
6000 3760001.85267
7000 3714924.79714
8000 3860811.89456
9000 3875841.20121

example run -- wpa2
1000 2638234.00836
2000 2862000.04776
3000 3140655.84082
4000 3508802.42771
5000 3479473.91911
6000 3579416.85791
7000 3634803.83782
8000 3693176.83523
9000 3730897.51104
----