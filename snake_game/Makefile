# Compiler and target

.PHONY : all clean

PRJ_NAME := app

LIB_NAME := template

CUR_DIR := .
INC_DIR := $(CUR_DIR)/inc
SRC_DIR := $(CUR_DIR)/src
OBJ_DIR := $(CUR_DIR)/objs
BIN_DIR := $(CUR_DIR)/bin

LIB_DIR := $(CUR_DIR)/lib

CC = /home/quan/gcc-linaro-6.5.0-2018.12-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc

C_FLAGS := -c -fPIC -o

#create objs file 

obj:
	$(CC) $(C_FLAGS) $(OBJ_DIR)/main.o $(CUR_DIR)/main.c -I $(INC_DIR)
	$(CC) $(C_FLAGS) $(OBJ_DIR)/st7735_user_lib.o $(SRC_DIR)/st7735_user_lib.c -I $(INC_DIR)
	$(CC) $(C_FLAGS) $(OBJ_DIR)/snake.o $(SRC_DIR)/snake.c -I $(INC_DIR)
	$(CC) $(C_FLAGS) $(OBJ_DIR)/keyboard.o $(SRC_DIR)/keyboard.c -I $(INC_DIR)
	$(CC) $(C_FLAGS) $(OBJ_DIR)/app.o $(SRC_DIR)/app.c -I $(INC_DIR)

static:
	ar rcs $(LIB_DIR)/lib$(LIB_NAME).a $(OBJ_DIR)/*.o

all: clean obj static
	$(CC) $(OBJ_DIR)/main.o -L$(LIB_DIR) -l$(LIB_NAME) -o $(BIN_DIR)/$(PRJ_NAME)

clean:
	rm -rf $(OBJ_DIR)/*
	rm -rf $(LIB_DIR)/*
	rm -rf $(BIN_DIR)/*

trans:
	lsblk
	sudo mount /dev/sdb1 /media/rootfs/
	lsblk
	sudo rm -rf /media/rootfs/home/debian/st7735
	ls -l /media/rootfs/home/debian/
	sudo cp ./bin/* /media/rootfs/home/debian/
	sync
	sudo umount /media/rootfs/
	lsblk