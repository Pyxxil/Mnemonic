cmake_minimum_required(VERSION 3.8)

set (ASSEMBLER_INCLUDE_FILES
    "${SIMULATOR_INCLUDE_FILES}"
    "${CMAKE_CURRENT_SOURCE_DIR}/Assembler.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Lexer/File.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Lexer/Line.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Lexer/Token.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Lexer/Tokenizer.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Lexer/Tokens.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Lexer/Tokens/Add.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Lexer/Tokens/Fill.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Lexer/Tokens/In.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Lexer/Tokens/Out.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Lexer/Tokens/End.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Lexer/Tokens/Halt.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Lexer/Tokens/Decimal.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Lexer/Tokens/Jump.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Lexer/Tokens/Register.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Lexer/Tokens/Orig.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Lexer/Tokens/Label.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Lexer/Tokens/Compare.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Lexer/Tokens/Clear.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Lexer/Tokens/Decrement.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Lexer/Tokens/Increment.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Lexer/Tokens/Jumpeq.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Lexer/Tokens/Jumpgt.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Lexer/Tokens/Jumplt.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Lexer/Tokens/Jumpneq.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Lexer/Tokens/Store.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Lexer/Tokens/Load.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/cxxopts.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Console.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Debug.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Algorithm.hpp"
    PARENT_SCOPE
)

set (ASSEMBLER_SOURCE_FILES
    "${SIMULATOR_SOURCE_FILES}"
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Lexer/File.cpp"
    PARENT_SCOPE
)