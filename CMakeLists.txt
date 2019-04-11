cmake_minimum_required(VERSION 3.8)
project(Mnemonic)

set(CMAKE_CXX_STANDARD 17)

set(DEBUG_FLAGS ${COMPILE_FLAGS} -g -g3 -pg -pedantic)

if (NOT WIN32)
  #set(DEBUG_FLAGS ${DEBUG_FLAGS} -Wall -Werror -Wextra -Wshadow -Wnon-virtual-dtor -Wconversion -Wno-error=conversion)
  #set(DEBUG_FLAGS ${DEBUG_FLAGS} -Wold-style-cast -Wcast-align -Wunused -Woverloaded-virtual)
  #set(DEBUG_FLAGS ${DEBUG_FLAGS} -Wsign-conversion -Wno-error=shadow -Wno-error=missing-braces -Wno-error=sign-conversion)
endif ()

set(RELEASE_FLAGS ${COMPILE_FLAGS} -flto)

#set(CMAKE_VERBOSE_MAKEFILE True)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

include(FetchContent)
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
)

FetchContent_GetProperties(fmt)
if (NOT fmt_POPULATED)
  FetchContent_Populate(fmt)
  add_subdirectory(${fmt_SOURCE_DIR} ${fmt_BINARY_DIR})
endif ()

add_subdirectory(Assembler)
add_subdirectory(Simulator)

add_executable(si ${SIMULATOR_SOURCE_FILES})
target_compile_options(si PUBLIC "$<$<CONFIG:DEBUG>:${DEBUG_FLAGS}>")
target_compile_options(si PUBLIC "$<$<CONFIG:RELEASE>:${RELEASE_FLAGS}>")

add_executable(as ${ASSEMBLER_SOURCE_FILES})
include_directories(as ${fmt_SOURCE_DIR})
include_directories(as "${CMAKE_CURRENT_SOURCE_DIR}/Assembler/Lexer/Tokens" "${CMAKE_CURRENT_SOURCE_DIR}/Assembler/Lexer/")
target_compile_options(as PUBLIC "$<$<CONFIG:DEBUG>:${DEBUG_FLAGS}>")
target_compile_options(as PUBLIC "$<$<CONFIG:RELEASE>:${RELEASE_FLAGS}>")
target_link_libraries (as fmt)
