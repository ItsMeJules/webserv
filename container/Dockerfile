FROM debian

EXPOSE 8080

RUN apt-get update && apt-get install build-essential make php-cgi python3 -y

COPY html/ html/
COPY conf/* conf/

COPY start_webserv.sh /

# ENTRYPOINT [ "./start_webserv.sh" ]
