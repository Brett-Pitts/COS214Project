ALL_FILES := $(wildcard GUI/*.cpp)

# List the files you want to drop, separated by SPACES
EXCLUDE_TEST_FILES := test_main.cpp tests.cpp
EXCLUDE_TEST_PATHS := $(addprefix GUI/, $(EXCLUDE_TEST_FILES))

# Filter them out
SRC_FILES_MAIN := $(filter-out $(EXCLUDE_TEST_PATHS), $(ALL_FILES))
SRC_FILES_TEST := $(filter-out GUI/main.cpp, $(ALL_FILES))

OBJ_FILES_MAIN := $(patsubst GUI/%.cpp, bin/%.o, $(SRC_FILES_MAIN))
OBJ_FILES_TEST := $(patsubst GUI/%.cpp, bin/%.o, $(SRC_FILES_TEST))


CXX        := g++
CXXFLAGS   := -Wall -Wextra -std=c++11 -IGUI $(shell pkg-config --cflags Qt5Widgets)
TARGET     := TIVIDY
TARGETTEST := $(TARGET)test

LIBS = $(shell pkg-config --libs Qt5Widgets)

all: $(TARGET) | saves

saves:
	mkdir -p saves

bin/%.o: GUI/%.cpp | bin
	$(CXX) $(CXXFLAGS) -c $< -o $@

bin:
	mkdir -p bin

$(TARGET): $(OBJ_FILES_MAIN)
	$(CXX) $(OBJ_FILES_MAIN) -o $@ $(LIBS)

$(TARGETTEST): $(OBJ_FILES_TEST)
	$(CXX) -g $(OBJ_FILES_TEST) -o $@ $(LIBS)

run: $(TARGET)
	./$(TARGET)

test: $(TARGETTEST)
	./$(TARGETTEST)


clean:
	rm -rf bin $(TARGET) $(TARGETTEST) *.exe


