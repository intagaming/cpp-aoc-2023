#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

enum Rank {
  HIGH_CARD = 0,
  ONE_PAIR = 1,
  TWO_PAIR = 2,
  THREE_OF_A_KIND = 3,
  FULL_HOUSE = 4,
  FOUR_OF_A_KIND = 5,
  FIVE_OF_A_KIND = 6,
};

struct Hand {
  std::string cards;
  int bid;
  Rank rank;
};

void calculate_rank(Hand &h) {
  std::map<char, int> m;
  for (char &c : h.cards) {
    if (m.find(c) == m.end()) {
      m.insert({c, 1});
    } else {
      m[c]++;
    }
  }

  std::map<int, int> occurances_map;
  for (auto const &pair : m) {
    if (occurances_map.find(pair.second) == occurances_map.end()) {
      occurances_map.insert({pair.second, 1});
    } else {
      occurances_map[pair.second]++;
    }
  }

  // for (auto const &pair : occurances_map) {
  //   std::cout << pair.first << " " << pair.second << std::endl;
  // }
  // std::cout << "end" << std::endl;

  Rank r;
  if (occurances_map.find(5) != occurances_map.end() &&
      occurances_map[5] == 1) {
    r = FIVE_OF_A_KIND;
  } else if (occurances_map.find(4) != occurances_map.end() &&
             occurances_map[4] == 1) {
    r = FOUR_OF_A_KIND;
  } else if (occurances_map.find(3) != occurances_map.end() &&
             occurances_map[3] == 1) {
    if (occurances_map.find(2) != occurances_map.end() &&
        occurances_map[2] == 1) {
      r = FULL_HOUSE;
    } else {
      r = THREE_OF_A_KIND;
    }
  } else if (occurances_map.find(2) != occurances_map.end()) {
    if (occurances_map[2] == 2) {
      r = TWO_PAIR;
    } else {
      r = ONE_PAIR;
    }
  } else {
    r = HIGH_CARD;
  }

  h.rank = r;
}

void calculate_rank_part2(Hand &h) {
  std::map<char, int> m;
  int jokers = 0;
  for (char &c : h.cards) {
    if (c == 'J') {
      jokers++;
      continue;
    }
    if (m.find(c) == m.end()) {
      m.insert({c, 1});
    } else {
      m[c]++;
    }
  }

  std::map<int, int> occurances_map;
  for (auto const &pair : m) {
    if (occurances_map.find(pair.second) == occurances_map.end()) {
      occurances_map.insert({pair.second, 1});
    } else {
      occurances_map[pair.second]++;
    }
  }

  if (jokers > 0) {
    // std::cout << h.cards << " - jokers: " << jokers << " - before" << std::endl;
    // for (auto const &pair : occurances_map) {
    //   std::cout << pair.first << " " << pair.second << std::endl;
    // }

    if (jokers == 5) {
      occurances_map[5] = 1;
    } else if (occurances_map[4] > 0) {
      occurances_map[4]--;
      occurances_map[5]++;
    } else if (occurances_map[3] > 0) {
      occurances_map[3]--;
      if (jokers == 2) {
        occurances_map[5] = 1;
      } else {
        occurances_map[4] = 1;
        occurances_map[1] = 1;
      }
    } else if (occurances_map[2] > 0) {
      occurances_map[2]--;
      if (occurances_map[2] == 2) {
        occurances_map[3] = 1;
      } else {
        occurances_map[2 + jokers] = 1;
      }
    } else { // All distinct cards
      occurances_map[1]--;
      occurances_map[jokers + 1] = 1;
    }

    // std::cout << "after" << std::endl;
    // int sum = 0;
    // for (auto const &pair : occurances_map) {
    //   sum += pair.first * pair.second;
    //   std::cout << pair.first << " " << pair.second << std::endl;
    // }
    // std::cout << "end" << std::endl;
    // if (sum != 5) {
    //   std::cout << "error" << std::endl;
    //   std::getchar();
    // }
  }

  Rank r;
  if (occurances_map[5] == 1) {
    r = FIVE_OF_A_KIND;
  } else if (occurances_map[4] == 1) {
    r = FOUR_OF_A_KIND;
  } else if (occurances_map[3] == 1) {
    if (occurances_map[2] == 1) {
      r = FULL_HOUSE;
    } else {
      r = THREE_OF_A_KIND;
    }
  } else if (occurances_map[2] > 0) {
    if (occurances_map[2] == 2) {
      r = TWO_PAIR;
    } else {
      r = ONE_PAIR;
    }
  } else {
    r = HIGH_CARD;
  }

  h.rank = r;
}

static std::string cards = "AKQJT98765432";
static std::string cards_part2 = "AKQT98765432J";

bool sort_hand(Hand &a, Hand &b) {
  if (a.rank != b.rank) {
    return a.rank < b.rank;
  }

  for (int i = 0; i < 5; ++i) {
    if (a.cards[i] == b.cards[i]) {
      continue;
    }
    return cards.find(a.cards[i]) > cards.find(b.cards[i]);
  }

  return false;
}

bool sort_hand_part2(Hand &a, Hand &b) {
  if (a.rank != b.rank) {
    return a.rank < b.rank;
  }

  for (int i = 0; i < 5; ++i) {
    if (a.cards[i] == b.cards[i]) {
      continue;
    }
    return cards_part2.find(a.cards[i]) > cards_part2.find(b.cards[i]);
  }

  throw std::runtime_error("Should not reach");
}

void part1() {
  std::ifstream input("input.txt");
  // std::ifstream input("test.txt");
  if (!input.is_open()) {
    std::cerr << "Failed to open input.txt" << std::endl;
    return;
  }

  std::string line;
  std::vector<Hand> hands;

  while (std::getline(input, line)) {
    int space_pos = line.find(" ");
    Hand h = {line.substr(0, space_pos), std::stoi(line.substr(space_pos + 1))};
    calculate_rank(h);
    hands.push_back(h);
  }

  std::sort(hands.begin(), hands.end(), sort_hand);

  // for (Hand h : hands) {
  //   std::cout << h.cards << " " << h.bid << " " << h.rank << std::endl;
  // }

  int winnings = 0;
  for (int i = 0; i < hands.size(); ++i) {
    Hand &h = hands[i];
    // std::cout << h.bid << std::endl;
    winnings += h.bid * (i + 1);
  }

  std::cout << winnings << std::endl;
}

void part2() {
  std::ifstream input("input.txt");
  // std::ifstream input("test.txt");
  if (!input.is_open()) {
    std::cerr << "Failed to open input.txt" << std::endl;
    return;
  }

  std::string line;
  std::vector<Hand> hands;

  while (std::getline(input, line)) {
    int space_pos = line.find(" ");
    Hand h = {line.substr(0, space_pos), std::stoi(line.substr(space_pos + 1))};
    calculate_rank_part2(h);
    hands.push_back(h);
  }

  std::sort(hands.begin(), hands.end(), sort_hand_part2);

  // for (Hand h : hands) {
  //   std::cout << h.cards << " " << h.bid << " " << h.rank << std::endl;
  // }

  int winnings = 0;
  for (int i = 0; i < hands.size(); ++i) {
    Hand &h = hands[i];
    // std::cout << h.bid << std::endl;
    winnings += h.bid * (i + 1);
  }

  std::cout << winnings << std::endl;
}

int main(int argc, char *argv[]) {
  part2();
  return 0;
}
