# loosely based on https://docs.python.org/2/library/socket.html#example

import socket
from timeit import default_timer as timer
import struct

TCP_IP = "10.1.1.1"
TCP_PORT = 4843

sock = socket.socket(socket.AF_INET,
                     socket.SOCK_STREAM)
sock.settimeout(5)
sock.connect((TCP_IP, TCP_PORT))

def recv_len(n):
	data = ''
	while len(data) < n:
		data += sock.recv(n - len(data))
	return data

total_times = {}

while True:
	data_len = struct.unpack('<L', recv_len(4))[0]
	if data_len == 0:
		break
	tries = struct.unpack('<L', recv_len(4))[0]

	total_time = 0
	for _ in range(tries):
		start = timer()
		got_back = recv_len(data_len)
		total_time += (timer() - start)
		if got_back[:4] != 'DEAD' or got_back[-4:] != 'BEEF':
			print 'ERROR did not get DEAD///BEEF'

	total_times[data_len] = total_time / float(tries)
	print '.',

for k in sorted(total_times.keys()):
	print k, k / total_times[k]