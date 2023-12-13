#include <fstream>
#include <iostream>
#include <map>
#include <ostream>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

void part1() {
  std::ifstream input("input.txt");
  // std::ifstream input("test.txt");
  if (!input.is_open()) {
    std::cerr << "Failed to open input.txt" << std::endl;
    return;
  }

  int sum = 0;
  std::string line;

  std::map<int, std::pair<int, bool>> number_map;
  int id_count = 0;
  std::vector<std::vector<int>> ids;
  std::vector<std::pair<int, int>> found_symbols;

  int i = 0;
  while (std::getline(input, line)) {
    std::vector<int> row;
    for (int j = 0; j < line.length(); j++) {
      row.push_back(-1);
    }

    int from = -1, end = -1;
    for (int j = 0; j < line.length(); j++) {
      const char &c = line[j];
      int num = c - '0';
      bool is_number = num >= 0 && num <= 9;
      if (c == '.' || !is_number) {
        if (from != -1) {
          int length = end - from + 1;
          number_map[id_count] =
              std::make_pair(std::stoi(line.substr(from, length)), false);

          for (int k = from; k <= end; k++) {
            row[k] = id_count;
          }

          id_count++;
          from = -1;
          end = -1;
        }
        if (c != '.' && !is_number) {
          found_symbols.push_back(std::make_pair(i, j));
        }
        continue;
      }

      // Now it's a number.
      if (from == -1) {
        from = j;
      }
      end = j;
    }

    if (from != -1) {
      end = line.length() - 1;
      int length = end - from + 1;
      number_map[id_count] =
          std::make_pair(std::stoi(line.substr(from, length)), false);

      for (int k = from; k <= end; k++) {
        row[k] = id_count;
      }

      id_count++;
    }

    ids.push_back(row);
    i++;
  }

  // for (const auto& p : number_map) {
  //   std::cout << p.first << " - " << p.second << std::endl;
  // }

  // for (const auto& p : found_symbols) {
  //   std::cout << "symbol: " << p.first << " - " << p.second << std::endl;
  // }

  static std::vector<std::pair<int, int>> arounds = {
      {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 0},
      {0, 1},   {1, -1}, {1, 0},  {1, 1},
  };
  int max_col = ids[0].size() - 1;
  int max_row = ids.size() - 1;
  for (auto const &symbol_coord : found_symbols) {
    for (auto const &offset : arounds) {
      int row = symbol_coord.first + offset.first;
      int col = symbol_coord.second + offset.second;
      if (row < 0 || row > max_row || col < 0 || col > max_col) {
        continue;
      }
      int id = ids[row][col];
      if (id != -1) {
        // std::cout << "ticking " << id << "; row: " << row << "; col: " << col
        //           << std::endl;
        number_map[id].second = true;
      }
    }
  }

  for (const auto &p : number_map) {
    auto const &number_info = p.second;
    // std::cout << number_info.first << " - " << number_info.second <<
    // std::endl;
    if (number_info.second) {
      sum += number_info.first;
    }
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

  std::map<int, std::pair<int, bool>> number_map;
  int id_count = 0;
  std::vector<std::vector<int>> ids;
  std::vector<std::pair<int, int>> found_gears;

  int i = 0;
  while (std::getline(input, line)) {
    std::vector<int> row;
    for (int j = 0; j < line.length(); j++) {
      row.push_back(-1);
    }

    int from = -1, end = -1;
    for (int j = 0; j < line.length(); j++) {
      const char &c = line[j];
      int num = c - '0';
      bool is_number = num >= 0 && num <= 9;
      if (c == '.' || !is_number) {
        if (from != -1) {
          int length = end - from + 1;
          number_map[id_count] =
              std::make_pair(std::stoi(line.substr(from, length)), false);

          for (int k = from; k <= end; k++) {
            row[k] = id_count;
          }

          id_count++;
          from = -1;
          end = -1;
        }
        if (c == '*') {
          found_gears.push_back(std::make_pair(i, j));
        }
        continue;
      }

      // Now it's a number.
      if (from == -1) {
        from = j;
      }
      end = j;
    }

    if (from != -1) {
      end = line.length() - 1;
      int length = end - from + 1;
      number_map[id_count] =
          std::make_pair(std::stoi(line.substr(from, length)), false);

      for (int k = from; k <= end; k++) {
        row[k] = id_count;
      }

      id_count++;
    }

    ids.push_back(row);
    i++;
  }

  // for (const auto& p : number_map) {
  //   std::cout << p.first << " - " << p.second << std::endl;
  // }

  // for (const auto& p : found_symbols) {
  //   std::cout << "symbol: " << p.first << " - " << p.second << std::endl;
  // }

  static std::vector<std::pair<int, int>> arounds = {
      {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 0},
      {0, 1},   {1, -1}, {1, 0},  {1, 1},
  };
  int max_col = ids[0].size() - 1;
  int max_row = ids.size() - 1;
  for (auto const &symbol_coord : found_gears) {
    std::unordered_set<int> counted_ids;
    for (auto const &offset : arounds) {
      int row = symbol_coord.first + offset.first;
      int col = symbol_coord.second + offset.second;
      if (row < 0 || row > max_row || col < 0 || col > max_col) {
        continue;
      }
      int id = ids[row][col];
      if (id != -1) {
        // std::cout << "ticking " << id << "; row: " << row << "; col: " << col
        //           << std::endl;
        counted_ids.insert(id);
      }
    }
    if (counted_ids.size() == 2) {
      int multiplied = 1;
      for (auto const &id : counted_ids) {
        multiplied *= number_map[id].first;
      }
      sum += multiplied;
    }
  }

  // for (const auto &p : number_map) {
  //   auto const &number_info = p.second;
  //   // std::cout << number_info.first << " - " << number_info.second <<
  //   // std::endl;
  //   if (number_info.second) {
  //     sum += number_info.first;
  //   }
  // }

  std::cout << sum;
}

int main(int argc, char *argv[]) {
  part2();
  return 0;
}
