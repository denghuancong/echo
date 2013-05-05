SRCEXTS = .c .C .cc .cpp .c++ .cxx .cp
HDREXTS = .h .H .hh .hpp .h++ .hxx .hp
LIBS =
CC = gcc
CXX = g++
CTAGS = ctags

INCLUDES=-I/usr/local/include -I/usr/include -I../libs
LIBS=-L/usr/local/lib -L/usr/lib -L../libs -lcommon 
CFLAGS = -g -c $(INCLUDES)
OFLAGS = -g $(LIBS)

SRCDIR = .
SRCS = $(wildcard $(SRCDIR)/*.cc $(SRCDIR)/*.cpp)
OBJS = $(addsuffix .o, $(basename $(SRCS)))
HEADS = $(addsuffix .h, $(basename $(SRCS)))

APP=echo_client

all : $(APP) 
	mv $(APP) ../bin
show :
	@echo $(CFLAGS)

$(APP) : $(OBJS)
	$(CXX) $(OFLAGS) -o $@ $^

%.o:%.cc
	$(CXX) $(CFLAGS) -o $@ $<

%.o:%.cpp
	$(CXX) $(CFLAGS) -o $@ $<

clean :
	rm -Rf *.o $(APP)

