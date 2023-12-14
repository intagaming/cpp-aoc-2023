#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <unordered_set>
#include <vector>

std::unordered_set<int> parse_nums(std::string_view s) {
  std::unordered_set<int> nums;

  size_t from = 0;
  size_t end = s.find(" ");
  while (end != std::string::npos) {
    if (from != end) {
      std::string num_str = std::string(s.substr(from, end - from));
      int num = std::stoi(num_str);
      nums.insert(num);
    }

    from = end + 1;
    end = s.find(" ", from);
  }

  std::string last_str = std::string(s.substr(from));
  int num = std::stoi(last_str);
  nums.insert(num);

  return nums;
}

void part1() {
  std::ifstream input("input.txt");
  // std::ifstream input("test.txt");
  if (!input.is_open()) {
    std::cerr << "Failed to open input.txt" << std::endl;
    return;
  }

  int sum = 0;
  std::string line;

  while (std::getline(input, line)) {
    int colon_pos = line.find(':');
    int winnings_from = colon_pos + 2;
    std::string_view content = std::string_view(line.data() + winnings_from);

    int pipe_pos = line.find('|');
    int winnings_end = pipe_pos - 2;
    int nums_from = pipe_pos + 2;
    std::string_view winnings_str = std::string_view(
        line.data() + winnings_from, winnings_end - winnings_from + 1);
    std::string_view nums_str = std::string_view(line.data() + nums_from);

    std::unordered_set<int> winnings = parse_nums(winnings_str);
    std::unordered_set<int> nums = parse_nums(nums_str);

    int count = 0;
    for (int n : nums) {
      if (winnings.find(n) != winnings.end()) {
        count++;
      }
    }

    if (count == 0) {
      continue;
    }

    sum += 1 << (count - 1);
  }

  std::cout << sum;
}

void part2() {
  std::ifstream input("input.txt");
  // std::ifstream input("test.txt");
  if (!input.is_open()) {
    std::cerr << "Failed to open input.txt" << std::endl;
    return;
  }

  int sum = 0;
  std::string line;

  std::vector<int> count_vec;

  int line_idx = 0;
  while (std::getline(input, line)) {
    if (count_vec.size() < line_idx + 1) {
      while (count_vec.size() < line_idx + 1) {
        count_vec.push_back(1);
      }
    } else {
      count_vec[line_idx]++;
    }

    int colon_pos = line.find(':');
    int winnings_from = colon_pos + 2;
    std::string_view content = std::string_view(line.data() + winnings_from);

    int pipe_pos = line.find('|');
    int winnings_end = pipe_pos - 2;
    int nums_from = pipe_pos + 2;
    std::string_view winnings_str = std::string_view(
        line.data() + winnings_from, winnings_end - winnings_from + 1);
    std::string_view nums_str = std::string_view(line.data() + nums_from);

    std::unordered_set<int> winnings = parse_nums(winnings_str);
    std::unordered_set<int> nums = parse_nums(nums_str);

    int count = 0;
    for (int n : nums) {
      if (winnings.find(n) != winnings.end()) {
        count++;
      }
    }

    if (count != 0) {
      // std::cout << "line: " << line_idx << " num count " << count << std::endl;

      int inc_from = line_idx + 1;
      int inc_to = inc_from + count - 1;
      while (count_vec.size() < inc_to + 1) {
        count_vec.push_back(0); // Will be incremented when the line comes by
      }
      for (int i = inc_from; i <= inc_to; i++) {
        // std::cout << "inc " << i << "by " << count_vec[line_idx] << std::endl;
        count_vec[i] += count_vec[line_idx];
      }
    }
    line_idx++;
    // std::cout << "end line" << std::endl;
  }

  // std::cout << "line_idx " << line_idx << std::endl;
  count_vec.resize(line_idx);

  for (int count : count_vec) {
    // std::cout << count << std::endl;
    sum += count;
  }

  std::cout << sum << std::endl;
}

int main(int argc, char *argv[]) {
  part2();
  return 0;
}
