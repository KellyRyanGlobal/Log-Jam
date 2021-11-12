#!/bin/bash

dind="$(docker container ls | grep "dind")"
jenkins="$(docker container ls | grep "myjenkins-blueocean")"

function login() {
    echo " logging into docker"
    docker login -u $DOCKER_USER -p $DOCKER_PASS
}

function main() {
  login
	echo "check namespace for network"
	if [[ -z $(docker network ls | grep jenkins) ]]; then
		echo "creating network"
		docker network create jenkins
	fi
	echo "set dind"
	if [[ -z "$dind" ]]; then
		echo "creating dind"
		docker run --name jenkins-docker --rm --detach --privileged --network jenkins --network-alias docker --env DOCKER_TLS_CERTDIR=/certs --volume jenkins-docker-certs:/certs/client --volume jenkins-data:/var/jenkins_home --publish 2376:2376  docker:dind --storage-driver overlay2
	fi
	echo " create jenkins"
	
	if [[ -z $jenkins ]]; then
		echo "creating jenkins server"
		docker run --name jenkins-blueocean --rm --detach --network jenkins --env DOCKER_HOST=tcp://docker:2376 --env DOCKER_CERT_PATH=/certs/client --env DOCKER_TLS_VERIFY=1 --publish 8080:8080 --publish 50000:50000 --volume jenkins-data:/var/jenkins_home --volume jenkins-docker-certs:/certs/client:ro jenkinsci/blueocean:latest
	else
		echo " Everything is up"
	fi
}

"$@main"
