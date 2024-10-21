# makefile
CC = gcc
OBJ = main.o foo.o

# Cette variable sera utilisée dans la règle implicite qui compile les .o  
CFLAGS = # My C Flags for the compiler
LDFLAGS = # My linker flags
TARGET = a.out

SRCDIR = src
TEST_DIR = tests
BUILD_DIR = build
LIB_TARGET = libmylib.a

SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
SRC_TEST = $(wildcard $(TEST_DIR)/*.c)
OBJ_TEST = $(SRC:$(TEST_DIR)/%.c=$(TEST_DIR)/%.o)

LIB_OBJS = $(filter-out $(BUILD_DIR)/main.o, $(OBJS))

all: $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

debug: CFLAGS += -g
debug: all
check: $(filter-out $(SRC_DIR)/main.o, $(OBJ)) $(OBJ_TEST)
		$(CC) $^ -o $(TARGET) $(LDFLAGS)

coverage: CFLAGS += -fPIC --coverage
coverage: LDFLAGS += -lgcov -lcriterion
coverage: check
		gcovr --print-summary

$(LIB_TARGET): $(LIB_OBJS)
	ar rcs $@ $^

# Compilation des fichiers source
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
		$(CC) $(CFLAGS) -c $< -o $@
    
# Creer le repertoire de build
$(BUILD_DIR):
		mkdir -p $(BUILD_DIR)

clean: 
    $(RM) $(OBJ)
    $(RM) $(TARGET)

.PHONY: clean all
