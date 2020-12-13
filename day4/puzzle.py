#!/usr/bin/python
import re

class Passport:

  def __init__(self, data):
    """
    Expects data split by whitespaces, single line
    """
    fields = data.split()
    self.data = {}

    for f in fields:
      pair = f.split(":")
      self.data[pair[0]] = pair[1]


  def is_valid(self):
    if not all(x in self.data.keys() for x in ["byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"]):
      return False

    def try_int(s, lower, upper):
      try:
        if lower <= int(s) <= upper:
          return True
        else:
          return False
      except ValueError:
        return False


    if not try_int(self.data['byr'], 1920, 2002):
      return False

    if not try_int(self.data['iyr'], 2010, 2020):
      return False

    if not try_int(self.data['eyr'], 2020, 2030):
      return False

    if self.data['hgt'].endswith("cm"):
      if not try_int(self.data['hgt'].rstrip('cm'), 150, 193):
        return False
    elif self.data['hgt'].endswith("in"):
      if not try_int(self.data['hgt'].rstrip('in'), 59, 76):
        return False
    else:
      return False


    if not re.match(r"^#[0-9a-f]{6}$", self.data['hcl']):
        return False

    if self.data['ecl'] not in [
        "amb", "blu", "brn", "gry", "grn", "hzl", "oth",
    ]:
        return False

    if not re.match(r"^[0-9]{9}$", self.data['pid']):
        return False

    return True

with open("input.txt") as file:
  all_lines = [x.strip() for x in file.readlines()]
  all_lines.append('')

  entries = []
  entry = ""
  for line in all_lines:
    if line != '':
      entry = entry + line + " "
    else:
      entries.append(entry.strip())
      entry = ""

  print("total entries: {}".format(len(entries)))
  passports = [Passport(x) for x in entries]

  valid_count = sum(p.is_valid() for p in passports)
  print("valid entries: {}".format(valid_count))

  print(valid_count)
