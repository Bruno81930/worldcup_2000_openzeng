.SUFFIXES: .C $(SUFFIXES)

CXX		= g++ -O1
CXXINCLUDE	= -I../include

CXXLIBS_default	= -lm
CXXLIBS_SunOS_4	= -lm
CXXLIBS_SunOS_5	= -lm -lsocket -lnsl -R/usr/lib
CXXLIBS_OSF1	= -lm
CXXLIBS_NEWS	= -lm
CXXLIBS_IRIX	= -lm
CXXLIBS_Linux	= -lm

LINK_Dynamic	= 
LINK_Static	= -static

CXXLIBS		= $(LINK_Dynamic) $(CXXLIBS_Linux) -L../lib -lYowAI

OSFLAGS_default	= 
OSFLAGS_SunOS_4	= -DSunOS4
OSFLAGS_SunOS_5	= -DSolaris -DSYSV
OSFLAGS_OSF1	= -DOSF1
OSFLAGS_NEWS	= -DNewsOS
OSFLAGS_IRIX	= -DIRIX -DSYSV
OSFLAGS_Linux	= -DLinux
CXXFLAGS	= -pipe $(OSFLAGS_Linux)

SRCS		= myagent.C main.C
OBJS		= $(SRCS:.C=.o)

TARGET		= mitsuhide

$(TARGET):	$(OBJS)
	$(CXX) -o $@ $(OBJS) $(CXXLIBS)

.C.o:
	$(CXX) -c $(CXXFLAGS) $(CXXINCLUDE) $<

clean:
	rm -f $(TARGET) $(OBJS) *core *bak *~

depend:
	makedepend $(CXXINCLUDE) $(SRCS)





