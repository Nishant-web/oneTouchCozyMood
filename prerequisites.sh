#!/bin/sh
cd
sudo apt update
sudo apt upgrade
sudo apt install libmpg123-dev
sudo apt install libao-dev
sudo git clone https://github.com/fltk/fltk.git
cd fltk
sudo git pull origin
sudo make install
cd ..
sudo git clone https://github.com/WiringPi/WiringPi.git
cd wiringPi
sudo git pull origin
sudo ./build

