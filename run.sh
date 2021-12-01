#!/bin/bash

clang $1.c -Ofast -Werror -Wall -fsanitize=address -g -lssl -lcrypto && ./a.out
