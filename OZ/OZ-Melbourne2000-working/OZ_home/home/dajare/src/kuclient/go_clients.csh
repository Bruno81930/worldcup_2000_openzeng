#! /bin/csh

set SERVER_HOST	= ($1)
set TEAMNAME	= KU-Sakura2
set GOALIE	= goalie/sample
set CMDLINE	= client/sample

$GOALIE  $TEAMNAME g $SERVER_HOST &
sleep 1

$CMDLINE $TEAMNAME f $SERVER_HOST & 
sleep 1

$CMDLINE $TEAMNAME f $SERVER_HOST &
sleep 1

$CMDLINE $TEAMNAME f $SERVER_HOST &
sleep 1

$CMDLINE $TEAMNAME f $SERVER_HOST &
sleep 1

$CMDLINE $TEAMNAME f $SERVER_HOST &
sleep 1

$CMDLINE $TEAMNAME f $SERVER_HOST &
sleep 1

$CMDLINE $TEAMNAME f $SERVER_HOST &
sleep 1

$CMDLINE $TEAMNAME f $SERVER_HOST &
sleep 1

$CMDLINE $TEAMNAME f $SERVER_HOST &
sleep 1

$CMDLINE $TEAMNAME f $SERVER_HOST &

