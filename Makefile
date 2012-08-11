CFLAGS = -O3 -Wall -std=c99

BUILD_DIR = build
EXE_FILE = $(BUILD_DIR)/bin/seuss
LIB_FILE = $(BUILD_DIR)/lib/libseuss.a
INC_DIR = $(BUILD_DIR)/include/Seuss
OBJ_DIR = $(BUILD_DIR)/obj

OBJECTS = $(OBJ_DIR)/SUError.o $(OBJ_DIR)/SUFunction.o $(OBJ_DIR)/SUIterator.o $(OBJ_DIR)/SUList.o $(OBJ_DIR)/SUProgram.o $(OBJ_DIR)/SUStatement.o $(OBJ_DIR)/SUString.o $(OBJ_DIR)/SUTokenizer.o $(OBJ_DIR)/SUType.o $(OBJ_DIR)/SUVariable.o

.PHONY : all bin lib inc clean install

all : bin lib

clean : 
	rm -rf $(BUILD_DIR)

install : all
	cp $(EXE_FILE) /usr/bin/seuss
	cp $(LIB_FILE) /usr/lib/seuss
	cp -r $(INC_DIR) /usr/include/Seuss

bin : $(EXE_FILE)

lib : $(LIB_FILE) inc

inc : 
	-@ mkdir -p $(INC_DIR)
	-@ cp seuss/Seuss.h $(INC_DIR)/seuss.h
	-@ cp seuss/SUType.h $(INC_DIR)/SUType.h
	-@ cp seuss/SUTokenizer.h $(INC_DIR)/SUTokenizer.h
	-@ cp seuss/SUList.h $(INC_DIR)/SUList.h
	-@ cp seuss/SUIterator.h $(INC_DIR)/SUIterator.h
	-@ cp seuss/SUString.h $(INC_DIR)/SUString.h
	-@ cp seuss/SUFunction.h $(INC_DIR)/SUFunction.h
	-@ cp seuss/SUStatement.h $(INC_DIR)/SUStatement.h
	-@ cp seuss/SUProgram.h $(INC_DIR)/SUProgram.h
	-@ cp seuss/SURange.h $(INC_DIR)/SURange.h
	-@ cp seuss/SUError.h $(INC_DIR)/SUError.h
	-@ cp seuss/SUVariable.h $(INC_DIR)/SUVariable.h

$(EXE_FILE) : seuss/seuss.c lib
	-@ mkdir -p $(BUILD_DIR)/bin
	$(CC) $(CFLAGS) -o $(EXE_FILE) $(LIB_FILE) seuss/seuss.c

$(LIB_FILE) : $(OBJECTS)
	-@ mkdir -p $(BUILD_DIR)/lib
	ar r $(LIB_FILE) $(OBJ_DIR)/*.o

$(OBJ_DIR)/SUError.o : seuss/SUError.c
	-@ mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $(OBJ_DIR)/SUError.o seuss/SUError.c
	
$(OBJ_DIR)/SUFunction.o : seuss/SUFunction.c
	-@ mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $(OBJ_DIR)/SUFunction.o seuss/SUFunction.c

$(OBJ_DIR)/SUIterator.o : seuss/SUIterator.c
	-@ mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $(OBJ_DIR)/SUIterator.o seuss/SUIterator.c

$(OBJ_DIR)/SUList.o : seuss/SUList.c
	-@ mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $(OBJ_DIR)/SUList.o seuss/SUList.c

$(OBJ_DIR)/SUProgram.o : seuss/SUProgram.c
	-@ mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $(OBJ_DIR)/SUProgram.o seuss/SUProgram.c

$(OBJ_DIR)/SUStatement.o : seuss/SUStatement.c
	-@ mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $(OBJ_DIR)/SUStatement.o seuss/SUStatement.c

$(OBJ_DIR)/SUString.o : seuss/SUString.c
	-@ mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $(OBJ_DIR)/SUString.o seuss/SUString.c

$(OBJ_DIR)/SUTokenizer.o : seuss/SUTokenizer.c
	-@ mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $(OBJ_DIR)/SUTokenizer.o seuss/SUTokenizer.c

$(OBJ_DIR)/SUType.o : seuss/SUType.c
	-@ mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $(OBJ_DIR)/SUType.o seuss/SUType.c

$(OBJ_DIR)/SUVariable.o : seuss/SUVariable.c
	-@ mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $(OBJ_DIR)/SUVariable.o seuss/SUVariable.c