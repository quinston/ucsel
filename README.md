ucsel
===
A tool for long-term course planning. Because satisfiability is hard.

At its core, a wrapper around Google's SAT solver.

All term numbers are zero-indexed. If the solver says the course will be taken in term `n` where `n` is the number of terms in the schedule being considered, it is to be interpreted that the course is not being taken in the returned solution.

Dependencies
--

* Google OR tools
* Guile 2.0+

The packaged Python utility scripts for parsing UWaterloo API data are written in Python 3.

Build
--
Put OR tools headers and shared object where CMake can find it. I'd suggest making a `./lib` folder for the shared object and a `./include` folder for the headers.

 Then in the folder of the cloned repo, run:

```
mkdir build
cd build
cmake ..
make
```

Operation
--
* `('< a1 a2 a3)` Represents a prerequisite chain. `a1`, `a2`, ... can each be integers or strings. For instance `(< "A" 2 "C")` means course A must be taken before term 2 and course C must be taken after term 2.
* `('!= a1 a2 a3)` Represents an inequality constraint. `a1`, `a2`, ... can be integers or strings. `(!= 0 "A" "C") means courses A and C must not be taken in the same term, nor in term 0.
* `('!! a1 a2 a3)` Must-take courses. `a1`, `a2`, ... are strings.
* `(1 a1 a2 a3)` At least one of `a1`, `a2`, ... shall be taken. `a1`, `a2`, ... are strings.
* `('ssn a1 seasons)` Course `a1` must be taken in the given season(s). `a1` is a string. `seasons` is any subset of `'(F W S)`. 

After compiling, run "ucsel" in the bin directory. `test14` contains a sample search.

```
; test14
(load "../scm/lang.scm")                                                
(solve 
; List of constraints follows
'(
(< "A" "B" "C") 
(ssn "B" (W))  
(< "D" "E")   
(!! "A" "B" "C" "D" "E" "F") 
(!= "E" 1)  
)           
; Seasons of each study term follow
'(F W S)    
; Maximum number of courses per term follows
2) 
```

Searches for all configuration that satisfy the given demands:

* "A" is a prereq for "B" which is a prereq for "C";
* "B" can only be taken in the Winter;
* "D" is a prereq for "E";
* the user wants to take all of courses "A" through "F";
* the user does not wish to take "D" in Term 1;
* the user's subsequent study terms occur in Fall, Winter, and Spring;
* the user wants to take at most two terms per semester.

Example
--
Say I have five more study terms left in the following seasons:

0. Winter
1. Fall
2. Spring
3. Winter
4. Winter

And I want to achieve the following:

* Take CS 452 (Winter, Spring)
* Take CS 488 (All seasons)
* Take CS 444 (Winter)
* Take PHYS 454 (Fall)
* Don't take CS 452 and CS 488 in the same term

These courses have dependencies, some more flexible than the others:

* CS 452 requires CS 350 which must be taken in Term 1.
* CS 350 requires CS 240, 241, and 251 which must all be taken in Term 0.
* CS 488 requires CS 341, which must be taken in Term 1; CS 350; CS 370 (every season)
* CS 370 requires CS 241
* CS 341 must be taken in Term 1.
* PHYS 454 requires PHYS 334 (Winter)

We try the following (going for up to 6 courses a term):

```
(load "../scm/lang.scm")
(solve  '(
(!! "CS452" "CS488" "CS444" "PHYS454")
(ssn "CS452" (W S))
(ssn "CS488" (F W S))
(ssn "CS444" (W))
(ssn "PHYS454" (F))
(!= "CS452" "CS488")
(< "CS350" "CS452")
(< "CS350" 2)
(!= "CS350" 0)
(< "CS240" 1)
(< "CS241" 1)
(< "CS251" 1)
(< "CS341" "CS488")
(< "CS350" "CS488")
(< "CS370" "CS488")
(< "CS241" "CS370")
(< "CS341" 2)
(!= "CS341" 0)
(< "PHYS334" "PHYS454")
(ssn "PHYS334" (W))
)
'(W F S W W)
6)
```

One solution is:
 
```
((PHYS454 . 1) (PHYS334 . 0) (CS488 . 2) (CS452 . 4) (CS444 . 3) (CS370 . 1) (CS350 . 1) (CS341 . 1) (CS251 . 0) (CS241 . 0) (CS240 . 0)) 
```

that is, 

0. Take CS 251, 240, 241, PHYS 334
1. Take CS 341, 350, 370, PHYS 454
2. Take CS 448
3. Take CS 444
4. Take CS 452

Future Steps
--
* generalize the syntax to allow nested conditions--the end goal is to be able to control the SAT solver with something as expressive as disjunctive normal form. 

This needs to be accomplished by creating a correspondence between every pair of parentheses and an IntVar in the solver, possibly with sorting of the arguments in some cases (e.g. for "or" or "and"-type conjunctions) to prevent repeated IntVars.

* generalize "1-of" constraint to "n-of"

Copyright
--
Where not otherwised specified, all source files are licensed under GPLv3. See `COPYING`.
