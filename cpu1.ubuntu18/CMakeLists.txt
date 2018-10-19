cmake_minimum_required(VERSION 3.12)
project(cpu1_ubuntu18)

set(CMAKE_CXX_STANDARD 14)

add_executable(cpu1_ubuntu18
        cpu.c
        dialog.c
        hand_in
        Makefile
        OSP.demo
        osp.o)
