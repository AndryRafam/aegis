# To Install: sudo make install
# To Uninstall: sudo make uninstall

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
		
# uninstall
uninstall: neptune
	rm -f /usr/local/bin/neptune

-include $(deps)

.PHONY: clean install uninstall
clean:
	rm -f $(obj) $(deps) neptune


