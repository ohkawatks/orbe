@echo off

REM
REM $Id: bridgeOrbe.bat 793 2008-09-17 02:09:28Z takaya $
REM Copyright (C) 2008 AIST, National Institute of Advanced Industrial Science and Technology
REM

set JAVA_HOME=C:\usr\java\jdk1.6.0_03
set PATH=%PATH%;../lib

if "%1" == "" goto error_bridge

if "%1" == "TCPtoUART" (
	if "%2" == "" goto error_TCPtoUART
	if "%3" == "" goto error_TCPtoUART
	%JAVA_HOME%\bin\java -classpath ..\lib\orbeUtils.jar;..\lib\RXTXcomm.jar jp.go.aist.utils.TCPtoUART %2 %3
	goto end
)
if "%1" == "TCPtoUDP" (
	if "%2" == "" goto error_TCPtoUDP
	if "%3" == "" goto error_TCPtoUDP
	if "%4" == "" goto error_TCPtoUDP
	%JAVA_HOME%\bin\java -classpath ..\lib\orbeUtils.jar jp.go.aist.utils.TCPtoUDP %2 %3 %4
	goto end
)

:error_bridge
echo Usage: bridge [TCPtoUART] [TCPtoUDP] ...
goto end

:error_TCPtoUART
echo Usage: bridge TCPtoUART COMport listenport
goto end

:error_TCPtoUDP
echo Usage: bridge TCPtoUDP listenport targethost targetport
goto end

:end

