#!/bin/sh

if test -x /usr/ucb/ps ; then 
  kill `/usr/ucb/ps ax | grep mitsuhide | grep -v grep | awk '{print $1}'`
else
  kill `ps ax | grep mitsuhide | grep -v grep | awk '{print $1}'`
fi

