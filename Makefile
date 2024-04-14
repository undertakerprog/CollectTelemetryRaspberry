CC=gcc
CFLAGS=-W -Wall -Wno-unused-parameter -Wno-unused-variable -Werror -std=c11 -pedantic -lc -D_POSIX_C_SOURCE=200809L -I/usr/include/cjson -I/usr/include 
LDFLAGS=-L/usr/lib/x86_64-linux-gnu -lcjson -lwiringPi 
SRC=src
BUILD=build
DEBUG=$(BUILD)/debug
RELEASE=$(BUILD)/release
DIST=$(DEBUG)
CUR_DIR=$(shell basename $(CURDIR))
SSH_SERVER=undertaker@172.20.10.2
SERVER_PATH=~/$(CUR_DIR)

SRCS=$(wildcard $(SRC)/*.c)
OBJS=$(patsubst $(SRC)/%.c, $(DIST)/%.o, $(SRCS))

.POSIX:
.PHONY: clean
.ONESHELL:
.NOTPARALLEL:

all: remote_build

$(DIST)/%.o: $(SRC)/%.c
	@mkdir -p $(@D)    
	$(CC) $(CFLAGS) -o $@ -c $<

remote_build: $(OBJS)  
	rsync --delete -r $(CURDIR)/ "$(SSH_SERVER):$(SERVER_PATH)"    
	ssh $(SSH_SERVER) "cd $(SERVER_PATH) && $(CC) $(CFLAGS) $(SRCS) -o  $(DIST)/main $(LDFLAGS)"

build: $(OBJS)   
	$(CC) $(CFLAGS) -o $(DIST)/main $^ $(LDFLAGS) 

clean:    
	rm -rf $(BUILD)
