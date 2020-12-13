#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <utility>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/erase.hpp>


/**
 * A bag.
 */
class Bag {
  public:
    Bag(std::string s, std::vector<std::pair<size_t, std::string>> rules) : name(s), can_contain(rules) {}

    // name of this bag
    const std::string name;

    bool operator==(Bag &other) const {
      return name == other.name;
    }

    // how many and of waht type this bag must contain
    std::vector<std::pair<size_t, std::string>> can_contain;
};

// teach ostream how to print a bag
std::ostream &operator<<(std::ostream &os, const Bag &bag) {
  std::string s = bag.name + " can contain ";
  for (auto r : bag.can_contain) {
    s = s + std::to_string(r.first) + " " + r.second + " ";
  }
  return os << s;
}

constexpr char c_contain[] = "contain";
constexpr char c_comma[] = ",";
constexpr char c_bag[] = "bag";

// i hate string parsing
Bag parse_rule(std::string &rule) {
    std::string type = rule.substr(0, rule.find(c_bag));
    std::vector<std::pair<size_t, std::string>> can_contain;
    boost::trim(type);

    // get rid of trailing period
    boost::erase_all(rule, ".");
    std::vector<std::string> contents;

    // split into bags that can be contained
    boost::split(contents, static_cast<const std::string>(rule.substr(rule.find(c_contain) + strlen(c_contain), rule.length())), boost::is_any_of(","));

    for (auto &s : contents) {
      boost::trim(s);
    }

    for (auto s : contents) {
      std::string subrule = s.substr(0, s.find(c_bag));
      boost::trim(subrule);

      if (subrule == "no other") {
        break;
      }

      std::vector<std::string> subrule_contents;
      boost::split(subrule_contents, subrule, boost::is_any_of(" "));

      size_t size = static_cast<size_t>(std::stoi(subrule_contents.front()));

      std::string contained_bag = s.substr(s.find(subrule_contents.front()) + subrule_contents.front().length(), s.find(c_bag) - 1);
      boost::trim(contained_bag);

      can_contain.push_back(std::make_pair(size, contained_bag));
    }

    return Bag(type, can_contain);
}

bool can_hold_recursive(const std::string type, const std::string container, const std::unordered_map<std::string, Bag> &bag_map) {
  bool can_hold = false;

  auto search = bag_map.find(container);
  Bag bag_in_question = search->second;

  // this bag cannot hold other bags or itself
  if (bag_in_question.can_contain.empty() or type == container) {
    return false;
  }


  for (auto can : bag_in_question.can_contain) {
    // done recursing, we've found a bag
    if (can.second == type) {
      return true;
    }
    can_hold = can_hold || can_hold_recursive(type, can.second, bag_map);
  }

  return can_hold;
}

size_t hold_cnt_recursive(const std::string type, const std::unordered_map<std::string, Bag> &bag_map) {
  size_t bags_i_hold = 0;

  auto search = bag_map.find(type);
  Bag bag_in_question = search->second;


  // this bag cannot hold other bags or itself
  if (bag_in_question.can_contain.empty()) {
    return 0;
  }

  for (auto can : bag_in_question.can_contain) {
    // must contain x number of bags with y bags in them...
     bags_i_hold += can.first + (can.first * hold_cnt_recursive(can.second, bag_map));
  }

  return bags_i_hold;
}

int main() {
  std::string line;
  std::ifstream input("input.txt");

  std::vector<Bag> bags;
  std::unordered_map<std::string, Bag> bag_map;

  while (std::getline(input, line)) {
    Bag b = parse_rule(line);
    bags.push_back(b);
    bag_map.insert({b.name, b});
  }

  size_t cnt = 0;
  for (auto b : bags) {
    if (can_hold_recursive("shiny gold", b.name, bag_map) ) {
      cnt++;
    }
  }

  std::cout << "part1 : " << cnt << std::endl;
  std::cout << "part2 : " << hold_cnt_recursive("shiny gold", bag_map) << std::endl;

  return 0;
}
