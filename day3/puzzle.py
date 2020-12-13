#!/usr/bin/python
import math
import numpy as np

slopes = [(1, 1), (3, 1), (5, 1), (7, 1), (1, 2)]
with open("input.txt") as file:
  lines = [x.strip() for x in file.readlines()]

  mmap = [x * 100 for x in lines]
  all_trees = []

  for s in slopes:
    xpos = s[0]
    ypos = s[1]

    trees = 0
    while ypos < len(mmap):
      if mmap[ypos][xpos] == '#':
        trees = trees + 1
      xpos = xpos + s[0]
      ypos = ypos + s[1]

    print("slope {} met {} trees".format(s, trees))
    all_trees.append(trees)

  print("product of trees: {}".format(np.prod(np.array(all_trees))))
