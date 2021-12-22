#!/bin/bash

clang $1.c -Ofast -Werror -Wall -lssl -lcrypto && time ./a.out
