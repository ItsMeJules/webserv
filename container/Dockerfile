FROM debian

RUN apt-get update && apt-get install build-essential make -y

COPY html/ html/
COPY conf/* conf/
	
COPY start_webserv.sh /