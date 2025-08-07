#!/bin/sh -x

sudo apt install nginx
sudo systemctl stop nginx
sudo systemctl disable nginx
