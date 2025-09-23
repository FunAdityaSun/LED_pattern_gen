# NetWorkBench

## Overview

This project implements a multi-mode LED patter generator. The system continuously drives an 8-LED array in different visual patterns, with the ability to switch between mode dynamically using user input.

## Project Setup (Linux)

### Pre-req

Install verilator.

```
sudo apt update
sudo apt install verilator -y
```

### Build and Run

```
verilator -Wall --cc led_scroll.v --exe test_scroll.c
make -C obj_dir -f Vled_scroll.mk Vled_scroll
./obj_dir/Vled_scroll
```
