#include <cstdio>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <string_view>
#include <vector>

std::vector<int> parse_nums(std::string_view s) {
  std::vector<int> nums;

  size_t from = 0;
  size_t end = s.find(" ");
  while (end != std::string::npos) {
    std::string num_str = std::string(s.substr(from, end - from));
    if (num_str != "") {
      // std::cout << num_str << std::endl;
      nums.push_back(std::stoi(num_str));
    }
    from = end + 1;
    end = s.find(" ", from);
  }
  std::string last_str = std::string(s.substr(from));
  nums.push_back(std::stoi(last_str));

  return nums;
}

void part1() {
  std::ifstream input("input.txt");
  // std::ifstream input("test.txt");
  if (!input.is_open()) {
    std::cerr << "Failed to open input.txt" << std::endl;
    return;
  }

  std::string line;

  std::getline(input, line);
  int colon_pos = line.find(":");
  std::vector<int> time_vec =
      parse_nums(std::string_view(line.data() + colon_pos + 2));
  // for (int time : time_vec) {
  //   std::cout << time << std::endl;
  // }

  std::getline(input, line);
  colon_pos = line.find(":");
  std::vector<int> dist_vec =
      parse_nums(std::string_view(line.data() + colon_pos + 2));
  // for (int time : dist_vec) {
  //   std::cout << time << std::endl;
  // }

  int multiplied = -1;
  for (int i = 0; i < time_vec.size(); ++i) {
    int &time = time_vec[i];
    int &dist = dist_vec[i];

    int left_midpoint = time / 2;
    double midpoint = (double)time / 2;

    int left = left_midpoint;
    int right;
    if (time % 2 != 0) {
      right = left + 1;
    } else {
      right = left;
    }

    int ways = 0;
    // std::cout << left << " " << right << std::endl;
    while (left * right > dist) {
      ways++;
      if (left != right) {
        ways++;
      }
      if (left - 1 > 0 && right + 1 < time) {
        left--;
        right++;
      } else {
        break;
      }
    }
    if (ways > 0) {
      // std::cout << ways << std::endl;
      if (multiplied == -1) {
        multiplied = ways;
      } else {
        multiplied *= ways;
      }
    }
  }

  std::cout << multiplied << std::endl;
}

long parse_nums_part2(std::string_view s) {
  std::string num_str_final;
  size_t from = 0;
  size_t end = s.find(" ");
  while (end != std::string::npos) {
    std::string num_str = std::string(s.substr(from, end - from));
    if (num_str != "") {
      num_str_final += num_str;
    }
    from = end + 1;
    end = s.find(" ", from);
  }
  std::string last_str = std::string(s.substr(from));
  num_str_final += last_str;

  return std::stol(num_str_final);
}

void part2() {
  std::ifstream input("input.txt");
  // std::ifstream input("test.txt");
  if (!input.is_open()) {
    std::cerr << "Failed to open input.txt" << std::endl;
    return;
  }

  std::string line;

  std::getline(input, line);
  int colon_pos = line.find(":");
  long time = parse_nums_part2(std::string_view(line.data() + colon_pos + 2));

  std::getline(input, line);
  colon_pos = line.find(":");
  long dist = parse_nums_part2(std::string_view(line.data() + colon_pos + 2));

  long left = 0;
  long right = time / 2;
  while (left < right) {
    long mid = left + (right - left) / 2;
    // std::cout << left << " " << mid << " " << right << std::endl;
    long counterpart = time - mid;
    if (mid * counterpart >= dist) {
      right = mid - 1;
    } else {
      left = mid + 1;
    }
  }
  long counterpart = time - left;
  // std::cout << left << " " << counterpart << std::endl;
  if (left * counterpart >= dist) {
    std::cout << counterpart - left + 1 << std::endl;
  } else {
    left++;
    counterpart = time - left;
    std::cout << counterpart - left + 1 << std::endl;
  }
}

int main(int argc, char *argv[]) {
  part2();
  return 0;
}
