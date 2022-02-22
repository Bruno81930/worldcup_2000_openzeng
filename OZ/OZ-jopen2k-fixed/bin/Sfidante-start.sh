#!/bin/sh

index=$#
program=./Sfidante
teamname="Sfidante"
server="localhost"
port=6000

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

$program $teamname nongoalie $server $port &
sleep 1
$program $teamname nongoalie $server $port &
sleep 1
$program $teamname nongoalie $server $port &
sleep 1
$program $teamname nongoalie $server $port &
sleep 1
$program $teamname nongoalie $server $port &
sleep 1
$program $teamname nongoalie $server $port &
sleep 1
$program $teamname nongoalie $server $port &
sleep 1
$program $teamname nongoalie $server $port &
sleep 1
$program $teamname nongoalie $server $port &
sleep 1
$program $teamname nongoalie $server $port &
sleep 1
$program $teamname goalie    $server $port &
sleep 1
