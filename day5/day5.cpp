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

  bool includes(long seed) {
    return seed >= source_from && seed <= source_from + size - 1;
  }

  long map(long seed) {
    if (!includes(seed)) {
      return seed;
    }
    return (seed - source_from) + dest_from;
  }
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

struct range {
  long from;
  long to;
};

void part2() {
  std::ifstream input("input.txt");
  // std::ifstream input("test.txt");
  if (!input.is_open()) {
    std::cerr << "Failed to open input.txt" << std::endl;
    return;
  }

  std::vector<std::vector<mapping>> maps;

  std::string line;

  std::vector<range> input_ranges;

  // Read seed ranges
  std::getline(input, line);
  int colon_pos = line.find(":");
  std::vector<long> seeds =
      parse_nums(std::string_view(line.data() + colon_pos + 2));
  for (int i = 0; i < seeds.size(); i += 2) {
    input_ranges.push_back({seeds[i], seeds[i] + seeds[i + 1] - 1});
  }
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

  // Goals: Break the seeds down into groups, then only evaluate the first item
  // in each group.
  //
  // Because a group is always a chunk of numbers being mapped around, the
  // first element in a group will always result in the smallest location in
  // the end.

  // For each layer, slice input into pieces and map them to next layer
  for (std::vector<mapping> &map_vec : maps) {
    for (mapping m : map_vec) {
      std::vector<range> sliced_ranges;

      for (range &r : input_ranges) {
        long source_from = m.source_from, source_to = m.source_from + m.size;
        // If outside the map
        if ((r.from < source_from && r.to < source_from) ||
            (r.from > source_from && r.to > source_from)) {
          sliced_ranges.push_back(r);
          continue;
        }
        // If inside the map
        if (r.from >= source_from && r.to <= source_to) {
          sliced_ranges.push_back(r);
          continue;
        }
        // If containing the map
        if (r.from <= source_from && r.to >= source_to) {
          // Slice into 3
          if (r.from < source_from) {
            sliced_ranges.push_back({r.from, source_from - 1});
          }
          sliced_ranges.push_back({source_from, source_to});
          if (r.to > source_to) {
            sliced_ranges.push_back({source_to + 1, r.to});
          }
          continue;
        }
        // If overlaps the map from the left
        if (r.from < source_from && r.to >= source_from && r.to <= source_to) {
          sliced_ranges.push_back({r.from, source_from - 1});
          sliced_ranges.push_back({source_from, r.to});
          continue;
        }
        // If overlaps the map from the right
        if (r.to > source_to && r.from <= source_to && r.from >= source_from) {
          sliced_ranges.push_back({r.from, source_to});
          sliced_ranges.push_back({source_to + 1, r.to});
          continue;
        }
      }

      input_ranges = sliced_ranges;
    }

    // map to next layer input
    for (range &r : input_ranges) {
      for (mapping &m : map_vec) {
        if (m.includes(r.from)) {
          r.from = m.map(r.from);
          r.to = m.map(r.to);
          goto out;
        }
      }
    out:;
    }
  }

  long min_loc = -1;
  for (range &r : input_ranges) {
    if (min_loc == -1 || r.from < min_loc) {
      min_loc = r.from;
    }
    // std::cout << r.from << " " << r.to << std::endl;
  }

  std::cout << min_loc << std::endl;
}

int main(int argc, char *argv[]) {
  part2();
  return 0;
}
