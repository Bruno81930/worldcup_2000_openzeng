#! /bin/csh

if( -f kill.tmp ) then 
   \rm kill.tmp 
endif
#ps  | grep ./sample | grep -v grep | awk '{printf("kill -9 %d\n",$6)}' >>! kill.tmp
ps  | grep sample | grep -v grep | awk '{printf("kill -9 %d &\n",$1)}' >>! kill.tmp
source kill.tmp
\rm kill.tmp
