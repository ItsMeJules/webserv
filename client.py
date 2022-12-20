import socket
import requests

# ip = input("Entrez une adresse: ").split(':')
host = "127.0.0.1"
port = 9999

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((host, port))
    str = input("Ecrivez quelquechose à envoyer : ")
    s.send(str.encode())
print("Connection ended.")