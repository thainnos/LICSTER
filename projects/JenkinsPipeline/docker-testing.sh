#!/bin/bash
# This script should only be used for linux based systems

function is_docker_installed {
    if [ $(which docker) ]; then
        # Assign stdin to keyboard to be able to read user input below
        exec < /dev/tty

        while true; do
            read -p "Do you want to continue with testing? (yes/no)`echo $'\n> '`" yn
            case $yn in
                [Yy]* ) echo "Starting the tests"; break;;
                [Nn]* ) cd ../..; exit 0;;
                * ) echo "Please answer yes or no.";;
            esac
        done
    else
        echo "Please install docker to use this script."
        echo "A script for installing docker on linux based systems can be found at:"
        echo "https://github.com/docker/docker-install"
        cd ../..
        exit 0
    fi
}

function is_user_in_docker_group {
    id -nG "$USER" | grep -qw "docker"
    if ! [ $? -eq 0 ]; then
        # User is in docker group -> can use docker without sudo commands
        echo "Please add yourself to the docker group. This is needed
        to execute docker commands in a script."
        cd ../..
        exit        
    fi

}

function docker_pytest {
    echo "Starting pytest Unit Tests..."
    # Build docker image:
    echo "Building docker container..."
    docker build -f local_testing_Dockerfiles/Pytest_Python.build ../../../ -t pytest &> /dev/null
    # Run tests in docker container with host STDOUT&STDERR attached
    echo "Running Unit Tests..."
    docker container run -a STDOUT -a STDERR pytest &> pytest.results # the testing part
    if [ $? -eq 0 ]
    then
        echo "All pytest Unit Tests ran successfully."
        rm pytest.results
    else
        echo "A pytest Unit Test failed. Details can be found in the file pytest.results"
    fi
    # Remove docker containers
    echo "Removing docker container..."
    docker container rm -f pytest &> /dev/null
    # Remove docker images
    docker image rm -f pytest &> /dev/null
}

function docker_python_3_7 {
    echo "Starting flake8 and bandit tests..."
    # Build docker image
    echo "Building docker container..."
    docker build -f local_testing_Dockerfiles/Python_3_7.build ../../../ -t python37 &> /dev/null
    # Run first test in docker container with host STDOUT&STDERR attached
    echo "Running flake8 tests..."
    docker container run -a STDOUT -a STDERR python37 flake8 . &> flake8.results
    if [ $? -eq 0 ]
    then
        echo "All flake8 tests ran successfully."
        rm flake8.results
    else
        echo "A flake8 test failed. Details can be found in the file flake8.results"
    fi
    # Run second test in docker container with host STDOUT&STDERR attached
    echo "Running bandit tests..."
    docker container run -a STDOUT -a STDERR python37 bandit -r . -x ./software/tests &> bandit.results
    if [ $? -eq 0 ]
    then
        echo "All bandit tests ran successfully."
        rm bandit.results
    else
        echo "A bandit test failed. Details can be found in the file bandit.results"
    fi    # Remove docker containers
    echo "Removing docker container..."
    docker container rm -f python37 &> /dev/null
    # Remove docker images
    docker image rm -f python37 &> /dev/null
}

cd projects/JenkinsPipeline
is_docker_installed
is_user_in_docker_group
docker_pytest
docker_python_3_7
cd ../..

# Build, run and check the status return code of the containers


# Flawfinder oder RATS, je nachdem welches in Jenkins funktioniert


# Check status code:


# Get and print feedback

# TODO: Testing in docker container:
# 1. Create docker container
# 2. move testing folders from host to docker container (docker cp?)
# 3. run tests in the docker container, redirect sdout into a file, 
#       remember return value
# 4. give return value to host
#       if return value == 0: print success message and go to 7. CLEANUP
#       else:
# 5. move the file from docker container to host (docker cp?)
# 6. print the file (and error message)
# 7. CLEANUP:
# 8. Delete the file
# 9. Remove the docker container
# 10. Exit the program

#while true; do
#    read -p "Do you wish to install this program?" yn
#    case $yn in
#        [Yy]* ) make install; break;;
#        [Nn]* ) exit;;
#        * ) echo "Please answer yes or no.";;
#    esac
#done
