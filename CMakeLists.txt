cmake_minimum_required(VERSION 3.22)
project(neptunecrypt LANGUAGES CXX)

# 1. Detect the Operating System (Linux-only rule)
if(NOT CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(FATAL_ERROR "ERROR: This program is strictly for Linux OS. Configuration aborted.")
endif()

# 2. Enforce C++23 Standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 3. Global Compiler Flags
add_compile_options(-Wall -Wextra)

# 4. Find Dependencies
find_package(Threads REQUIRED)

# Locate cryptopp
find_library(CRYPTOPP_LIB NAMES cryptopp HINTS /usr/local/lib)
if(NOT CRYPTOPP_LIB)
    message(FATAL_ERROR "ERROR: cryptopp library not found. Please install cryptopp-modern.")
endif()

# 5. Collect Source Files
file(GLOB CIPHER_SRCS "src/cipher/*.cpp")
file(GLOB PASSWORD_SRCS "src/password/*.cpp")
file(GLOB DRIVER_SRCS "src/driverProgram/*.cpp")

set(ALL_SRCS ${CIPHER_SRCS} ${PASSWORD_SRCS} ${DRIVER_SRCS})

# 6. Define the Executable Target
add_executable(neptune ${ALL_SRCS})

# 7. Link Libraries
target_link_libraries(neptune PRIVATE ${CRYPTOPP_LIB} Threads::Threads)

# 8. Installation Rule
include(GNUInstallDirs)
# CMAKE_INSTALL_BINDIR defaults to "bin" (evaluates to /usr/local/bin by default)
install(TARGETS neptune DESTINATION ${CMAKE_INSTALL_BINDIR})
