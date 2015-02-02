###############
## Makefile
###############

BIN=../bin/MessageApi

##ARCH = $(shell uname -m)	# ARCH becomes the output from the uname -m command.
CXX=/usr/bin/g++
CXXFLAGS=-g -O2 -std=c++11 -Wall -march=native -mtune=native -DOF_USING_GTK -pthread -D_REENTRANT $(INCS)
BOOST_ROOT=/home/dev/boost
OF_INCLUDE_ROOT=/home/dev/Downloads/of_v0.8.4_linux64_release/libs/openFrameworks
OF_EXTRA_INCLUDE_ROOT=/home/dev/Downloads/of_v0.8.4_linux64_release/libs
XMLPARSER_ROOT=/home/dev/svnProjects/myproject/xmlParserApi
CTEMPLATES_ROOT=/home/dev/ctemplates

SRCS= LatencyStatManager.cpp HttpReqHandler.cpp ChannelTest.cpp Transport.cpp HttpReply.cpp HttpReqParser.cpp HttpServerTransport.cpp HttpClientConnectionManager.cpp HttpClientConnection.cpp 

OBJS = $(patsubst %.cpp, %.o, $(SRCS))

INCS = -I../include \
       -I$(BOOST_ROOT)/include \
       -I$(XMLPARSER_ROOT)/include \
       -I$(CTEMPLATES_ROOT)/include \

LIBS = \
        $(BOOST_ROOT)/lib/libboost_thread.a \
	$(BOOST_ROOT)/lib/libboost_system.a \
	$(XMLPARSER_ROOT)/lib/libMessageApi.a \
	$(CTEMPLATES_ROOT)/lib/libctemplate.a \
	-lpthread
	
$(BIN): $(OBJS)
	$(CXX) -o $(BIN) $(OBJS) $(LIBS)

all: $(BIN) run
	
run:
	@echo "----------------------------"
	@echo "Running the programme. "
	@echo "----------------------------"
	$(BIN)
	@echo "----------------------------"

clean:
	rm -rf $(BIN) *.o
