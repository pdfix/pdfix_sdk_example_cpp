#!/bin/bash

cmake -G Ninja --target example -B build -S .
cmake --build build --target example
