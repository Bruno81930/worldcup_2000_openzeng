.SUFFIXES: .C $(SUFFIXES)

CXX		= gcc
CXXINCLUDE	= -I../include

CXXLIBS_default	= -L../lib -lsclient -lm
CXXLIBS_SunOS_4	= -L../lib -lsclient -lm
#CXXLIBS_SunOS_5	= -L/usr/ucblib -L../lib -lsclient -lucb -lsocket -lnsl -lm
CXXLIBS_SunOS_5	= -L../lib -lsclient -lsocket -lnsl -lm
CXXLIBS_OSF1	= -L../lib -lsclient -lm
CXXLIBS_NEWS	= -L../lib -lsclient -lm
CXXLIBS_IRIX	= -L../lib -lsclient -lm
CXXLIBS_Linux	= -lbsd -L../lib -lsclient -lm
CXXLIBS		= $(LINK____LINK_STYLE___) $(CXXLIBS_Linux)

OSFLAGS_default	= 
OSFLAGS_SunOS_4	= -DSunOS4
OSFLAGS_SunOS_5	= -DSolaris -DMEMTEST
OSFLAGS_OSF1	= -DOSF1
OSFLAGS_NEWS	= -DNewsOS
OSFLAGS_IRIX	= -DIRIX -D_BSD_SIGNALS
OSFLAGS_Linux	= -DLinux
CXXFLAGS	= -g $(OSFLAGS_Linux)

SRCS		= client.c sendcom.c vector.c action.c
OBJS		= $(SRCS:.c=.o)

TARGET		= sample

$(TARGET):	$(OBJS)
	$(CXX) -o $@ $(OBJS) $(CXXLIBS)

.c.o:
	$(CXX) -c $(CXXFLAGS) $(CXXINCLUDE) $<

clean:
	rm -f $(TARGET) $(OBJS) *core *bak *~

depend:
	makedepend $(CXXINCLUDE) $(SRCS)

