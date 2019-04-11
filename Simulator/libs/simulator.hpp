#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include <array>
#include <iomanip>
#include <iostream>
#include <vector>

class Memory {
private:
  std::array<uint16_t, 0xFFFF> memory{};

public:
  constexpr Memory() = default;

  constexpr uint16_t &operator()(uint16_t X) { return memory[X]; }
};

struct ConditionCode {
  bool GT{false};
  bool EQ{false};
  bool LT{false};

  constexpr ConditionCode() = default;
  constexpr ConditionCode(uint16_t con, uint16_t r)
    : GT{con > r}, EQ{con == r}, LT{con < r} { }
};

class Simulator {
  enum OPCODE {
    LOAD = 0x0000,
    STORE = 0x1000,
    CLEAR = 0x2000,
    ADD = 0x3000,
    INC = 0x4000,
    SUB = 0x5000,
    DEC = 0x6000,
    COMP = 0x7000,
    JUMP = 0x8000,
    JGT = 0x9000,
    JEQ = 0xA000,
    JLT = 0xB000,
    JNEQ = 0xC000,
    IN = 0xD000,
    OUT = 0xE000,
    HALT = 0xF000,
  };

  using Instruction = uint16_t;

private:
  Memory CON{};
  uint16_t R{0};
  bool is_halted{false};
  uint16_t PC{0};
  ConditionCode codes{};

  constexpr auto next_instruction() -> Instruction {
    Instruction instr{CON(PC)};
    increment_program_counter(PC + 1);
    return instr;
  }

  constexpr void increment_program_counter(int16_t newPC) { PC = newPC; }

public:
  constexpr Simulator() = default;

  constexpr void run() {
    while (!halted()) {
      const auto instruction = next_instruction();

      const uint16_t X = static_cast<int16_t>(
                             static_cast<int16_t>(instruction & 0x0FFF) << 4) >>
                         4;

      switch (instruction & 0xF000) {
      case LOAD: {
        R = CON(X);
        break;
      }
      case STORE: {
        CON(X) = R;
        break;
      }
      case CLEAR: {
        CON(X) = 0;
        break;
      }
      case ADD: {
        R += CON(X);
        break;
      }
      case INC: {
        CON(X) += 1;
        break;
      }
      case SUB: {
        R -= CON(X);
        break;
      }
      case DEC: {
        CON(X) -= 1;
        break;
      }
      case COMP: {
        codes = ConditionCode(CON(X), R);
        break;
      }
      case JUMP: {
        increment_program_counter(X);
        break;
      }
      case JGT: {
        if (!codes.GT) {
          break;
        }

        increment_program_counter(X);
        break;
      }
      case JEQ: {
        if (!codes.EQ) {
          break;
        }

        increment_program_counter(X);
        break;
      }
      case JLT: {
        if (!codes.LT) {
          break;
        }

        increment_program_counter(X);
        break;
      }
      case JNEQ: {
        if (codes.EQ) {
          break;
        }

        increment_program_counter(X);
        break;
      }
      case IN: {
        int16_t val{0};
        std::cout << "(Input a number) => ";

        while (!(std::cin >> val)) {
          std::cout << "(INVALID! Input a number) => ";
          std::cin.clear();
          std::cin.ignore();
        }

        std::cin.ignore();
        CON(X) = val;

        break;
      }
      case OUT: {
        std::cout << "(Output        ) => " << static_cast<int16_t>(CON(X)) << '\n';
        break;
      }
      case HALT: {
        is_halted = true;
        break;
      }
      }
    }
  }

  constexpr bool halted() const { return is_halted; }

  constexpr void fill(std::array<uint16_t, 0xFFFF> contents) {
    for (int i = 0; i < contents.size(); i++) {
      CON(i) = contents[i];
    }
  }
};

#endif // SIMULATOR_HPP
