# Space Fortress

## about

Dwarf Fortress but in space!

## todo list
### how to use todo list in here:

1. mark what you are working on right now by adding some character ("MJL") in the second box (so we don't work on the same thing at the same time)
2. mark with "X" in the first box when you push a working and tested solution
3. add the mark by doing a direct commit to main (this is the only exception when you are allowed to commit to main directly, this way the todo list is always in sync!)

- [X] [MJL] add todo to README so we know what to work on
- [X] [MJL] add some infos at the bottom of the README so commits are standadized and whatever else

## idk, more info stuff for dev stuff:
0. do not commit to main from now on.
1. main has to ALWAYS be in a "compile and run, it works" state
2. when you work on something you make a branch based on whatever commit you need for it (probably the most recent one from main but could also be a different branch) and you call that branch "`<your name characters>_branch_for_<thing you are working on>`" (example: "MJL_branch_for_noise_functions") (or pick any other reasonable name, I'm just trying to come up with stuff right now lol)
3. when you are done with working on your branch you pull the most recent main, you test your changes in it (and later on check all our test cases once we have some written) and only when it all works the way it should you make a pull request for the branch (add some information for what you changed, on which main branch version your changes worked / were tested on (in case it gets pulled a bit later on so we know exactly what where works and whatever), etc)
4. add a short info into commit messages (so searching something is easy) and as much information into the description so we know exactly what you did in what commit
5. no specific coding style currently ... BUT (!!!) try to write "simple" code for now so the prototype can be done faster (and so the code can be understood easier by other people) ... once the prototype is done we can still improve performance or optimize code pieces, it's hell to try to decipher other peoples coding style when they write complex code (so when you write complex code in your style try to comment what some lines done so the concept of the information flow is clear at least)
6. always be balling! :bsns:
