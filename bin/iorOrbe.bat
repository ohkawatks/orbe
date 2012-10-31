@echo off

REM
REM $Id: iorOrbe.bat 793 2008-09-17 02:09:28Z takaya $
REM Copyright (C) 2008 AIST, National Institute of Advanced Industrial Science and Technology
REM

set JAVA_HOME=C:\usr\java\jdk1.6.0_03

if "%1" == "" goto error
if "%2" == "" goto error
if "%3" == "" goto error

%JAVA_HOME%\bin\java -classpath ..\lib\orbeUtils.jar jp.go.aist.utils.IORgenerator %1 %2 %3
goto end

:error
echo Usage: iorOrbe I/Fname ipaddress port

:end
