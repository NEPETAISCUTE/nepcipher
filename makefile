C = g++
libC = gcc
srcfiles = src/nepcipher.c++ src/util/nepstring.c
libs = -L./lib -lnepstring
includepath = -I./src/include
opts = -g
outfiles = -o nepcipher

libver:
	g++ -g -I./src/include src/nepcipher.c++ $(libs) $(outfiles)
saver:
	g++ -g -I./src/include src/nepcipher.c++ src/nepstring.c $(outfiles)