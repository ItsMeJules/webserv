import socket
import requests
import sys

# ip = input("Entrez une adresse: ").split(':')
host = "127.0.0.1"
port = 9999
url = "http://" + host + ":" + str(port)

def gen():
    yield "HOOOOOOOOOOOOOOO".encode()
    yield "HELLO THERE HOW ARE YOU".encode()
    yield "THIS TEXT IS NONSENSE".encode()
    yield "I USE IT ONLY FOR TESTING".encode()

pload = {'username':'Olivia','password':'123'}
r = ""
if (sys.argv[1] == "chunked"):
    r = requests.post(url, data=gen())
else:
    r = requests.post(url, data = pload)
print(r.text)