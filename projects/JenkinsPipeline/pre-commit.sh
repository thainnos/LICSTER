#!/bin/sh

projects/JenkinsPipeline/docker-testing.sh
if [ $? -eq 0 ]; then
    # tests passed, proceed to prepare commit message
    exit 0
else
    # some tests failed, prevent from committing broken code on master
    echo "The commit was aborted as some tests failed."
    exit 1
fi
