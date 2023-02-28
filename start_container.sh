#!/bin/bash

docker build -t webserv .
docker run --rm --name webserv -td --network="host" webserv