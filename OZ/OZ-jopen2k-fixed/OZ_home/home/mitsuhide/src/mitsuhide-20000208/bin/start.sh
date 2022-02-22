#!/bin/sh

index=$#
program=default
teamname="YowAI"
host="localhost"
port=6000
version=5.25

case $index in
2)
host=$1
teamname=$2
;;
1)
host=$1
;;
0)
;;
esac

mitsuhide         $teamname nongoalie $host $port $version &
