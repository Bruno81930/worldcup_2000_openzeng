# Microsoft Developer Studio Project File - Name="Gullwing" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** $BJT=8$7$J$$$G$/$@$5$$(B **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Gullwing - Win32 Debug
!MESSAGE $B$3$l$OM-8z$JR28L'2(BB $B$3$NL_[<^*8D$rK^YD(BNMAKE $B$r;HMQ$7$F$/$@$5$$!#(B
!MESSAGE [$BR28L'2L48=N_0D(B] $B:O]D(Bg$BMQ$7$F<B9T$7$F$/$@$5$$(B
!MESSAGE 
!MESSAGE NMAKE /f "Gullwing.mak".
!MESSAGE 
!MESSAGE NMAKE $B$N<B9T;~$K9=@.$r;XDj$G$-$^$9(B
!MESSAGE $B:O]DW2EO8[$N@_Dj$rDj5A$7$^$9!#Nc(B:
!MESSAGE 
!MESSAGE NMAKE /f "Gullwing.mak" CFG="Gullwing - Win32 Debug"
!MESSAGE 
!MESSAGE $BA*Br2DG=$JK^YDS0D^(B:
!MESSAGE 
!MESSAGE "Gullwing - Win32 Release" ("Win32 (x86) Application" $BMQ(B)
!MESSAGE "Gullwing - Win32 Debug" ("Win32 (x86) Application" $BMQ(B)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Gullwing", BAAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Gullwing - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /Za /W4 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D LEARNING=1 /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib winmm.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "Gullwing - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /Ze /W4 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D LEARNING=1 /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib winmm.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Gullwing - Win32 Release"
# Name "Gullwing - Win32 Debug"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\icon.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\Angle.cxx
# End Source File
# Begin Source File

SOURCE=.\Angle.hxx
# End Source File
# Begin Source File

SOURCE=.\Angle.inl
# End Source File
# Begin Source File

SOURCE=.\basic.cxx
# End Source File
# Begin Source File

SOURCE=.\basic.hxx
# End Source File
# Begin Source File

SOURCE=.\basic.inl
# End Source File
# Begin Source File

SOURCE=.\CoachSee.cxx
# End Source File
# Begin Source File

SOURCE=.\CoachSee.hxx
# End Source File
# Begin Source File

SOURCE=.\CoachSee.inl
# End Source File
# Begin Source File

SOURCE=.\common.hxx
# End Source File
# Begin Source File

SOURCE=.\green\Config.cxx
# End Source File
# Begin Source File

SOURCE=.\green\Config.hxx
# End Source File
# Begin Source File

SOURCE=.\green\Config.inl
# End Source File
# Begin Source File

SOURCE=.\Connection.cxx
# End Source File
# Begin Source File

SOURCE=.\Connection.hxx
# End Source File
# Begin Source File

SOURCE=.\Connection.inl
# End Source File
# Begin Source File

SOURCE=.\Double.cxx
# End Source File
# Begin Source File

SOURCE=.\Double.hxx
# End Source File
# Begin Source File

SOURCE=.\Double.inl
# End Source File
# Begin Source File

SOURCE=.\EnumValue.hxx
# End Source File
# Begin Source File

SOURCE=.\green\Green.cxx
# End Source File
# Begin Source File

SOURCE=.\green\Green.hxx
# End Source File
# Begin Source File

SOURCE=.\green\Green.inl
# End Source File
# Begin Source File

SOURCE=.\green\Gullwing.cxx
# End Source File
# Begin Source File

SOURCE=.\green\Gullwing.hxx
# End Source File
# Begin Source File

SOURCE=.\green\Gullwing.inl
# End Source File
# Begin Source File

SOURCE=.\Integer.cxx
# End Source File
# Begin Source File

SOURCE=.\Integer.hxx
# End Source File
# Begin Source File

SOURCE=.\Integer.inl
# End Source File
# Begin Source File

SOURCE=.\Log.cxx
# End Source File
# Begin Source File

SOURCE=.\Log.hxx
# End Source File
# Begin Source File

SOURCE=.\Log.inl
# End Source File
# Begin Source File

SOURCE=.\main.cxx
# End Source File
# Begin Source File

SOURCE=.\Monobrain.hxx
# End Source File
# Begin Source File

SOURCE=.\Monoworld.hxx
# End Source File
# Begin Source File

SOURCE=.\Mutex.cxx
# End Source File
# Begin Source File

SOURCE=.\Mutex.hxx
# End Source File
# Begin Source File

SOURCE=.\Mutex.inl
# End Source File
# Begin Source File

SOURCE=.\parameters.hxx
# End Source File
# Begin Source File

SOURCE=.\PlayerClient.cxx
# End Source File
# Begin Source File

SOURCE=.\PlayerClient.hxx
# End Source File
# Begin Source File

SOURCE=.\PlayerClient.inl
# End Source File
# Begin Source File

SOURCE=.\PlayerHear.cxx
# End Source File
# Begin Source File

SOURCE=.\PlayerHear.hxx
# End Source File
# Begin Source File

SOURCE=.\PlayerHear.inl
# End Source File
# Begin Source File

SOURCE=.\PlayerJunction.cxx
# End Source File
# Begin Source File

SOURCE=.\PlayerJunction.hxx
# End Source File
# Begin Source File

SOURCE=.\PlayerJunction.inl
# End Source File
# Begin Source File

SOURCE=.\PlayerObserver.cxx
# End Source File
# Begin Source File

SOURCE=.\PlayerObserver.hxx
# End Source File
# Begin Source File

SOURCE=.\PlayerObserver.inl
# End Source File
# Begin Source File

SOURCE=.\PlayerSee.cxx
# End Source File
# Begin Source File

SOURCE=.\PlayerSee.hxx
# End Source File
# Begin Source File

SOURCE=.\PlayerSee.inl
# End Source File
# Begin Source File

SOURCE=.\PlayerSenseBody.cxx
# End Source File
# Begin Source File

SOURCE=.\PlayerSenseBody.hxx
# End Source File
# Begin Source File

SOURCE=.\PlayerSenseBody.inl
# End Source File
# Begin Source File

SOURCE=.\Premise.cxx
# End Source File
# Begin Source File

SOURCE=.\Premise.hxx
# End Source File
# Begin Source File

SOURCE=.\Premise.inl
# End Source File
# Begin Source File

SOURCE=.\Random.cxx
# End Source File
# Begin Source File

SOURCE=.\Random.hxx
# End Source File
# Begin Source File

SOURCE=.\Random.inl
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\resource.rc
# End Source File
# Begin Source File

SOURCE=.\SayBuffer.cxx
# End Source File
# Begin Source File

SOURCE=.\SayBuffer.hxx
# End Source File
# Begin Source File

SOURCE=.\SayBuffer.inl
# End Source File
# Begin Source File

SOURCE=.\Screen.cxx
# End Source File
# Begin Source File

SOURCE=.\Screen.hxx
# End Source File
# Begin Source File

SOURCE=.\Screen.inl
# End Source File
# Begin Source File

SOURCE=.\Server.cxx
# End Source File
# Begin Source File

SOURCE=.\Server.hxx
# End Source File
# Begin Source File

SOURCE=.\Server.inl
# End Source File
# Begin Source File

SOURCE=.\green\SimpleNN.hxx
# End Source File
# Begin Source File

SOURCE=.\Synchronizer.cxx
# End Source File
# Begin Source File

SOURCE=.\Synchronizer.hxx
# End Source File
# Begin Source File

SOURCE=.\Synchronizer.inl
# End Source File
# Begin Source File

SOURCE=.\Vector.cxx
# End Source File
# Begin Source File

SOURCE=.\Vector.hxx
# End Source File
# Begin Source File

SOURCE=.\Vector.inl
# End Source File
# Begin Source File

SOURCE=.\White.cxx
# End Source File
# Begin Source File

SOURCE=.\White.hxx
# End Source File
# Begin Source File

SOURCE=.\White.inl
# End Source File
# Begin Source File

SOURCE=.\Window.cxx
# End Source File
# Begin Source File

SOURCE=.\Window.hxx
# End Source File
# Begin Source File

SOURCE=.\Window.inl
# End Source File
# Begin Source File

SOURCE=.\WindowsScreen.cxx
# End Source File
# Begin Source File

SOURCE=.\WindowsScreen.hxx
# End Source File
# Begin Source File

SOURCE=.\WindowsScreen.inl
# End Source File
# Begin Source File

SOURCE=.\WindowsWindow.cxx
# End Source File
# Begin Source File

SOURCE=.\WindowsWindow.hxx
# End Source File
# Begin Source File

SOURCE=.\WindowsWindow.inl
# End Source File
# End Target
# End Project
