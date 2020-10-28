#!/bin/sh
python3 -m pytest -q devices/hmi/software
if [ $? -eq 0 ]; then
    # tests passed, proceed to prepare commit message
    echo "All tests passed :)"
    exit 0
else
    # some tests failed, prevent from committing broken code on master
    echo "Some tests failed."
    exit 1
fi
