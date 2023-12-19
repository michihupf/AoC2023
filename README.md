# C++ Solutions to Advent of Code 2023
This respository contains my solutions to the Advent of Code 2023 event (https://adventofcode.com/2023).

## Disclaimer
My code is certainly not the optimal solution. Every solution works and this repository serves to showcase
my approach to the problems. The code can certainly be refined, optimzied and improved in terms of readability.
Having said that those are just my initial solutions and I did not find the time to refactor (yet). I might refactor
the code when I find the time but I also might not - we shall see.

#### Completeness
I am still working on some of my solutions. I will solve them still but I do not have the
time to look into it right now. I did not have time to do some entire days (e.g 12 and 17) yet either so I will do that
when I find the time - I looked at the problems and they doesn't seem too hard, some thinking will probably be required
though. I hope to be finished by 01.01.2024 but I can't guarantee it. Having said that there are plenty of other C++
solution repos here on GitHub if you really need a solution fast although some are quite cryptic at a 
first glance - see [Bogdanp/awesome-advent-of-code](https://github.com/Bogdanp/awesome-advent-of-code?tab=readme-ov-file#c-2).

## File Structure
There a 25 directories named `day01` to `day25` which contain the code to solve the problems of that
specific day. In those directories two `cpp`-files can be found: `1.cpp` and `2.cpp` corresponding
to the first and second part of the days problem respectively.
All source files have been compiled using

    $ g++ -std=c++20 -Wall <x>.cpp

The root directory contains a `day` script which can be sourced to open the problem page and `cd` into the 
corresponding directory. For example running

    $ . day 1

or

    $ source day 1

opens https://adventofcode.com/2023/day/1 and changes the working directory to `day01`. Simply executing the file 
with `./day 1` will not change the directory so make sure to source it if you use the script.


## Input and Output
Every solution reads the input from a file called `input`. The result is saved into `output` but 
can also be read out in `stdout` as the last space seperated token of the first line.
