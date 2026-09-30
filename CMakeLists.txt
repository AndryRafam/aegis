cmake_minimum_required(VERSION 3.22)
project(aegis LANGUAGES CXX)

# 1. Detect the Operating System (Linux-only rule)
if(NOT CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(FATAL_ERROR "ERROR: This program is strictly for Linux OS. Configuration aborted.")
endif()

# 2. C++17 Standard or Higher
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)
set(CMAKE_CXX_EXTENSIONS OFF)

# 3. Global Compiler Flags
add_compile_options(-Wall -Wextra)

# 4. Find Dependencies
find_package(Threads REQUIRED)

# 5. Locate system-installed FTXUI
find_package(ftxui QUIET)

# if can't locate ftxui
if(NOT ftxui_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        ftxui
        GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
        GIT_TAG v5.0.0
    )
    FetchContent_MakeAvailable(ftxui)
endif()

# 6. Locate cryptopp
find_library(CRYPTOPP_LIB NAMES cryptopp HINTS /usr/local/lib)
if(NOT CRYPTOPP_LIB)
    message(FATAL_ERROR "ERROR: cryptopp library not found. Please install cryptopp-modern.")
endif()

# 7. Collect Source Files
file(GLOB CIPHER_SRCS "src/cipher/*.cpp")
file(GLOB DEC_SRCS "src/aegisdecryption/*.cpp")
file(GLOB ENC_SRCS "src/aegisencryption/*.cpp")
file(GLOB HELP_SRCS "src/helper_function/*.cpp")
file(GLOB PASSWORD_SRCS "src/password/*.cpp")
file(GLOB DRIVER_SRCS "src/driverProgram/*.cpp")

set(ALL_SRCS ${CIPHER_SRCS} ${DEC_SRCS} ${ENC_SRCS} ${HELP_SRCS} ${PASSWORD_SRCS} ${DRIVER_SRCS})

# 8. Define the Executable Target
add_executable(aegis ${ALL_SRCS})

# 9. Link Libraries
target_link_libraries(aegis PRIVATE
    ${CRYPTOPP_LIB}
    Threads::Threads
    ftxui::screen
    ftxui::dom
    ftxui::component
)

# 10. Installation Rule
include(GNUInstallDirs)
# CMAKE_INSTALL_BINDIR defaults to "bin" (evaluates to /usr/local/bin by default)
install(TARGETS aegis DESTINATION ${CMAKE_INSTALL_BINDIR})
