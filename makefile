C = g++
libC = gcc
srcfiles = src/nepcipher.c++ src/util/nepstring.c
libs = -L./lib -lnepstring
includepath = -I./src/include
opts = -g
outfiles = -o bin/nepcipher

libver:
	g++ -g -I./src/include -Wall src/nepcipher.c++ $(libs) $(outfiles)
release:
	g++ -I./src/include -Wall src/nepcipher.c++ $(libs) $(outfiles)