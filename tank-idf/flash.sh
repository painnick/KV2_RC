#!/bin/bash
source env.sh
idf.py flash monitor -p /dev/ttyACM0
