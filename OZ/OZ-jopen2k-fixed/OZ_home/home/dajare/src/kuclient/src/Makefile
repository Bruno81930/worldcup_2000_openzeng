.SUFFIXES: .C $(SUFFIXES)

ARCHIVE		= ../lib/libsclient.a
CXX			= gcc
CXXINCLUDE	= 

CXXLIBS_default	= -lm
CXXLIBS_SunOS_4	= -lm
CXXLIBS_SunOS_5	= -lm -L/usr/ucblib -lucb -lsocket -lnsl
CXXLIBS_OSF1	= -lm
CXXLIBS_NEWS	= -lm
CXXLIBS_IRIX	= -lm
CXXLIBS_Linux	= -lm -lbsd
CXXLIBS		= $(LINK____LINK_STYLE___) $(CXXLIBS_Linux)

OSFLAGS_default	= 
OSFLAGS_SunOS_4	= -DSunOS4
OSFLAGS_SunOS_5	= -DSolaris -DMEMTEST
OSFLAGS_OSF1	= -DOSF1
OSFLAGS_NEWS	= -DNewsOS
OSFLAGS_IRIX	= -DIRIX -D_BSD_SIGNALS
OSFLAGS_Linux	= -DLinux
CXXFLAGS	= -g $(OSFLAGS_Linux)

SRCS		= netif.c basic_com.c sensor_info.c position.c playmode.c
OBJS		= $(SRCS:.c=.o)

.c.o:
	$(CXX) -c $(CXXFLAGS) $(CXXINCLUDE) $<

all:	$(ARCHIVE)

$(ARCHIVE):	$(OBJS)
	ar ruv $(ARCHIVE) $(OBJS)
	ranlib $(ARCHIVE)

clean:
	rm -f $(ARCHIVE) $(OBJS) *core *bak *~

depend:
	makedepend $(CXXINCLUDE) $(SRCS)

