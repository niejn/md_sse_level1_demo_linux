#
####                          Copyright (c) 2016
#                                  AcceleCom
#                                  #                                  #                                  
#                             All Rights Reserved
#                             #                             #                             
# File: Makefile
# # # # $Id$
# # 
# #

DIR_SRC = ./src
DIR_INC = ./include
DIR_OBJ = ./obj
DIR_LIB = ./lib

SRC = $(DIR_SRC)/userfunc.cpp \
      $(DIR_SRC)/FixSseApiStruct.cpp
OBJ = $(patsubst %.cpp,$(DIR_OBJ)/%.o,$(notdir $(SRC)))
LIB = $(patsubst %.cpp,$(DIR_LIB)/%.a,$(notdir $(SRC)))

BIN_TARGET = userdemo_sse

TARGET = $(DIR_SRC)/userdemo.cpp

CC = g++

CFLAGS = -g -Wall -I$(DIR_INC) -DNOBYTEORD

$(BIN_TARGET) : $(LIB)  
	$(CC) $(TARGET) $(LIB) $(CFLAGS) -o $@ -lpthread

$(DIR_LIB)/%.a : $(DIR_OBJ)/%.o
	ar -crv $@ $<  

$(DIR_OBJ)/%.o : $(DIR_SRC)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY : clean
clean :
	find $(DIR_OBJ) -name *.o -exec rm {} \;
	find $(DIR_LIB) -name *.a -exec rm {} \;
	find . -name $(BIN_TARGET) -exec rm {} \;
