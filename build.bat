
@echo off
rem Run this script form this folder
rem Set the location of the g++ compiler and boost library locations
rem If needed modify the rest of this script

set COMPILER="C:\Program Files (x86)\CodeBlocks\MinGW\bin\mingw32-g++.exe"
set BOOST_LIBRARIES="C:\Program Files (x86)\CodeBlocks\MinGW\lib\boost"
@echo on

%COMPILER% -c source\token.cpp -o obj\Release\token.o -std=c++11
%COMPILER% -c source\scope.cpp -o obj\Release\scope.o -std=c++11
%COMPILER% -c source\run.cpp -o obj\Release\run.o -std=c++11
%COMPILER% -c source\options.cpp -o obj\Release\options.o -std=c++11
%COMPILER% -c source\management.cpp -o obj\Release\management.o -std=c++11
%COMPILER% -c source\main.cpp -o obj\Release\main.o -std=c++11
%COMPILER% -c source\log.cpp -o obj\Release\log.o -std=c++11
%COMPILER% -c source\locate.cpp -o obj\Release\locate.o -std=c++11
%COMPILER% -c source\lexer.cpp -o obj\Release\lexer.o -std=c++11
%COMPILER% -c source\file.cpp -o obj\Release\file.o -std=c++11
%COMPILER% -c source\errors.cpp -o obj\Release\errors.o -std=c++11
%COMPILER% -c source\enforcer.cpp -o obj\Release\enforcer.o -std=c++11
%COMPILER% -c source\dump.cpp -o obj\Release\dump.o -std=c++11
%COMPILER% -c source\die.cpp -o obj\Release\die.o -std=c++11
%COMPILER% -c source\core.cpp -o obj\Release\core.o -std=c++11
%COMPILER% -c source\context.cpp -o obj\Release\context.o -std=c++11
%COMPILER% -c source\assembler.cpp -o obj\Release\assembler.o -std=c++11

@echo off
pushd
cd obj\Release
@echo on
%COMPILER% token.o scope.o run.o options.o management.o main.o log.o locate.o lexer.o ^
	file.o errors.o enforcer.o dump.o die.o core.o context.o assembler.o %BOOST_LIBRARIES%\libboost_system-mgw47-mt-1_60.a ^
	%BOOST_LIBRARIES%\libboost_filesystem-mgw47-mt-1_60.a -o ..\..\bin\Release\BoomslangCompiler.exe
@echo off
popd
echo.
echo Build finished, resulting executable should be in bin\Release\BoomslangCompiler.exe
pause
@echo on