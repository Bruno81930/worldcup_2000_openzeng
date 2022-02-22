.SUFFIXES: .cxx $(SUFFIXES)

CXX		= g++
CXXINCLUDE	= 

LINK_Dynamic	= 
LINK_Static	= -static

CXXLIBS		= $(LINK_Dynamic) -lm

#CXXFLAGS	= -pipe -Dsocklen_t=int
CXXFLAGS	= -pipe 

SRCS		= Angle.cxx CoachSee.cxx Connection.cxx Double.cxx Integer.cxx Log.cxx Mutex.cxx PlayerClient.cxx PlayerHear.cxx PlayerJunction.cxx PlayerObserver.cxx PlayerSee.cxx PlayerSenseBody.cxx Premise.cxx Random.cxx SayBuffer.cxx Screen.cxx Server.cxx Synchronizer.cxx Vector.cxx White.cxx Window.cxx WindowsScreen.cxx  WindowsWindow.cxx basic.cxx
OBJS		= Angle.o CoachSee.o Connection.o Double.o Integer.o Log.o Mutex.o PlayerClient.o PlayerHear.o PlayerJunction.o PlayerObserver.o PlayerSee.o PlayerSenseBody.o Premise.o Random.o SayBuffer.o Screen.o Server.o Synchronizer.o Vector.o White.o Window.o WindowsScreen.o  WindowsWindow.o basic.o Config.o Green.o Gullwing.o main.o

TARGET		= gullwing

$(TARGET):	$(OBJS)
	$(CXX) -o $@ $(OBJS) $(CXXLIBS)

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(CXXINCLUDE) $<

Config.o:	green/Config.cxx
	$(CXX) -c $(CXXFLAGS) $(CXXINCLUDE) green/Config.cxx

Green.o:	green/Green.cxx
	$(CXX) -c $(CXXFLAGS) $(CXXINCLUDE) green/Green.cxx

Gullwing.o:	green/Gullwing.cxx
	$(CXX) -c $(CXXFLAGS) $(CXXINCLUDE) green/Gullwing.cxx


clean:
	rm -f $(TARGET) $(OBJS) *core *bak *~

depend:
	makedepend $(CXXINCLUDE) $(SRCS)

