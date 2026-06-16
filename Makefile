# 1. Detect the Operating System
OS := $(shell uname -s)

# 2. Linux-only rule
ifneq ($(OS), Linux)
$(error ERROR: This program is strictly for Linux OS). Compilation aborted.
endif

# 3. Super user only rule
ifneq ($(shell id -u), 0)
$(error ERROR: You must run 'sudo make' to build and install this project.)
endif

.DEFAULT_GOAL := install

ccsrc = $(wildcard src/simpleCipher/*.cpp) \
		$(wildcard src/password/*.cpp) \
		$(wildcard src/driverProgram/*.cpp) 
		
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++23 -MMD -MP

obj = $(ccsrc:.cpp=.o)
deps = $(obj:.o=.d)

LDFLAGS = -lcryptopp

neptune: $(obj)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)


# install
install: neptune
	mkdir -p /usr/local/bin
	cp -f neptune /usr/local/bin/neptune

		
-include $(deps)

.PHONY: purge
purge:
	rm -f /usr/local/bin/neptune
	rm -f $(obj) $(deps) neptune


