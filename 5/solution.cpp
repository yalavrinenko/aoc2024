//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"
#include <algorithm>
#include <charconv>
#include <fmt/color.h>
#include <fmt/core.h>
#include <fmt/xchar.h>
#include <ranges>
#include <regex>
#include <stdexcept>
#include <string>
#include <string_view>
#include <system_error>
#include <unordered_set>
#include <vector>

struct AOC_Output {
  unsigned long value{};
  AOC_Output() = default;
  explicit AOC_Output(auto v) : value{v} {}
  auto operator()() const { return value; }
};

struct AOC_Input {
  std::map<unsigned, std::unordered_set<unsigned>> rules;

  std::vector<std::vector<unsigned>> pages;
};

struct AOC_Reader {
  static std::vector<AOC_Input>
  from_string_vector(const std::vector<std::string> &lines) {
    AOC_Input input;

    auto line = lines.begin();
    while (!line->empty()) {
      auto after_before = *line | vw::split('|') | as_vector;
      auto before = as_value<unsigned>(std::string_view(after_before[0]));
      auto after = as_value<unsigned>(std::string_view(after_before[1]));

      input.rules[before].insert(after);
      ++line;
    }

    ++line;
    while (line != lines.end()) {
      auto pages = *line | vw::split(',') | vw::transform([](auto e) {
        return as_value<unsigned>(std::string_view(e));
      }) | as_vector;
      input.pages.emplace_back(std::move(pages));
      ++line;
    }
    return {input};
  }
};

class AOCDaySolution
    : public AOCSolution<AOCDaySolution, AOC_Input, AOC_Reader> {
  friend AOCSolution;

  bool check_page_order(std::vector<unsigned> const &pages, const auto &rules) {
    bool is_correct = true;
    for (auto i = 0UL; i < pages.size() && is_correct; ++i) {
      auto ref = pages[i];
      if (rules.contains(ref)) {
        auto &rule = rules.at(ref);
        for (auto j = 0UL; j < pages.size() && is_correct; ++j) {
          if (rule.contains(pages[j]) && j < i) {
            is_correct = false;
          }
        }
      }
    }
    return is_correct;
  }

  AOC_Output part1(std::vector<AOC_Input> const &v) {
    auto in = v.front();

    auto middle_sum = 0UL;

    for (auto pages : in.pages) {
      bool is_correct = check_page_order(pages, in.rules);
      if (is_correct) {
        middle_sum += pages[pages.size() / 2];
      }
    }

    return AOC_Output{middle_sum};
  }

  void restore_order(std::vector<unsigned> &pages,
                     std::map<unsigned, std::unordered_set<unsigned>> &rules) {
    auto less = [&rules](auto lhs, auto rhs) {
      return rules.contains(lhs) && rules.at(lhs).contains(rhs);
    };
    std::ranges::sort(pages, less);
  }

  AOC_Output part2(std::vector<AOC_Input> const &v) {
    auto in = v.front();

    auto middle_sum = 0UL;

    for (auto pages : in.pages) {
      bool is_correct = check_page_order(pages, in.rules);
      if (!is_correct) {
        restore_order(pages, in.rules);
        middle_sum += pages[pages.size() / 2];
      }
    }

    return AOC_Output{middle_sum};
    return AOC_Output{};
  }
};

AOC_DAY_SOLUTION(AOCDaySolution);
