ALL_FILES := $(wildcard src/*.cpp)

# List the files you want to drop, separated by SPACES
EXCLUDE_TEST_FILES := test_main.cpp tests.cpp
EXCLUDE_TEST_PATHS := $(addprefix src/, $(EXCLUDE_TEST_FILES))

# Filter them out
SRC_FILES_MAIN := $(filter-out $(EXCLUDE_TEST_PATHS), $(ALL_FILES))
SRC_FILES_TEST := $(filter-out src/main.cpp, $(ALL_FILES))

OBJ_FILES_MAIN := $(patsubst src/%.cpp, bin/%.o, $(SRC_FILES_MAIN))
OBJ_FILES_TEST := $(patsubst src/%.cpp, bin/%.o, $(SRC_FILES_TEST))


CXX        := g++
CXXFLAGS   := -Wall -Wextra -std=c++11 -Isrc
TARGET     := TIVIDY
TARGETTEST := $(TARGET)test



all: $(TARGET)

bin/%.o: src/%.cpp | bin
	$(CXX) $(CXXFLAGS) -c $< -o $@

bin:
	mkdir -p bin

$(TARGET): $(OBJ_FILES_MAIN)
	$(CXX) $(OBJ_FILES_MAIN) -o $@

$(TARGETTEST): $(OBJ_FILES_TEST)
	$(CXX) -g $(OBJ_FILES_TEST) -o $@

run: $(TARGET)
	./$(TARGET)

test: $(TARGETTEST)
	./$(TARGETTEST)


clean:
	rm -rf bin $(TARGET) $(TARGETTEST) *.exe


