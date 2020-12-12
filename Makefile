SRCS_DIR := ./src
BUILD_DIR := ./build

SRCS := $(wildcard $(SRCS_DIR)/*.cc)
OBJS := $(subst $(SRCS_DIR),$(BUILD_DIR),$(SRCS))
OBJS := $(OBJS:.cc=.o)

all: main

main: main.cc $(OBJS)
	$(CXX) -Wall -Wextra -std=c++14 -O2 -Wshadow -o main main.cc $(OBJS)

build:
	mkdir -p build

$(BUILD_DIR)/%.o: $(SRCS_DIR)/%.cc build
	$(CXX) -Wall -Wextra -std=c++14 -O2 -Wshadow -c -o $@ $<

clean:
	rm -rf $(BUILD_DIR) main

.PHONY: all clean
