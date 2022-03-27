APP_NAME = fifteen
LIB_NAME = lib$(APP_NAME)
TEST_NAME = $(APP_NAME)-test

CC = g++
CFLAGS = -W -Wall -Wextra -Werror
LFLAGS = -I src -MP -MMD -lsfml-graphics -lsfml-window -lsfml-system
TESTFLAGS = -pthread -lgtest -I $(GTEST_SRC_PATH)/googletest/include/ -Llib 

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
TEST_DIR = test
THIRDPARTY = thirdparty

TEST_PATH = $(BIN_DIR)/$(TEST_NAME)
GTEST_LIB = libgtest.a
GTEST_PATH = lib/$(GTEST_LIB)
GTEST_SRC_PATH = build/googletest

APP_PATH = $(BIN_DIR)/$(APP_NAME)
OBJ_PATH = $(OBJ_DIR)/$(SRC_DIR)
LIB_PATH = $(OBJ_DIR)/$(SRC_DIR)/$(LIB_NAME)/$(LIB_NAME).a

APP_SOURCES = $(shell find $(SRC_DIR)/$(APP_NAME) -name '*.cpp')
APP_OBJECTS = $(APP_SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_PATH)/%.o)

LIB_SOURCES = $(shell find $(SRC_DIR)/$(LIB_NAME) -name '*.cpp')
LIB_OBJECTS = $(LIB_SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_PATH)/%.o)

TEST_SOURCES = $(shell find $(TEST_DIR) -name '*.cpp')
TEST_OBJECTS = $(TEST_SOURCES:%.cpp=$(OBJ_DIR)/%.o)

DEPS = $(APP_OBJECTS:.o=.d) $(LIB_OBJECTS:.o=.d) 

all: $(APP_PATH)

$(APP_PATH): $(APP_OBJECTS) $(LIB_PATH)
	$(CC) $(CFLAGS) $^ -o $@ $(LFLAGS) 

$(LIB_PATH): $(LIB_OBJECTS)
	ar rcs $@ $^

$(OBJ_PATH)/$(LIB_NAME)/%.o: $(SRC_DIR)/$(LIB_NAME)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ $(LFLAGS) 

$(OBJ_PATH)/$(APP_NAME)/%.o: $(SRC_DIR)/$(APP_NAME)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ $(LFLAGS) 

test: all $(GTEST_PATH) $(TEST_PATH)

#$(TEST_PATH): $(GTEST_PATH) $(LIB_PATH)
#	$(CC) $(CFLAGS) $(TEST_DIR)/main.cpp $(LIB_PATH) -I $(TEST_DIR) -o $@ $(LFLAGS) $(TESTFLAGS)

$(TEST_PATH): $(TEST_OBJECTS)
	$(CC) $(CFLAGS) -I $(TEST_DIR) $^ $(LIB_PATH) -o $@ $(LFLAGS) $(TESTFLAGS)

$(OBJ_DIR)/$(TEST_DIR)/%.o: $(TEST_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -I $(TEST_DIR) -o $@ $(LFLAGS) $(TESTFLAGS)

$(GTEST_PATH): $(GTEST_SRC_PATH)
	 cd $(GTEST_SRC_PATH) &&\
		mkdir -p build &&\
		cd build &&\
	   	cmake .. -DBUILD_SHARED_LIBS=OFF -DBUILD_GMOCK=OFF &&\
		make 
	 mkdir -p bin lib obj
	 cp $(GTEST_SRC_PATH)/build/lib/*.a lib/

$(GTEST_SRC_PATH):
	git clone https://github.com/google/googletest.git $@

clean:
	$(RM) $(APP_PATH) $(LIB_PATH) $(TEST_PATH)
	find $(OBJ_DIR) -name '*.[od]' -exec $(RM) '{}' \;
	$(RM) "bin/*" -f

clean-all: clean
	$(RM) build/ lib/ -r

.PHONY: all clean

-include $(DEPS)
