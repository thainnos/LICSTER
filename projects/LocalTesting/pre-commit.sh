#!/bin/bash
# This script should only be used for linux based systems

FAILURE_COUNTER=0

function is_docker_installed {
    if [ $(which docker) ]; then
        # Assign stdin to keyboard to be able to read user input below
        exec < /dev/tty

        while true; do
            read -p "Do you want to continue testing? (yes/no)`echo $'\n> '`" yn
            case $yn in
                [Yy]* ) echo -e "\nStarting tests"; break;;
                [Nn]* ) cd ../..; exit 0;;
                * ) echo "Please answer yes or no.";;
            esac
        done
    else
        echo "Please install docker to use this script."
        echo "A script for installing docker on linux based systems can be found at:"
        echo "https://github.com/docker/docker-install"
        cd ../..
        exit 1
    fi
}

function is_user_in_docker_group {
    id -nG "$USER" | grep -qw "docker"
    if ! [ $? -eq 0 ]; then
        # User is in docker group -> can use docker without sudo commands
        echo "Please add yourself to the docker group. This is needed
        to execute docker commands in a script."
        cd ../..
        exit 1       
    fi

}

function docker_pytest {
    echo "Starting pytest Unit Tests..."
    # Build docker image:
    echo "Building docker container..."
    docker build -f Dockerfiles/Pytest_Python.build ../../../ -t pytest &> /dev/null
    # Run tests in docker container with host STDOUT&STDERR attached
    echo "Running Unit Tests..."
    docker container run -a STDOUT -a STDERR pytest &> pytest.results # the testing part
    if [ $? -eq 0 ]
    then
        echo -e "All pytest Unit Tests ran successfully.\n"
        rm pytest.results
    else
        echo -e "A pytest Unit Test failed. Details can be found in the file pytest.results\n"
        FAILURE_COUNTER=$((FAILURE_COUNTER+1))
    fi
    # Remove docker containers
    echo -e "Removing docker container...\n"
    docker container rm -f pytest &> /dev/null
    # Remove docker images
    docker image rm -f pytest &> /dev/null
}

function docker_python_3_7 {
    echo "Starting flake8 and bandit tests..."
    # Build docker image
    echo "Building docker container..."
    docker build -f Dockerfiles/Python_3_7.build ../../../ -t python37 &> /dev/null
    # Run first test in docker container with host STDOUT&STDERR attached
    echo "Running flake8 tests..."
    docker container run -a STDOUT -a STDERR python37 flake8 . &> flake8.results
    if [ $? -eq 0 ]
    then
        echo -e "All flake8 tests ran successfully.\n"
        rm flake8.results
    else
        echo -e "A flake8 test failed. Details can be found in the file flake8.results\n"
        FAILURE_COUNTER=$((FAILURE_COUNTER+1))
    fi
    # Run second test in docker container with host STDOUT&STDERR attached
    echo "Running bandit tests..."
    docker container run -a STDOUT -a STDERR python37 bandit -r . -x ./software/tests &> bandit.results
    if [ $? -eq 0 ]
    then
        echo -e "All bandit tests ran successfully.\n"
        rm bandit.results
    else
        echo -e "A bandit test failed. Details can be found in the file bandit.results\n"
        FAILURE_COUNTER=$((FAILURE_COUNTER+1))
    fi
    echo -e "Removing docker container...\n\n"
    docker container rm -f python37 &> /dev/null
    # Remove docker images
    docker image rm -f python37 &> /dev/null
}

cd projects/LocalTesting
is_docker_installed
is_user_in_docker_group
docker_pytest
docker_python_3_7
cd ../..

if [ ${FAILURE_COUNTER} -eq 0 ]; then
    # tests passed, proceed to prepare commit message
    exit 0
else
    # some tests failed, prevent from committing broken code on master
    echo "The commit was aborted as some tests failed."
    exit 1
fi
