#!/bin/sh

kill `ps ax | grep Gemini | grep -v grep | awk '{print $1}'`
