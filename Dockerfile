FROM debian

EXPOSE 8080

RUN apt-get update && apt-get install build-essential make -y

RUN mkdir -p webserv

COPY srcs/ webserv/srcs/
COPY headers/ webserv/headers/

COPY html/ html/
COPY conf/ conf/
COPY Makefile webserv/
	
WORKDIR /webserv

RUN make

ENTRYPOINT ["./webserv", "/conf/test.conf" ]