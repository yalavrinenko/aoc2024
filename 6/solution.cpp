//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"
#include <algorithm>
#include <charconv>
#include <fmt/color.h>
#include <fmt/core.h>
#include <fmt/xchar.h>
#include <numeric>
#include <ranges>
#include <regex>
#include <stdexcept>
#include <string>
#include <string_view>
#include <system_error>
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

  bool has_loop(int srow, int scol, std::vector<std::string> const &M) {
    auto [x, y] = std::tie(srow, scol);
    std::pair dir{-1, 0};
    bool reach_end = false;

    std::vector<std::vector<int>> V(M.size(),
                                    std::vector<int>(M.front().size(), 0));
    std::vector<std::vector<std::pair<int, int>>> D(
        M.size(), std::vector<std::pair<int, int>>(M.front().size(),
                                                   std::pair<int, int>{}));

    while (!reach_end) {
      if (x < 0 || x >= static_cast<int>(M.size()) || y < 0 ||
          y >= static_cast<int>(M.front().size())) {
        reach_end = true;
        continue;
      }

      if (M[x][y] == '.' || M[x][y] == '^') {
        if (V[x][y] && D[x][y] == dir) {
          return true;
        }

        V[x][y] = true;
        D[x][y] = dir;
      } else {
        x -= dir.first;
        y -= dir.second;

        dir = {dir.second, -dir.first};
      }

      x += dir.first;
      y += dir.second;
    }

    return false;
  }

  AOC_Output part1(std::vector<AOC_Input> const &v) {
    auto &M = v.front().map;
    auto srow = 0;
    auto scol = 0;
    for (auto i : vw::iota(0UL, M.size())) {
      for (auto j : vw::iota(0UL, M[i].size())) {
        if (M[i][j] == '^') {
          srow = i;
          scol = j;
        }
      }
    }

    auto [x, y] = std::tie(srow, scol);
    std::pair dir{-1, 0};
    bool reach_end = false;

    std::vector<std::vector<int>> V(M.size(),
                                    std::vector<int>(M.front().size(), 0));

    while (!reach_end) {
      if (x < 0 || x >= static_cast<int>(M.size()) || y < 0 ||
          y >= static_cast<int>(M.front().size())) {
        reach_end = true;
        continue;
      }

      if (M[x][y] == '.' || M[x][y] == '^') {
        V[x][y] = true;
      } else {
        x -= dir.first;
        y -= dir.second;

        dir = {dir.second, -dir.first};
      }

      x += dir.first;
      y += dir.second;
    }

    unsigned count =
        std::accumulate(V.begin(), V.end(), 0, [](auto sum, auto &e) {
          return sum += std::ranges::count(e, 1);
        });

    return AOC_Output{count};
  }

  AOC_Output part2(std::vector<AOC_Input> const &v) {
    auto &M = v.front().map;
    auto srow = 0;
    auto scol = 0;
    for (auto i : vw::iota(0UL, M.size())) {
      for (auto j : vw::iota(0UL, M[i].size())) {
        if (M[i][j] == '^') {
          srow = i;
          scol = j;
        }
      }
    }

    auto count = 0UL;

    for (auto i : vw::iota(0UL, M.size())) {
      for (auto j : vw::iota(0UL, M[i].size())) {
        if (M[i][j] == '.') {
          auto CM = M;
          CM[i][j] = '#';
          count += has_loop(srow, scol, CM);
        }
      }
    }

    return AOC_Output{count};
  }
};

AOC_DAY_SOLUTION(AOCDaySolution);
