# Microsoft Developer Studio Project File - Name="blue" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=blue - Win32 Debug
!MESSAGE これは有効な匆弧РB この餅杣洽個を凡拂NMAKE を使用してください。
!MESSAGE [匆弧Р夢現累按] 栽歡g用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "blue.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE 栽歡弉渡雇の設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "blue.mak" CFG="blue - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能な凡拂唹爪:
!MESSAGE 
!MESSAGE "blue - Win32 Release" ("Win32 (x86) Console Application" 用)
!MESSAGE "blue - Win32 Debug" ("Win32 (x86) Console Application" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "blue - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W4 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D LEARNING=1 /YX /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib winmm.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "blue - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W4 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D LEARNING=1 /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib winmm.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "blue - Win32 Release"
# Name "blue - Win32 Debug"
# Begin Source File

SOURCE=..\Angle.cxx
# End Source File
# Begin Source File

SOURCE=..\Angle.hxx
# End Source File
# Begin Source File

SOURCE=..\Angle.inl
# End Source File
# Begin Source File

SOURCE=..\basic.cxx
# End Source File
# Begin Source File

SOURCE=..\basic.hxx
# End Source File
# Begin Source File

SOURCE=..\basic.inl
# End Source File
# Begin Source File

SOURCE=.\Blue.cxx
# End Source File
# Begin Source File

SOURCE=.\Blue.hxx
# End Source File
# Begin Source File

SOURCE=.\Blue.inl
# End Source File
# Begin Source File

SOURCE=.\Client.cxx
# End Source File
# Begin Source File

SOURCE=.\Client.hxx
# End Source File
# Begin Source File

SOURCE=.\Client.inl
# End Source File
# Begin Source File

SOURCE=..\CoachSee.cxx
# End Source File
# Begin Source File

SOURCE=..\CoachSee.hxx
# End Source File
# Begin Source File

SOURCE=..\CoachSee.inl
# End Source File
# Begin Source File

SOURCE=..\common.hxx
# End Source File
# Begin Source File

SOURCE=..\Connection.cxx
# End Source File
# Begin Source File

SOURCE=..\Connection.hxx
# End Source File
# Begin Source File

SOURCE=..\Connection.inl
# End Source File
# Begin Source File

SOURCE=..\Double.cxx
# End Source File
# Begin Source File

SOURCE=..\Double.hxx
# End Source File
# Begin Source File

SOURCE=..\Double.inl
# End Source File
# Begin Source File

SOURCE=..\EnumValue.hxx
# End Source File
# Begin Source File

SOURCE=..\Integer.cxx
# End Source File
# Begin Source File

SOURCE=..\Integer.hxx
# End Source File
# Begin Source File

SOURCE=..\Integer.inl
# End Source File
# Begin Source File

SOURCE=..\Log.cxx
# End Source File
# Begin Source File

SOURCE=..\Log.hxx
# End Source File
# Begin Source File

SOURCE=..\Log.inl
# End Source File
# Begin Source File

SOURCE=..\main.cxx
# End Source File
# Begin Source File

SOURCE=..\Monobrain.hxx
# End Source File
# Begin Source File

SOURCE=..\Monoworld.hxx
# End Source File
# Begin Source File

SOURCE=..\Mutex.cxx
# End Source File
# Begin Source File

SOURCE=..\Mutex.hxx
# End Source File
# Begin Source File

SOURCE=..\Mutex.inl
# End Source File
# Begin Source File

SOURCE=..\parameters.hxx
# End Source File
# Begin Source File

SOURCE=..\PlayerClient.cxx
# End Source File
# Begin Source File

SOURCE=..\PlayerClient.hxx
# End Source File
# Begin Source File

SOURCE=..\PlayerClient.inl
# End Source File
# Begin Source File

SOURCE=..\PlayerHear.cxx
# End Source File
# Begin Source File

SOURCE=..\PlayerHear.hxx
# End Source File
# Begin Source File

SOURCE=..\PlayerHear.inl
# End Source File
# Begin Source File

SOURCE=..\PlayerJunction.cxx
# End Source File
# Begin Source File

SOURCE=..\PlayerJunction.hxx
# End Source File
# Begin Source File

SOURCE=..\PlayerJunction.inl
# End Source File
# Begin Source File

SOURCE=..\PlayerObserver.cxx
# End Source File
# Begin Source File

SOURCE=..\PlayerObserver.hxx
# End Source File
# Begin Source File

SOURCE=..\PlayerObserver.inl
# End Source File
# Begin Source File

SOURCE=..\PlayerSee.cxx
# End Source File
# Begin Source File

SOURCE=..\PlayerSee.hxx
# End Source File
# Begin Source File

SOURCE=..\PlayerSee.inl
# End Source File
# Begin Source File

SOURCE=..\PlayerSenseBody.cxx
# End Source File
# Begin Source File

SOURCE=..\PlayerSenseBody.hxx
# End Source File
# Begin Source File

SOURCE=..\PlayerSenseBody.inl
# End Source File
# Begin Source File

SOURCE=..\Premise.cxx
# End Source File
# Begin Source File

SOURCE=..\Premise.hxx
# End Source File
# Begin Source File

SOURCE=..\Premise.inl
# End Source File
# Begin Source File

SOURCE=..\Random.cxx
# End Source File
# Begin Source File

SOURCE=..\Random.hxx
# End Source File
# Begin Source File

SOURCE=..\Random.inl
# End Source File
# Begin Source File

SOURCE=..\resource.h
# End Source File
# Begin Source File

SOURCE=..\SayBuffer.cxx
# End Source File
# Begin Source File

SOURCE=..\SayBuffer.hxx
# End Source File
# Begin Source File

SOURCE=..\SayBuffer.inl
# End Source File
# Begin Source File

SOURCE=..\Screen.cxx
# End Source File
# Begin Source File

SOURCE=..\Screen.hxx
# End Source File
# Begin Source File

SOURCE=..\Screen.inl
# End Source File
# Begin Source File

SOURCE=..\Server.cxx
# End Source File
# Begin Source File

SOURCE=..\Server.hxx
# End Source File
# Begin Source File

SOURCE=..\Server.inl
# End Source File
# Begin Source File

SOURCE=..\Synchronizer.cxx
# End Source File
# Begin Source File

SOURCE=..\Synchronizer.hxx
# End Source File
# Begin Source File

SOURCE=..\Synchronizer.inl
# End Source File
# Begin Source File

SOURCE=..\Vector.cxx
# End Source File
# Begin Source File

SOURCE=..\Vector.hxx
# End Source File
# Begin Source File

SOURCE=..\Vector.inl
# End Source File
# Begin Source File

SOURCE=..\White.cxx
# End Source File
# Begin Source File

SOURCE=..\White.hxx
# End Source File
# Begin Source File

SOURCE=..\White.inl
# End Source File
# Begin Source File

SOURCE=..\Window.cxx
# End Source File
# Begin Source File

SOURCE=..\Window.hxx
# End Source File
# Begin Source File

SOURCE=..\Window.inl
# End Source File
# Begin Source File

SOURCE=..\WindowsScreen.cxx
# End Source File
# Begin Source File

SOURCE=..\WindowsScreen.hxx
# End Source File
# Begin Source File

SOURCE=..\WindowsScreen.inl
# End Source File
# Begin Source File

SOURCE=..\WindowsWindow.cxx
# End Source File
# Begin Source File

SOURCE=..\WindowsWindow.hxx
# End Source File
# Begin Source File

SOURCE=..\WindowsWindow.inl
# End Source File
# End Target
# End Project
