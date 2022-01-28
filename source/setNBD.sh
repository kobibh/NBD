#!/bin/bash
DEV_PATH=$1
echo "*** NBD Setup ***"
sudo modprobe nbd
sudo ./main $DEV_PATH
# sudo mkfs.ext4 $DEV_PATH