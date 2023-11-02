# Karl's src/test makefile.

#  I used information gleaned from StackOverflow and Makefile.com when I wrote precursors to this makefile. Then I used those precursors as references when writing this.

#  The idea is to compile everything in src to bin. So "src/foo/bar.c" becomes "bin/src/foo/bar.c.o" before linking.

# ------------------------------------------------------
# 			Flags and Compiler options
# ------------------------------------------------------

# Flags to be passed to the compiler.
#  -Wall : Warnings all.
#  -g : Compile for debug mode. 
CFLAGS := -Wall

# The compiler we will use
CC := gcc 

# ------------------------------------------------------
#			Directories to scan, output to
# ------------------------------------------------------

# A list of directories where .c files are located. These will be recursed, and all sub directories will be scanned.
SRC_DIRS := src

# A list of directories where .c files are located that are related to running tests. These will be recursed, and all subdirectories will be scanned.
TEST_SRC_DIRS := tests

# Where int main is located in the regular program. It will be excluded when compiling for tests, which has a different main.c
MAIN_C_SERVER := src/server/main.c

MAIN_C_CLIENT := src/client/main.c

# The directory will .o files will go
O_DIR := bin

# If there are additional includes, they can go here.
ADDITIONAL_INCLUDE_DIRS := 

# The name of the file for the server executable to be compiled. Will be dropped in root.
SERVER_OUTPUT_FILENAME := server

# The name of the file for the client executable to be compiled. Will be dropped in root.
CLIENT_OUTPUT_FILENAME := client

# The name of the test file to be compiled.
TESTOUTFILE := testfile


# ------------------------------------------------------
#			Generated variables (Don't edit)
# ------------------------------------------------------

# 	-----------------------
# 	 Regular source files 
# 	-----------------------

# Finds all files within src directories.
SERVER_SRC_FILES := $(filter-out $(MAIN_C_CLIENT), $(shell find $(SRC_DIRS) -name '*.c'))

CLIENT_SRC_FILES := $(filter-out $(MAIN_C_SERVER), $(shell find $(SRC_DIRS) -name '*.c'))

# The object files that we will generate
SERVER_OBJ_FILES := $(SERVER_SRC_FILES:%=$(O_DIR)/%.o)

CLIENT_OBJ_FILES := $(CLIENT_SRC_FILES:%=$(O_DIR)/%.o)

# Find every folder containing source code so it can be included.
SRC_INCLUDE_DIRS := $(shell find $(SRC_DIRS) -type d) $(ADDITIONAL_INCLUDE_DIRS)

# Add the -I flag so they will be included
SRC_INCLUDE := $(addprefix -I, $(SRC_INCLUDE_DIRS))

# 	-----------------------
# 	   Test source files  
# 	-----------------------

#  These vars include everything from regular compilation, with the addition of the test directories and source.

# Finds all files within the test src directories, plus the regular source files, but excludes 'main.c'
TEST_SRC_FILES := $(shell find $(TEST_SRC_DIRS) -name '*c') $(filter-out $(MAIN_C_SERVER), $(SERVER_SRC_FILES))

# Gets the list of .o files that will be created
TEST_OBJ_FILES := $(TEST_SRC_FILES:%=$(O_DIR)/%.o) 

# Generates the list of include directores
TEST_INCLUDE_DIRS := $(shell find $(TEST_SRC_DIRS) -type d)

# Generates the include flags
TEST_INCLUDE := $(addprefix -I, $(TEST_INCLUDE_DIRS)) $(SRC_INCLUDE)


# ------------------------------------------------------
#			Make commands
# ------------------------------------------------------

run: $(SERVER_OUTPUT_FILENAME)
	./$(SERVER_OUTPUT_FILENAME)

run-client: $(CLIENT_OUTPUT_FILENAME)
	./$(CLIENT_OUTPUT_FILENAME)

test: $(TESTOUTFILE)
	./$(TESTOUTFILE)

# Build .o files, and directories needed along the way
$(O_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(TEST_INCLUDE) -c $< -o $@

# Build the server program executable (after building .o files)
$(SERVER_OUTPUT_FILENAME): $(SERVER_OBJ_FILES)
	$(CC) $(SERVER_OBJ_FILES) -lpthread -o $@

# Build the client program executable (after building .o files)
$(CLIENT_OUTPUT_FILENAME): $(CLIENT_OBJ_FILES)
	$(CC) $(CLIENT_OBJ_FILES) -o $@

# Build the test executable
$(TESTOUTFILE): $(TEST_OBJ_FILES)
	$(CC) $(TEST_OBJ_FILES) -o $@

# clean, and clean files
cleanf: clean
	-rm -f "registered.txt"
	-rm -f "nohup.out"

PHONY: clean
clean:
	-rm -r $(O_DIR)
	-mkdir -p $(O_DIR)
	-rm -f $(TESTOUTFILE)
	-rm -f $(SERVER_OUTPUT_FILENAME)
	-rm -f $(CLIENT_OUTPUT_FILENAME)
	-rm example/client
	-rm example/server
	-rm example/server2
	