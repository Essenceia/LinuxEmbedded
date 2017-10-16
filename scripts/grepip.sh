#!/bin/bash
echo "Get ip from wifi"
sudo ifconfig wlan0 | grep 'inet '| awk '{print $2}'
