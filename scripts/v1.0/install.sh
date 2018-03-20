#!/bin/bash

apt-get update
apt-get upgrade
apt-get install mysql-server
apt-get install libmysqlclient-dev
apt-get install openvpn
apt-get install libsctp-dev
apt-get install openssl
add-apt-repository "ppa:patrickdk/general-lucid"
apt-get update
apt-get install iperf3
apt-get install iperf
apt-get install htop
