#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <ostream>
#include <string>
#include <string_view>

void part1() {
  std::ifstream input("input.txt");
  if (!input.is_open()) {
    std::cerr << "Failed to open input.txt" << std::endl;
    return;
  }

  int sum = 0;
  std::string line;

  while (std::getline(input, line)) {
    int first = -1, last = -1;
    int firstIdx = -1;
    for (int i = 0; i < line.length(); ++i) {
      char &c = line[i];
      int num = c - '0';
      if (num < 0 || num > 9) {
        continue;
      }
      first = num;
      firstIdx = i;
      break;
    }
    for (int i = line.length() - 1; i >= firstIdx; --i) {
      char &c = line[i];
      int num = c - '0';
      if (num < 0 || num > 9) {
        continue;
      }
      last = num;
      break;
    }
    int num = first * 10 + last;
    sum += num;
  }

  input.close();

  std::cout << sum << std::endl;

  return;
}

int parseString(std::string s, int at) {
  static std::map<std::string, int> mapper = {
      {"one", 1}, {"two", 2},   {"three", 3}, {"four", 4}, {"five", 5},
      {"six", 6}, {"seven", 7}, {"eight", 8}, {"nine", 9},
  };

  std::string_view part =
      std::string_view(s.data() + at, std::min<int>(5, s.length() - at));

  for (auto it = mapper.begin(); it != mapper.end(); ++it) {
    if (part.compare(0, it->first.length(), it->first) == 0) {
      return it->second;
    }
  }

  // Try number parsing
  int num = s[at] - '0';
  if (num >= 0 && num <= 9) {
    return num;
  }

  return -1;
}

void part2() {
  std::ifstream input("input.txt");
  if (!input.is_open()) {
    std::cerr << "Failed to open input.txt" << std::endl;
    return;
  }

  int sum = 0;
  std::string line;

  while (std::getline(input, line)) {
    int first = -1, last = -1;
    for (int i = 0; i < line.length(); ++i) {
      int parsed = parseString(line, i);
      if (parsed != -1) {
        first = parsed;
        break;
      }
    }
    for (int i = line.length() - 1; i >= 0; --i) {
      int parsed = parseString(line, i);
      if (parsed != -1) {
        last = parsed;
        break;
      }
    }
    int num = first * 10 + last;
    sum += num;
  }

  input.close();

  std::cout << sum << std::endl;

  return;
}

int main(int argc, char *argv[]) {
  part2();
  return 0;
}
