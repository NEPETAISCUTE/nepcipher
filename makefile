C = g++
libC = gcc
srcfiles = src/nepcipher.c++ src/util/nepstring.c
libs = -L./lib -lnepstring
includepath = -I./src/include
opts = -g
outfiles = -o bin/nepcipher

libver:
	g++ -I./src/include -Wall src/nepcipher.c++ $(libs) $(outfiles)
