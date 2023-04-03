# opsys_project
C project for the ELTE-IK opsys subject.

### Bunny race
There's an egghunt game in the village of the bunnies. Each bunny has to collect as many eggs as it can. The bunny with the most eggs win the game.

### How does it work
Basically it's just a simulation with pipes (fork). The program reads the information about the bunnies from a file. Then if we choose to run the game, it simulates the race and
gives us the outcome.
When we run the program fron console we have to add a command.

### Commands
- --a: It prints out 

if (argc == 2) {
    if (strcmp(argv[1], "--a") == 0) {
      printToFile();
    }
    if (strcmp(argv[1], "--fl") == 0) {
      printAll();
    }
    if (strcmp(argv[1], "--d") == 0) {
      deleteLine();
    }
    if (strcmp(argv[1], "--m") == 0) {
      modifyLine();
    }
    if (strcmp(argv[1], "--rl") == 0) {
      listByPlace();
    }
    if (strcmp(argv[1], "--c") == 0) {
      contest();
    }
    if (strcmp(argv[1], "--help") == 0) {
      help();
    }
  }
