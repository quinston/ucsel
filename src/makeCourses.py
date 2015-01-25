#!/bin/env python3

courses = [
("cs136", "fws"),
("cs146", "w"),
("cs246", "fws"),
("or", ["cs136", "cs146"])
]

def availToCpp(avail):
 if avail == "fws":
  return "return true"
 elif avail == "f":
  return "return s == F"

for c in courses:
 print("LoneVertex {0};
{0}.isAvail = [](Season s, year_t y) { {1}; };", c[0],  availToCpp(c[1]))
