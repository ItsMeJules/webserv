import socket
import requests

# ip = input("Entrez une adresse: ").split(':')
host = "127.0.0.1"
port = 9999

pload = {'username':'Olivia','password':'123'}
# rg = requests.get(host + ":" + str(port))
r = requests.post("http://" + host + ":" + str(port), data = pload)
print(r.text)