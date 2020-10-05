#!/bin/bash

# Create a network bridge and persistent data storage for data and
# TLS certificates

docker network create jenkins
docker volume create jenkins-docker-certs
docker volume create jenkins-data

# Download and start the Docker:Dind image zu execute commands 
# in nodes/agents

docker container run --name jenkins-docker --rm --detach \
--privileged --network jenkins --network-alias docker \
--env DOCKER_TLS_CERTDIR=/certs \
--volume jenkins-docker-certs:/certs/client \
--volume jenkins-data:/var/jenkins_home \
--publish 2376:2376 docker:dind 

# Download and start the jenkins blueocean image

docker container run --name jenkins-blueocean --rm --detach \
--network jenkins --env DOCKER_HOST=tcp://docker:2376 \
--env DOCKER_CERT_PATH=/certs/client --env DOCKER_TLS_VERIFY=1 \
--volume jenkins-data:/var/jenkins_home \
--volume jenkins-docker-certs:/certs/client:ro \
--publish 8080:8080 --publish 50000:50000 jenkinsci/blueocean

# Print the docker initialisation Command
echo "The setup will take approximately 20 more seconds."
sleep 20 # It takes time for docker to write the password to the file
echo "Copy the following line, visit http://localhost:8080 and paste it into the field"
sudo docker exec jenkins-blueocean cat /var/jenkins_home/secrets/initialAdminPassword