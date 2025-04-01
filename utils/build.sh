#!/bin/bash

if [ ! -d "build" ]; then
    mkdir build
fi
gcc main.c -o build/datj_todolist_app -std=c99 -Wpedantic -Wall -Wextra -Waggregate-return -Wbad-function-cast -Wcast-align -Wcast-qual -Wdeclaration-after-statement -Wfloat-equal -Wformat=2 -Wlogical-op -Wmissing-declarations -Wmissing-declarations -Wmissing-prototypes -Wmissing-include-dirs -Wnested-externs -Wpointer-arith -Wredundant-decls -Wsequence-point -Wshadow -Wstrict-prototypes -Wswitch -Wundef -Wunreachable-code -Wunused-but-set-parameter -Wwrite-strings