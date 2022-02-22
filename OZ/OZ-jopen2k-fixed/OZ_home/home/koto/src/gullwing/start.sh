#!/bin/sh

index=$#
program=./gullwing
teamname="gullwing"
server="localhost"
port=6000
sleep=0

case $index in
2)
server=$1
teamname=$2
;;
1)
server=$1
;;
0)
;;
esac

$program $server $port $teamname &
sleep $sleep
$program $server $port $teamname &
sleep $sleep
$program $server $port $teamname &
sleep $sleep

$program $server $port $teamname &
sleep $sleep
$program $server $port $teamname &
sleep $sleep
$program $server $port $teamname &
sleep $sleep
$program $server $port $teamname &
sleep $sleep

$program $server $port $teamname &
sleep $sleep
$program $server $port $teamname &
sleep $sleep
$program $server $port $teamname &
sleep $sleep
$program $server $port $teamname &
sleep $sleep
