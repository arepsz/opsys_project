# opsys_project
C project for the ELTE-IK opsys subject.

### Bunny race
There's an egghunt game in the village of the bunnies. Each bunny has to collect as many eggs as it can. The bunny with the most eggs win the game.

### How does it work
Basically it's just a simulation with pipes (fork). The program reads the information about the bunnies from a file. Then if we choose to run the game, it simulates the race and
gives us the outcome.
When we run the program fron console we have to add a command.

### Commands
- --a: With this command you can add a new bunny to the contest.
- --fl This command prints out all the bunnies that are currently in the file.
- --d You can delete the chosen bunny.
- --m You can modify the chosen bunny.
- --rl With this you can print out the bunnies grouped by their location.
- --c You can start the contest with this command.
- --help Help command.

### Functionality
The code opens up the pipes and throws error when it does not succeed. 
Then it forks the parent and children elements. The locations are sorted into two groups, so that's why there's two children, the child uses the pipes depending on the location.
Basically it just goes through the given array (the file with the bunnies) and decides which pipe to use to transfer data between the parent and the children.
