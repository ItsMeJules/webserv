#!/bin/bash

docker build -t webserv .
docker run --name webserv -td --network="host" webserv
