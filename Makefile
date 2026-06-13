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

run: neptune
	./neptune

-include $(deps)

.PHONY: clean run
clean:
	rm -f $(obj) $(deps) neptune


