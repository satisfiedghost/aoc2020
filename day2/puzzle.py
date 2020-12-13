#!/usr/bin/python

with open("input1.txt") as file:
  valid = 0
  lines = file.readlines()

  for line in lines:
    tokens = line.strip().split()

    bounds = tokens[0].split('-')
    lower = int(bounds[0])
    upper = int(bounds[1])

    check = tokens[1].strip(":")

    cnt = 0
    if tokens[2][lower - 1] == check:
      cnt = cnt + 1
    if tokens[2][upper - 1] == check:
      cnt = cnt + 1

    if cnt == 1:
      valid = valid + 1

  print(valid)




