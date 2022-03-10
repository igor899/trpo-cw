APP_NAME = fifteen
LIB_NAME = lib$(APP_NAME)

CC = g++
CFLAGS = -W -Wall -Wextra -Werror
LFLAGS = -I src -MP -MMD -lsfml-graphics -lsfml-window -lsfml-system

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

APP_PATH = $(BIN_DIR)/$(APP_NAME)
OBJ_PATH = $(OBJ_DIR)/$(SRC_DIR)
LIB_PATH = $(OBJ_DIR)/$(SRC_DIR)/$(LIB_NAME)/$(LIB_NAME).a

APP_SOURCES = $(shell find $(SRC_DIR)/$(APP_NAME) -name '*.cpp')
APP_OBJECTS = $(APP_SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_PATH)/%.o)

LIB_SOURCES = $(shell find $(SRC_DIR)/$(LIB_NAME) -name '*.cpp')
LIB_OBJECTS = $(LIB_SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_PATH)/%.o)

DEPS = $(APP_OBJECTS:.o=.d) $(LIB_OBJECTS:.o=.d)


all: $(APP_PATH)

$(APP_PATH): $(APP_OBJECTS) $(LIB_PATH)
	$(CC) $(CFLAGS) $^ -o $@ $(LFLAGS) 

$(LIB_PATH): $(LIB_OBJECTS)
	ar rcs $@ $^

$(OBJ_PATH)/$(LIB_NAME)/%.o: $(SRC_DIR)/$(LIB_NAME)/%.cpp
	@echo heyy
	$(CC) $(CLFLAGS) -c $< -o $@ $(LFLAGS) 

$(OBJ_PATH)/$(APP_NAME)/%.o: $(SRC_DIR)/$(APP_NAME)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ $(LFLAGS) 

clean:
	$(RM) $(APP_PATH) $(LIB_PATH)
	find $(OBJ_DIR) -name '*.[od]' -exec $(RM) '{}' \;

.PHONY: all clean

-include $(DEPS)
