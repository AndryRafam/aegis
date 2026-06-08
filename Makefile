ccsrc = $(wildcard fileCipher/*.hpp) \
		$(wildcard fileCipher/*.cpp) \
		$(wildcard password/*.hpp) \
		$(wildcard password/*.cpp) \
		$(wildcard driverProgram/*.cpp)
		
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++23

obj = $(ccsrc:.cpp=.o)
deps = $(obj:.o=.d)

LDFLAGS = -lcryptopp

neptune: $(obj)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
