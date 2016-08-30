#!/bin/sh

# Run this script form this folder
# If needed modify this script

g++ -c source/token.cpp -o obj/Release/token.o -std=c++11
g++ -c source/scope.cpp -o obj/Release/scope.o -std=c++11
g++ -c source/run.cpp -o obj/Release/run.o -std=c++11
g++ -c source/options.cpp -o obj/Release/options.o -std=c++11
g++ -c source/management.cpp -o obj/Release/management.o -std=c++11
g++ -c source/main.cpp -o obj/Release/main.o -std=c++11
g++ -c source/log.cpp -o obj/Release/log.o -std=c++11
g++ -c source/locate.cpp -o obj/Release/locate.o -std=c++11
g++ -c source/lexer.cpp -o obj/Release/lexer.o -std=c++11
g++ -c source/file.cpp -o obj/Release/file.o -std=c++11
g++ -c source/errors.cpp -o obj/Release/errors.o -std=c++11
g++ -c source/enforcer.cpp -o obj/Release/enforcer.o -std=c++11
g++ -c source/dump.cpp -o obj/Release/dump.o -std=c++11
g++ -c source/die.cpp -o obj/Release/die.o -std=c++11
g++ -c source/core.cpp -o obj/Release/core.o -std=c++11
g++ -c source/context.cpp -o obj/Release/context.o -std=c++11
g++ -c source/assembler.cpp -o obj/Release/assembler.o -std=c++11

pushd
cd obj/Release
g++ token.o scope.o run.o options.o management.o main.o log.o locate.o lexer.o \
	file.o errors.o enforcer.o dump.o die.o core.o context.o assembler.o -lboost_system-mgw47-mt-1_60 \
	-lboost_filesystem-mgw47-mt-1_60 -o ../../bin/Release/BoomslangCompiler
popd
