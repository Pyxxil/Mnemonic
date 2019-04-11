#include "Assembler.hpp"

auto main(int argc, char **argv) -> int {
  Assembler assembler(argc, argv);
  return assembler.assemble();
}