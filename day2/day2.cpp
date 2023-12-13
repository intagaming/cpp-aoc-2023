#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <string_view>
#include <vector>

std::vector<std::string> split(std::string_view s, std::string_view delimiter) {
  std::vector<std::string> parts;

  size_t start_pos = 0;
  size_t end_pos = s.find(delimiter);

  while (end_pos != std::string::npos) {
    std::string token = std::string(s.substr(start_pos, end_pos - start_pos));
    parts.push_back(token);

    start_pos = end_pos + delimiter.length();
    end_pos = s.find(delimiter, start_pos);
  }

  std::string lastToken = std::string(s.substr(start_pos, end_pos));
  parts.push_back(lastToken);

  return parts;
}

void part1() {
  static const int LIMIT_RED = 12;
  static const int LIMIT_GREEN = 13;
  static const int LIMIT_BLUE = 14;

  std::ifstream input("input.txt");
  if (!input.is_open()) {
    std::cerr << "Failed to open input.txt" << std::endl;
    return;
  }

  int sum = 0;
  std::string line;

  while (std::getline(input, line)) {
    int colon_idx = line.find(":", 6);
    int id = std::stoi(line.substr(5, colon_idx - 5));

    std::string round_str = line.substr(colon_idx + 2);
    std::vector<std::string> rounds = split(round_str, "; ");

    bool valid = true;

    for (std::string const &round : rounds) {
      std::vector<std::string> cubes = split(round, ", ");
      for (std::string const &cube : cubes) {
        std::vector<std::string> parts = split(cube, " ");
        if ((parts[1] == "red" && std::stoi(parts[0]) > LIMIT_RED) ||
            (parts[1] == "green" && std::stoi(parts[0]) > LIMIT_GREEN) ||
            (parts[1] == "blue" && std::stoi(parts[0]) > LIMIT_BLUE)) {
          valid = false;
          goto out;
        }
      }
    }
  out:
    if (valid) {
      sum += id;
    }
  }

  std::cout << sum;
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
    int colon_idx = line.find(":", 6);
    int id = std::stoi(line.substr(5, colon_idx - 5));

    std::string round_str = line.substr(colon_idx + 2);
    std::vector<std::string> rounds = split(round_str, "; ");

    int max_red = 0, max_green = 0, max_blue = 0;

    for (std::string const &round : rounds) {
      std::vector<std::string> cubes = split(round, ", ");
      for (std::string const &cube : cubes) {
        std::vector<std::string> parts = split(cube, " ");
        if (parts[1] == "red") {
          max_red = std::max(max_red, std::stoi(parts[0]));
        } else if (parts[1] == "green") {
          max_green = std::max(max_green, std::stoi(parts[0]));
        } else if (parts[1] == "blue") {
          max_blue = std::max(max_blue, std::stoi(parts[0]));
        }
      }
    }

    sum += max_red * max_green * max_blue;
  }

  std::cout << sum;
}

int main(int argc, char *argv[]) {
  part2();
  return 0;
}
