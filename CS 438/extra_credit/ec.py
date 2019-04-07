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

from Crypto.Cipher import AES, PKCS1_OAEP
from Crypto.PublicKey import RSA
from Crypto.Hash import SHA256, HMAC
from Crypto.Signature import PKCS1_v1_5
from Crypto import Random
import base64

# general vars
random_generator = Random.new().read
message_split = '='*100

# vars for AES
block_size = AES.block_size


def generateRSAKeys():
	"""
	Generate a new set of RSA Keys

	:returns: privKey, pubKey
	"""
	# vars for RSA
	RSAKeys = RSA.generate(1024, random_generator) #generate pub and priv RSA keys
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
	encryptor = PKCS1_OAEP.new(key)
	cipherText = encryptor.encrypt(plainText.encode())
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


if __name__ == "__main__":

	RSAPrivKey, RSAPubKey = generateRSAKeys()

	AESKey = "This is the AES Key"

	message = "This is a longer test message to encrypt for testing"

	print(message_split)
	print(message_split)
	print("Message to encrypt is:")
	print(message)
	print(message_split)
	print("Encrypting with AES key:")
	print(AESKey)
	print(message_split)
	print("Encrypting with RSA private key:")
	print(RSAPrivKey.exportKey('PEM').decode())
	print(message_split)
	print("Decrypting with RSA public key:")
	print(RSAPubKey.exportKey('PEM').decode())
	print(message_split)
	print(message_split)
	print('')
	print('')
	print('')


	AESciphertext = EncryptAES(AESKey, message)
	RSAciphertext = EncryptRSA(RSAPubKey, message)

	#messageSignature = SignRSA(RSAPrivKey, HashSHA( message ) ) # messages with be ciphered before signing
	#AESSignature = SignRSA(RSAPrivKey, HashSHA( AESciphertext ) ) # AES will use HMAC, not signatures
	RSASignature = SignRSA(RSAPrivKey, HashSHA( RSAciphertext ) )
	AESHMAC = ProtectHMAC(AESKey, AESciphertext)


	print(message_split)
	print(message_split)
	print("AES cipher text:")
	print(AESciphertext.decode())
	print(message_split)
	print("RSA cipher text:")
	print(RSAciphertext.decode())
	print(message_split)
	print(message_split)
	#print("message signature: "+messageSignature)
	#print("AES signature: "+AESSignature)
	print("RSA signature:")
	print(RSASignature.decode())
	print(message_split)
	print("AES HMAC:")
	print(AESHMAC.decode())
	print(message_split)
	print(message_split)
	print('')
	print('')
	print('')


	AESplaintext = DecryptAES(AESKey, AESciphertext)
	RSAplaintext = DecryptRSA(RSAPrivKey, RSAciphertext)

	#messageVerify = VerifySignatureRSA(RSAPrivKey, HashSHA(message), messageSignature)
	#AESVerify = VerifySignatureRSA(RSAPrivKey, HashSHA(AESciphertext), AESSignature)
	RSAVerify = VerifySignatureRSA(RSAPrivKey, HashSHA(RSAciphertext), RSASignature)
	HMACVerify = True if ProtectHMAC(AESKey, AESciphertext) == AESHMAC else False


	print(message_split)
	print(message_split)
	print("Verifying signatures:")
	#print("message is valid: " + str(messageVerify))
	#print("AES is valid: " + str(AESVerify))
	print("RSA is valid: " + str(RSAVerify))
	print("HMAC is valid: " + str(HMACVerify))
	print(message_split)
	print(message_split)
	print("AES plain text:")
	print(AESplaintext.decode())
	print(message_split)
	print("RSA plain text:")
	print(RSAplaintext.decode())
	print(message_split)
	print(message_split)

