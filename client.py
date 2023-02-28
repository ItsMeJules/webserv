import socket
import requests
from requests import Session, Request
import sys
from requests_toolbelt import MultipartEncoder

# ip = input("Entrez une adresse: ").split(':')
host = "127.0.0.1"
port = 8080
url = "http://" + host + ":" + str(port)

def gen():
    yield "HOOOOOOOOOOOOOOO".encode()
    yield "HELLO THERE HOW ARE YOU".encode()
    yield "THIS TEXT IS NONSENSE".encode()
    yield "I USE IT ONLY FOR TESTING".encode()

pload = {'username':'Olivia','password':'123'}
headers = {'Accept': '*/*',
            'Accept-Encoding': 'gzip, deflate',
            'Connection': 'keep-alive',
            'Content-Type': 'application/x-www-form-urlencoded',
            'Host': '127.0.0.1:9999',
            'Transfer-Encoding': '',
            'User-Agent': 'python-requests/2.25.1'}
r = ""

values = {'DB': 'photcat', 'OUT': 'csv', 'SHORT': 'short'}

m = MultipartEncoder(
  fields = {
    'DB': 'photcat', 'OUT': 'csv', 'SHORT': 'short'
  }
)

def gen_file():
  a = m.read(4096)
  while a:
    yield a
    a = m.read(4096)

if (len(sys.argv) > 1 and sys.argv[1] == "chunked"):
    r = requests.post(url, data=gen())
elif (len(sys.argv) > 1 and sys.argv[1] == "upload"):
    r = requests.post(url, files=files, data=values)
elif (len(sys.argv) > 1 and sys.argv[1] == "c_upload"):
    r = requests.post(url, data=gen_file(), headers={'Content-Type': m.content_type})
else:
    s = Session()
    req = Request('POST', url, data=pload)
    prepped = req.prepare()
    # prepped.headers['Content-Length'] = 30
    r = s.send(prepped)
print(r.text)
