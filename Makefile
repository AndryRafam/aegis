ccsrc = $(wildcard simpleCipher/*.cpp) \
		$(wildcard password/*.cpp) \
		$(wildcard driverProgram/*.cpp)
		
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++23 -MMD -MP

obj = $(ccsrc:.cpp=.o)
deps = $(obj:.o=.d)

LDFLAGS = -lcryptopp

neptune: $(obj)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

-include $(deps)

.PHONY: clean
clean:
	rm -f $(obj) $(deps) neptune
