//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"
#include <charconv>
#include <fmt/color.h>
#include <fmt/core.h>
#include <ranges>
#include <regex>
#include <stdexcept>
#include <string>
#include <string_view>
#include <system_error>
#include <utility>
#include <vector>

struct AOC_Output {
  unsigned long value{};
  AOC_Output() = default;
  explicit AOC_Output(auto v) : value{v} {}
  auto operator()() const { return value; }
};

struct AOC_Input {
  std::string line;
};

struct AOC_Reader {
  static std::vector<AOC_Input>
  from_string_vector(const std::vector<std::string> &lines) {
    return lines | vw::transform([](auto &e) { return AOC_Input{.line = e}; }) |
           as_vector;
  }
};

template <typename T> auto str_to_int(std::string_view s) {
  T value{};
  if (std::from_chars(s.begin(), s.end(), value).ec == std::errc{}) {
    return value;
  } else {
    throw std::runtime_error{"Fail to parse int."};
  }
}

class AOCDaySolution
    : public AOCSolution<AOCDaySolution, AOC_Input, AOC_Reader> {
  friend AOCSolution;

  AOC_Output part1(std::vector<AOC_Input> const &v) {
    auto result = 0UL;
    for (auto e : v) {
      std::regex rexpr("mul\\((\\d{1,3}),(\\d{1,3})\\)");

      auto rbegin = std::sregex_iterator(e.line.begin(), e.line.end(), rexpr);
      auto rend = std::sregex_iterator();

      for (auto it = rbegin; it != rend; ++it) {
        auto match = *it;

        result += str_to_int<long long>(match[1].str()) *
                  str_to_int<long long>(match[2].str());
      }
    }

    return AOC_Output{result};
  }

  AOC_Output part2(std::vector<AOC_Input> const &v) {
    bool enable = true;
    auto result = 0ULL;
    for (auto e : v) {
      std::regex rexpr(
          "(mul\\((\\d{1,3}),(\\d{1,3})\\))|(do\\(\\))|(don't\\(\\))");

      auto rbegin = std::sregex_iterator(e.line.begin(), e.line.end(), rexpr);
      auto rend = std::sregex_iterator();

      for (auto it = rbegin; it != rend; ++it) {
        auto match = *it;
        if (match.str() == "do()") {
          enable = true;
        } else if (match.str() == "don't()") {
          enable = false;
        } else {
          if (enable) {
            result += str_to_int<long long>(match[2].str()) *
                      str_to_int<long long>(match[3].str());
          }
        }
      }
    }

    return AOC_Output{result};
  }
};

AOC_DAY_SOLUTION(AOCDaySolution);
