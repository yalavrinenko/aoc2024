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
  std::vector<std::string> map;
};

struct AOC_Reader {
  static std::vector<AOC_Input>
  from_string_vector(const std::vector<std::string> &lines) {
    return {{.map = lines}};
  }
};

class AOCDaySolution
    : public AOCSolution<AOCDaySolution, AOC_Input, AOC_Reader> {
  friend AOCSolution;

  using Text = std::vector<std::string>;
  static constexpr std::string_view pattern_a = "XMAS";
  static constexpr std::string_view pattern_b = "SAMX";

  std::string get_horizontal(unsigned i, unsigned j, Text const &M) {
    if (j + pattern_a.size() > M.front().size()) {
      return {};
    }
    return M[i].substr(j, pattern_a.size());
  }
  std::string get_vertical(unsigned row, unsigned col, Text const &M) {
    if (row + pattern_a.size() > M.size()) {
      return {};
    }
    std::string text{};
    for (auto i = row; i < row + pattern_a.size(); ++i) {
      text += M[i][col];
    }
    return text;
  }

  auto diagonals(unsigned row, unsigned col, Text const &M) {
    auto in_range = [&M](auto i, auto j) {
      return (0 <= i && i < M.size()) && (0 <= j && j < M.front().size());
    };

    std::string main{};
    std::string aux{};
    for (auto i : vw::iota(0UL, pattern_a.size())) {
      if (in_range(row + i, col + i)) {
        main += M[row + i][col + i];
      }

      if (in_range(row + i, col - i)) {
        aux += M[row + i][col - i];
      }
    }
    return std::pair{main, aux};
  }

  AOC_Output part1(std::vector<AOC_Input> const &v) {
    auto &T = v.front().map;
    auto count = 0UL;

    auto test = [](auto text) {
      return text == pattern_a || text == pattern_b;
    };

    for (auto i : vw::iota(0UL, T.size())) {
      for (auto j : vw::iota(0UL, T[i].size())) {
        auto [main, aux] = diagonals(i, j, T);
        auto horizontal = get_horizontal(i, j, T);
        auto vertical = get_vertical(i, j, T);

        // fmt::print("m:{} a:{} h:{} v:{}\n", main, aux, horizontal, vertical);

        count += test(main);
        count += test(aux);
        count += test(horizontal);
        count += test(vertical);
      }
    }
    return AOC_Output{count};
  }

  std::vector<std::vector<std::string>> patterns{
      {
          "M.S",
          ".A.",
          "M.S",
      },
      {
          "S.S",
          ".A.",
          "M.M",
      },
      {
          "M.M",
          ".A.",
          "S.S",
      },
      {
          "S.M",
          ".A.",
          "S.M",
      },
  };

  std::vector<std::string> get_pattern(unsigned row, unsigned col,
                                       Text const &T) {
    std::vector<std::string> pt;

    pt.emplace_back(T[row + 0].substr(col, 3));
    pt.emplace_back(T[row + 1].substr(col, 3));
    pt.emplace_back(T[row + 2].substr(col, 3));

    pt[0][1] = pt[1][0] = pt[1][2] = pt[2][1] = '.';
    return pt;
  }

  AOC_Output part2(std::vector<AOC_Input> const &v) {
    auto &T = v.front().map;
    auto count = 0UL;
    for (auto i : vw::iota(0UL, T.size() - 2)) {
      for (auto j : vw::iota(0UL, T.front().size() - 2)) {
        auto pt = get_pattern(i, j, T);

        for (auto golden : patterns) {
          if (golden == pt) {
            count++;
          }
        }
      }
    }
    return AOC_Output{count};
  }
};

AOC_DAY_SOLUTION(AOCDaySolution);
