#!/bin/bash

clang $1.c -Ofast -Werror -Wall -Wshadow -fsanitize=address -g -lssl -lcrypto && ./a.out
