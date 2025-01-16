#!/bin/bash

cd build
cmake ..
make clean
make

cp client_module.cpython-312-x86_64-linux-gnu.so ../gui/client_module.cpython-312-x86_64-linux-gnu.so
