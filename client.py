import socket
import requests

# ip = input("Entrez une adresse: ").split(':')
host = "127.0.0.1"
port = 9999

def gen():
    yield "HOOOOOOOOOOO"
    yield "HOOOOOOOOOOOOOOO".encode()
    yield "HELLO THERE HOW ARE YOU".encode()
    yield "THIS TEXT IS NONSENSE".encode()
    yield "I USE IT ONLY FOR TESTING".encode()

pload = {'username':'Olivia','password':'123'}
# rg = requests.get(host + ":" + str(port))
r = requests.post("http://" + host + ":" + str(port), data=gen())
print(r.text)