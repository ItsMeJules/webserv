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

files = {'upload_file': open('client.py','rb')}
values = {'DB': 'photcat', 'OUT': 'csv', 'SHORT': 'short'}

if (len(sys.argv) > 1 and sys.argv[1] == "chunked"):
    r = requests.post(url, data=gen())
elif (len(sys.argv) > 1 and sys.argv[1] == "upload"):
    r = requests.post(url, files=files, data=values)
else:
    r = requests.post(url, data = pload)
print(r.text)