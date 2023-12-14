#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <string_view>
#include <vector>

struct mapping {
  long dest_from;
  long source_from;
  long size;
};

std::vector<long> parse_nums(std::string_view s) {
  std::vector<long> nums;

  size_t from = 0;
  size_t end = s.find(" ");
  while (end != std::string::npos) {
    std::string num_str = std::string(s.substr(from, end - from));
    // std::cout << num_str << std::endl;
    nums.push_back(std::stol(num_str));
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

  std::vector<std::vector<mapping>> maps;

  std::string line;

  // Read seeds
  std::getline(input, line);
  int colon_pos = line.find(":");
  std::vector<long> seeds =
      parse_nums(std::string_view(line.data() + colon_pos + 2));
  // for (long seed : seeds) {
  //   std::cout << seed << std::endl;
  // }
  std::getline(input, line);

  // Now fetch maps
  bool header_detected = false;
  while (std::getline(input, line)) {
    if (!header_detected) {
      header_detected = true;
      maps.push_back(std::vector<mapping>());
      continue;
    }

    if (line.length() == 0) {
      header_detected = false;
      continue;
    }

    std::vector<mapping> &map_vec = maps[maps.size() - 1];

    std::vector<long> nums = parse_nums(std::string_view(line));
    map_vec.push_back({nums[0], nums[1], nums[2]});
  }

  // for (std::vector<mapping> map_vec : maps) {
  //   std::cout << "start" << std::endl;
  //   for (mapping m : map_vec) {
  //     std::cout << m.dest_from << " " << m.source_from << " " << m.size
  //               << std::endl;
  //   }
  //   std::cout << "end" << std::endl;
  // }

  // Mapping seed to loc
  std::vector<long> end_locs;
  for (long seed : seeds) {
    long current = seed;
    for (std::vector<mapping> map_vec : maps) {
      for (mapping m : map_vec) {
        if (current >= m.source_from && current <= m.source_from + m.size) {
          current = (current - m.source_from) + m.dest_from;
          break;
        }
      }
    }
    end_locs.push_back(current);
  }

  // for (long loc : end_locs) {
  //   std::cout << loc << std::endl;
  // }

  long const &min = *std::min_element(end_locs.begin(), end_locs.end());

  std::cout << min << std::endl;
}

bool check_if_can_reached_from_previous(
    std::vector<std::vector<mapping>> const &maps,
    std::vector<mapping> const &curr_vec_map, mapping const &target_m) {
  for (auto maps_it = maps.begin(); maps_it != maps.end(); ++maps_it) {
    std::vector<mapping> const &vec_map = *maps_it;
    if (&vec_map == &curr_vec_map) {
      return false;
    }

    // For each map in vec_map, check if it can reach target_m
    for (mapping const &m : vec_map) {
      long from = m.dest_from, to = m.dest_from + m.size;
      long target_from = target_m.source_from,
           target_to = target_m.source_from + m.size;
      if ((to >= target_from && to <= target_to) || (from <= target_to)) {
        return true;
      }
    }
  }

  return true;
}

void trim_maps(std::vector<std::vector<mapping>> &maps, long seed, long end) {
  for (auto it = maps.begin(); it != maps.end();) {
    std::vector<mapping> &map_vec = *it;
    for (auto it = map_vec.begin(); it != map_vec.end();) {
      mapping &m = *it;
      // std::cout << min_loc << " " << m.dest_from << std::endl;
      // If can reach without going through previous maps
      bool can_straight_reach_later = seed >= m.source_from ||
                                      end >= m.source_from ||
                                      seed <= m.source_from + m.size;
      if (!can_straight_reach_later) {
        // Check if can be reached from previous maps
        if (!check_if_can_reached_from_previous(maps, map_vec, m)) {
          std::cout << "trimmed" << std::endl;
          it = map_vec.erase(it);
          continue;
        }
      }
      ++it;
    }
    if (map_vec.size() == 0) {
      std::cout << "trimmed" << std::endl;
      it = maps.erase(it);
    } else {
      ++it;
    }
  }
}

void part2() {
  std::ifstream input("input.txt");
  // std::ifstream input("test.txt");
  if (!input.is_open()) {
    std::cerr << "Failed to open input.txt" << std::endl;
    return;
  }

  std::vector<std::vector<mapping>> initial_maps;

  std::string line;

  // Read seeds
  std::getline(input, line);
  int colon_pos = line.find(":");
  std::vector<long> seeds =
      parse_nums(std::string_view(line.data() + colon_pos + 2));
  // for (long seed : seeds) {
  //   std::cout << seed << std::endl;
  // }
  std::getline(input, line);

  // Now fetch maps
  bool header_detected = false;
  while (std::getline(input, line)) {
    if (!header_detected) {
      header_detected = true;
      initial_maps.push_back(std::vector<mapping>());
      continue;
    }

    if (line.length() == 0) {
      header_detected = false;
      continue;
    }

    std::vector<mapping> &map_vec = initial_maps[initial_maps.size() - 1];

    std::vector<long> nums = parse_nums(std::string_view(line));
    map_vec.push_back({nums[0], nums[1], nums[2]});
  }

  // for (std::vector<mapping> map_vec : maps) {
  //   std::cout << "start" << std::endl;
  //   for (mapping m : map_vec) {
  //     std::cout << m.dest_from << " " << m.source_from << " " << m.size
  //               << std::endl;
  //   }
  //   std::cout << "end" << std::endl;
  // }

  // Mapping seed to loc
  // std::vector<long> end_locs;
  long min_loc = -1;
  for (int i = 0; i < seeds.size(); i += 2) {
    // We will trim this copy of maps
    std::vector<std::vector<mapping>> maps = initial_maps;

    long end = seeds[i] + seeds[i + 1] - 1;
    for (long seed = seeds[i]; seed <= end; ++seed) {
      if (seed % 1000000 == 0) {
        std::printf("Progress: %.2f%% | current min: %ld | seed: %ld\n",
                    (double)(seed - seeds[i]) / seeds[i + 1] * 100, min_loc,
                    seed);
        trim_maps(maps, seed, end);
      }
      long current = seed;
      for (std::vector<mapping> map_vec : maps) {
        for (mapping m : map_vec) {
          if (current >= m.source_from && current <= m.source_from + m.size) {
            current = (current - m.source_from) + m.dest_from;
            break;
          }
        }
      }

      if (min_loc == -1 || current < min_loc) {
        min_loc = current;

        // Trim maps
        trim_maps(maps, seed, end);
      }
    }
  }

  std::cout << min_loc << std::endl;
}

int main(int argc, char *argv[]) {
  part2();
  return 0;
}
