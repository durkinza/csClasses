"""
Create a proof-of-concept secure chat application in Python.

    Create function: EncryptAES the function will receive a key and plaintext as arguments and will return the corresponding ciphertext. (1 point)
    Create function: DecryptAES the function will receive a key and a ciphertext as arguments and return the corresponding plaintext. (1 point)
    Create function: EncryptRSA the function will receive an RSA public key and an AES key as arguments and return the encrypted key. (1 point)
    Create function: DecryptRSA the function will receive an RSA private key and an AES key as arguments and return the encrypted key. (1 point)
    Create function: HashSHA the function will receive an arbitrary size string and create a digest. (1 point)
    Create function: SignRSA the function will receive a private key and a digest and will sign the digest. (1 point)
    Create function: VerifySignatureRSA the function will receive an RSA public key and a signature and verify the signature. (1 point)
    Create function: ProtectHMAC the function will receive an AES key and a string of arbitrary size and return the HMAC of the message (1 point)

For the creation of these functions, the students may use existing Python libraries. Additional functions may be created as needed.

Use Python's Twisted module to create a server and a client application component. The client will connect to the server and send messages. The server will send the message back (echo). All chat messages should be encrypted with AES and protected via HMAC while in transit. (7 points)

Extend the scenario above to have the server support multiple clients. When a client sends a message the server will forward the message to all clients connected. Again, all chat messages should be encrypted and protected. (5 points)

Use docstrings for comments. For each function generate documentation in Shpinx.
"""
import sys
from Crypto.Cipher import AES, PKCS1_OAEP
from Crypto.PublicKey import RSA
from Crypto.Hash import SHA256, HMAC
from Crypto.Signature import PKCS1_v1_5
from Crypto import Random
from twisted.internet.protocol import Factory, ClientFactory
from twisted.protocols.basic import LineReceiver
from twisted.internet import reactor
from twisted.internet.error import ReactorNotRunning
from time import sleep
import base64

# general vars
random_seed = Random.new().read
message_split = '='*100


# Debugging output level
# 0 = no debug
# 1 = minor debug
# ...
# 5 = max debug
DEBUG_LEVEL = 1

# vars for AES
block_size = AES.block_size


def generateRSAKeys():
	"""
	Generate a new set of RSA Keys

	:returns: privKey, pubKey
	"""
	# vars for RSA
	RSAKeys = RSA.generate(4096, random_seed) #generate pub and priv RSA keys
	pubKey = getPubKey(RSAKeys)
	return RSAKeys, pubKey	


def EncryptAES(key, plainText):
	"""
	Encrypt plainText with AES

	:param key: encryption key
			must be of size 16, 24, 32
	:param plainText: plain text message
			The message to be encrypted (may be padded)

	:returns: base64 encoded cipher text
	"""
	key = HashSHA(key).digest()
	message = pad(plainText)
	cipher = AES.new(key)
	return base64.b64encode( cipher.encrypt( message ) )


def DecryptAES(key, cipherText):
	"""
	Decrypt cipherText with AES

	:param key: encryption key
			must be of size 16, 24, 32
	:param cipherText: encrypted message
			The message to be decrypted

	:returns: plaintext (bytes)
	"""
	key = HashSHA(key).digest()
	cipher = AES.new(key)
	return unpad( cipher.decrypt( base64.b64decode( cipherText ) ) )


def HashSHA(plainText):
	"""
	Get sha256 digest of plaintext

	:param plainText: The message to be hashed.
	
	:returns: Hash object
	"""
	try:
		plainText = plainText.encode('utf-8')
	except AttributeError:
		pass
	return SHA256.new(plainText)


def EncryptRSA(key, plainText):
	"""
	Encrypt plainText with given RSA key

	:param key: The RSA key to use to encrypt (public key).
	:param plainText: The message to be encrypted.

	:returns: base64 encoded cipher text
	"""
	try:
		plainText = plainText.encode()
	except AttributeError:
		pass
	encryptor = PKCS1_OAEP.new(key)
	cipherText = encryptor.encrypt(plainText)
	encodedCipherText = base64.b64encode(cipherText)
	return encodedCipherText


def DecryptRSA(key, encodedCipherText):
	"""
	Decrypt cipherText with given RSA key

	:param key: The RSA key to use to decrypt (private key).
	:param encodedCipherText : The base64 encoded cipher text to be decrypted.

	:returns: plaintext (bytes)
	"""
	encryptor = PKCS1_OAEP.new(key)
	cipherText = base64.b64decode(encodedCipherText)
	plainText = encryptor.decrypt(cipherText)
	return plainText	


def SignRSA(privKey, digest): 
	"""
	Sign the given digest with the private key

	:param privKey: The RSA private key to sign with.
	:param digest: The message digest to be signed.

	:returns: base64 encoded signature
	"""
	signer = PKCS1_v1_5.new( privKey )
	return base64.b64encode( signer.sign( digest ) )


def VerifySignatureRSA(pubKey, digest, signature):
	"""
	Verify the given signature with the given digest and public key

	:param pubkey: The RSA public key to verify against.
	:param digest: The digest to verify against. 
	:param signature: The signature to verify.

	:returns: Boolean
	"""
	signer = PKCS1_v1_5.new( pubKey )
	return signer.verify( digest, base64.b64decode( signature ) )


def ProtectHMAC(key, message):
	"""
	Generate an HMAC for the given message

	:param key: The AES key to use.
	:param message: The Message to be hashed.

	:returns: base64 encoded HMAC
	"""
	try:
		key = key.encode('utf-8')
	except AttributeError:
		pass
	mac = HMAC.new(key, message, SHA256.new())
	return base64.b64encode(mac.digest())

##############################
# Aditional funcitons
##############################
# padding functions
pad  =  lambda string: string + (block_size - len(string) % block_size) * chr(block_size - len(string) % block_size) 
unpad = lambda string : string[:-ord(string[len(string)-1:])]


def importKey(externKey):
	"""
	Imports a given RSA key
	
	:param externKey: The key to import

	:returns: RSA key object
	"""
	return RSA.importKey(externKey)


def getPubKey(privKey):
	"""
	Get public key from private key object

	:param privKey: The private key object

	:returns: RSA key object (public only) 
	"""
	return privKey.publickey()








#######################################################################
#
#
# SERVER
#
#
#######################################################################














class ServerWorker(LineReceiver):
	"""
	This class will act as a point of contact for a single client.
	"""
	def __init__(self, factory, id):
		self.factory = factory
		self.client_id = id
		self.AESKey = random_seed(256)

	def debug(self, message, level=1):
		if level <= DEBUG_LEVEL:
			self.log(message)

	def log(self, message):
		"""
		Log message to console

		:param message: The message to log
		"""
		try:
			message = message.decode()
		except AttributeError:
			pass
		print("(Server "+str(self.client_id)+"): "+message)
		sys.stdout.flush()

	def send(self, message):
		self.sendLine(message)

	def connectionMade(self):
		"""
		Automatically ran when client connects
		"""
		self.log("Connection made to client")
		self.factory.servers.append(self)
		self.send(b"SPK:"+self.factory.RSAPubKey.exportKey("PEM"))

	def lineReceived(self, data):	
		"""
		Automatically ran when a message is recieved
		
		:param data: (bytes) the data recieved
		"""
		# decode data
		data = data.decode("utf-8")
		self.debug("client "+str(self.client_id)+" said:"+data, 5)
		if data.startswith("CPK:"):
			self.log("Recieved Client Public Key")
			self.debug("Client Public Key is:"+data[4:], 3)
			self.clientPubKey = importKey(data[4:])

			# After a public key is recieved, send an AES key to use from now on
			#self.log(b"Using AES KEY:"+self.AESKey)
			AEScipherText = EncryptRSA(self.clientPubKey, self.AESKey)
			AESSignature = SignRSA(self.factory.RSAPrivKey, HashSHA( AEScipherText ) )
			self.send(b"AES:"+AEScipherText+b":::"+AESSignature)
		else:
			data = DecryptAES(self.AESKey, data.encode()).decode("utf-8")
			self.log("forwarding:"+data)
			# if it's a general message, repeat it.
			for echoer in self.factory.servers:
				d = EncryptAES(echoer.AESKey, data)
				echoer.sendLine(d)

	def connectionLost(self, reason):
		"""
		Automatically ran when a client disconnects
		
		:param reason: The reason for the disconnect
		"""
		self.log("Connection lost")
		self.factory.servers.remove(self)


class ServerFactory(Factory):
	"""
	This class will listen for new client connections
	For each connection, a ServerWorker will be spawned
	"""
	def __init__(self):
		self.servers = []
		self.RSAPrivKey, self.RSAPubKey = generateRSAKeys()

	def getPublicKey(self):
		return self.RSAPubKey.exportKey("PEM")

	def buildProtocol(self, addr):
		"""
		Build the worker for the connection

		:param addr: The address the connection is made on
		"""
		return ServerWorker(self, len(self.servers)+1)











#######################################################################
#
#
# CLIENT
#
#
#######################################################################





class ClientWorker(LineReceiver):
	"""
	This class will act as a client to talk with the server
	"""

	def __init__(self, factory, id, pubKey):
		self.factory = factory
		self.id = id
		self.RSAPrivKey, self.RSAPubKey = generateRSAKeys()
		self.serverPublicKey = pubKey
		self.serverAESKey = ''

	def debug(self, message, level=1):
		if level <= DEBUG_LEVEL:
			self.log(message)

	def log(self, message):
		"""
		Log message to console

		:param message: The message to log
		"""
		try:
			message = message.decode()
		except AttributeError:
			pass
		print("(Client "+str(self.id)+"): "+message)
		sys.stdout.flush()

	def send(self, message):
		self.sendLine(message)

	def makeConnection(self, transport):
		"""
		Automatically to connect to server

		:param transport: The connection to the server
		"""
		self.transport = transport
		self.factory.clients.append(self)
		self.log("Connection made")
		self.send(b"CPK:"+self.RSAPubKey.exportKey("PEM"))


	def connectionMade(self):
		"""
		Automatically ran when a connection to the server is made
		"""
		pass


	def lineReceived(self, data):
		"""
		Automatically ran when a message is recieved from the server
		
		:param data: The message recieved from the server
		"""
		data = data.decode('utf-8')
		self.debug("server said:"+data, 5)
		if data.startswith("SPK:"):
			# if it's a public key, save it
			self.log("Recieved server public key")
			self.debug("server public key is:"+data[4:], 3)
			self.serverPubKey = importKey(data[4:])
		elif data.startswith("AES:"):
			# if it's an AES key, save it
			AESKey = data[4:].split(":::")
			AESKeyEncrypted = AESKey[0]
			self.debug("server AES key Encrypted is:"+AESKeyEncrypted, 4)
			AESSignature = AESKey[1]
			self.debug("server AES key Signature is:"+AESSignature, 3)
			# verify that the server sent the AES key.
			isValid = VerifySignatureRSA(self.serverPubKey, HashSHA(AESKeyEncrypted), AESSignature)
			self.debug("server AES key Signauture is valid? - "+str(isValid), 2)
			if isValid:
				# Decrypt the AES key
				AESKeyPlainText = DecryptRSA(self.RSAPrivKey, AESKeyEncrypted.encode())
				#self.debug("server AES key is:"+AESKeyPlainText.decode(), 3) ## Can't print AES key since it's a random set of bytes that don't print in utf-8
				self.AESKey = AESKeyPlainText

				message = "This is a test message from client"+str(self.id)
				self.log("Sending message to server: "+message)
				self.send(EncryptAES(self.AESKey, message))
			else:
				self.debug("Server AES Signature is invalid", 1)			
			
		else:
			# if we don't know it, it's probably a message, just print it out
			data = DecryptAES(self.AESKey, data.encode()).decode("utf-8")
			self.log(data)


			


class ClientFactory(ClientFactory):
	"""
	This class will generate new clients as requested
	Each client created will act independently as a unique client.
	"""
	def __init__(self, ServerPubKey):
		self.client_count = 0
		self.clients = []
		self.ServerPubKey = ServerPubKey

	def startedConnecting(self, connector):
		"""
		Automatically ran before a client connection is created.
		"""
		pass

	def buildProtocol(self, addr):
		"""
		For each new client requests, generate a new ClientWorker

		:param addr: The address to connect to
		"""
		self.client_count = self.client_count+1
		return ClientWorker(self, self.client_count, self.ServerPubKey)

	def clientConnectionFailed(self, connector, reason):
		"""
		Automatically ran if the connection failed to connect.
		
		:param connector: The failed connection
		:param reason: The reason the connection failed
		"""
		print("Connection failed - goodbye!")
		try:
			reactor.stop()
		except ReactorNotRunning:
			pass

	def clientConnectionLost(self, connector, reason):
		"""
		Automatically ran if the connection is lost.
		:param connector: The connection that failed
		:param reason: The reason the connectoin failed
		"""
		print("Connection lost - goodbye!")
		try:
			reactor.stop()
		except ReactorNotRunning:
			pass

if __name__ == "__main__":
	"""
	Default to a server instance.
	if an argument if given then create a client
	"""

	SF = ServerFactory()
	CF = ClientFactory(SF.getPublicKey)
	reactor.listenTCP(4321, SF)
	reactor.connectTCP('localhost', 4321, CF)
	reactor.connectTCP('localhost', 4321, CF)
	reactor.run()


#	RSAPrivKey, RSAPubKey = generateRSAKeys()
#	
#
#	AESKey = "This is the AES Key"
#
#	message = "This is a longer test message to encrypt for testing"
#
#	print(message_split)
#	print(message_split)
#	print("Message to encrypt is:")
#	print(message)
#	print(message_split)
#	print("Encrypting with AES key:")
#	print(AESKey)
#	print(message_split)
#	print("Encrypting with RSA private key:")
#	print(RSAPrivKey.exportKey('PEM').decode())
#	print(message_split)
#	print("Decrypting with RSA public key:")
#	print(RSAPubKey.exportKey('PEM').decode())
#	print(message_split)
#	print(message_split)
#	print('')
#	print('')
#	print('')
#
#
#	AESciphertext = EncryptAES(AESKey, message)
#	RSAciphertext = EncryptRSA(RSAPubKey, message)
#
#	#messageSignature = SignRSA(RSAPrivKey, HashSHA( message ) ) # messages with be ciphered before signing
#	#AESSignature = SignRSA(RSAPrivKey, HashSHA( AESciphertext ) ) # AES will use HMAC, not signatures
#	RSASignature = SignRSA(RSAPrivKey, HashSHA( RSAciphertext ) )
#	AESHMAC = ProtectHMAC(AESKey, AESciphertext)
#
#
#	print(message_split)
#	print(message_split)
#	print("AES cipher text:")
#	print(AESciphertext.decode())
#	print(message_split)
#	print("RSA cipher text:")
#	print(RSAciphertext.decode())
#	print(message_split)
#	print(message_split)
#	#print("message signature: "+messageSignature)
#	#print("AES signature: "+AESSignature)
#	print("RSA signature:")
#	print(RSASignature.decode())
#	print(message_split)
#	print("AES HMAC:")
#	print(AESHMAC.decode())
#	print(message_split)
#	print(message_split)
#	print('')
#	print('')
#	print('')
#
#
#	AESplaintext = DecryptAES(AESKey, AESciphertext)
#	RSAplaintext = DecryptRSA(RSAPrivKey, RSAciphertext)
#
#	#messageVerify = VerifySignatureRSA(RSAPrivKey, HashSHA(message), messageSignature)
#	#AESVerify = VerifySignatureRSA(RSAPrivKey, HashSHA(AESciphertext), AESSignature)
#	RSAVerify = VerifySignatureRSA(RSAPrivKey, HashSHA(RSAciphertext), RSASignature)
#	HMACVerify = True if ProtectHMAC(AESKey, AESciphertext) == AESHMAC else False
#
#
#	print(message_split)
#	print(message_split)
#	print("Verifying signatures:")
#	#print("message is valid: " + str(messageVerify))
#	#print("AES is valid: " + str(AESVerify))
#	print("RSA is valid: " + str(RSAVerify))
#	print("HMAC is valid: " + str(HMACVerify))
#	print(message_split)
#	print(message_split)
#	print("AES plain text:")
#	print(AESplaintext.decode())
#	print(message_split)
#	print("RSA plain text:")
#	print(RSAplaintext.decode())
#	print(message_split)
#	print(message_split)
#
