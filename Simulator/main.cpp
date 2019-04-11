#include <fstream>

#include "libs/simulator.hpp"

auto main(int argc, char **argv) -> int {

  for (int idx = 1; idx < argc; ++idx) {
    std::array<uint16_t, 0xFFFF> memory{};

    std::string file_name = argv[idx];
    // std::cout << "FILE IS " << file_name << '\n';
    std::ifstream file(file_name, std::ios::binary);

    std::vector<unsigned char> buff(std::istreambuf_iterator<char>(file), {});

    for (int i = 0; i < buff.size(); i++) {
      // std::cout << "VALUE IS " << static_cast<int>(buff[i * 2]) << " AND  "
      //           << static_cast<int>(buff[i * 2 + 1]) << '\n';
      memory[i] = (buff[i * 2] << 8) | buff[i * 2 + 1];
    }

    Simulator sim{};
    sim.fill(memory);
    sim.run();
  }

  return 0;
}