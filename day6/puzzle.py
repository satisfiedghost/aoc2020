#!/usr/bin/python3


with open("input.txt") as input:
  lines = [x.strip() for x in input.readlines()] + ['']

  groups, group = [], []
  for l in lines:
    if l != '':
      group.append(l)
    else:
      groups.append(group)
      group = [] 

  sum = 0
  for g in groups:
    sum = sum + len(set([char for char in ''.join(g)]))

  print("part 1: {}".format(sum))

  sum = 0
  for g in groups:
    all_answers = [char for char in ''.join(g)]
    counts = {}
    for a in all_answers:
      if a not in counts.keys():
        counts[a] = 1
      else:
        counts[a] = counts[a] + 1
      
    for a in set(all_answers):
      if counts[a] == len(g):
        sum = sum + 1

  print("part 2: {}".format(sum))
    


