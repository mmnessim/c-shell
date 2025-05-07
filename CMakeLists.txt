cmake_minimum_required(VERSION 3.10)
project(c-shell)
add_executable(c-shell main.c utils.c)
add_executable(c-shell-test test.c utils.c)
