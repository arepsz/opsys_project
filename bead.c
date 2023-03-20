#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void help() {
  char *places[] = {"Barátfa",     "Lovas",  "Szula",         "Kígyós-patak",
                    "Malom telek", "Páskom", "Káposztás kert"};
  int len = sizeof(places) / sizeof(places[0]);

  for (int i = 0; i < len; i++) {
    printf("%d. -> %s\n", i + 1, places[i]);
  }
}

void contest() {
  srand(time(NULL));
  FILE *ptr;
  ptr = fopen("nyuszik.txt", "r");
  char str[90];
  char copy[90];
  char name[60];
  char name_bal[60];
  char name_jobb[60];
  char read_name[60];
  char place[30];
  int pipebal[2];
  int pipejobb[2];
  int pipevissza_bal[2];
  int pipevissza_jobb[2];
  char max_name[60];
  int max_eggs = 0;
  pid_t child1, child2;
  int c = fgetc(ptr);
  if (c == EOF) {
    perror("Nincsenek felvett adatok!");
    fclose(ptr);
    exit(0);
  } else {
    ungetc(c, ptr);
  }

  if (pipe(pipebal) == -1 || pipe(pipejobb) == -1 ||
      pipe(pipevissza_bal) == -1 || pipe(pipevissza_jobb) == -1) {
    perror("Hiba a csövek nyitásakor");
    exit(EXIT_FAILURE);
  }

  child1 = fork();
  if (child1 == -1) {
    perror("Hiba a forkolásnál!");
    exit(EXIT_FAILURE);
  }
  if (child1 == 0) {
    child2 = fork();
    if (child2 == -1) {
      perror("Hiba a forkolásnál!");
      exit(EXIT_FAILURE);
    }
    if (child2 == 0) {
      // szulo
      while (fgets(str, 90, ptr) != NULL) {
        str[strcspn(str, "\n")] = 0;
        memcpy(copy, str, sizeof(str));
        char *name2 = strtok(copy, ",");
        char *place2 = strtok(NULL, ",");
        memcpy(name, name2, sizeof(name));
        memcpy(place, place2, sizeof(place));
        if (strcmp(place, "Barátfa") == 0 ||
            strcmp(place, "Kígyós-patak") == 0 || strcmp(place, "Lovas") == 0 ||
            strcmp(place, "Káposztás kert") == 0) {
          write(pipebal[1], name, sizeof(name));
        } else {
          write(pipejobb[1], name, sizeof(name));
        }
      }
      write(pipebal[1], ".", sizeof(name));
      write(pipejobb[1], ".", sizeof(name));

      close(pipebal[0]);
      close(pipejobb[0]);
      close(pipebal[1]);
      close(pipejobb[1]);

      while (1) {
        close(pipevissza_jobb[1]);
        read(pipevissza_jobb[0], read_name, sizeof(name_jobb));
        // printf("Szulo kapja: %s\n", read_name);
        if (strcmp(read_name, ".") == 0) {
          break;
        }
        char *bunny = strtok(read_name, ",");
        char *eggs = strtok(NULL, ",");
        int num = atoi(eggs);
        // printf("Kapott szam: %d\n", num);
        if (num > max_eggs) {
          max_eggs = num;
          memcpy(max_name, bunny, sizeof(max_name));
        }
      }
      // printf("Max tojas: %d\n", max_eggs);
      close(pipevissza_jobb[0]);
      while (1) {
        close(pipevissza_bal[1]);
        read(pipevissza_bal[0], read_name, sizeof(name_bal));
        // printf("Szulo kapja: %s\n", read_name);
        if (strcmp(read_name, ".") == 0) {
          break;
        }
        char *bunny = strtok(read_name, ",");
        char *eggs = strtok(NULL, ",");
        int num = atoi(eggs);
        // printf("Kapott szam: %d\n", num);
        if (num > max_eggs) {
          max_eggs = num;
          memcpy(max_name, bunny, sizeof(max_name));
        }
      }
      // printf("Max tojas: %d\n", max_eggs);
      close(pipevissza_bal[0]);
      printf("%s nyert! Ennyi tojással: %d\n", max_name, max_eggs);
    } else {
      // gyerek 2
      while (1) {
        close(pipejobb[1]);
        read(pipejobb[0], name_jobb, sizeof(name_jobb));
        printf("Gyerek2: %s\n", name_jobb);
        if (strcmp(name_jobb, ".") == 0) {
          break;
        }
        int r = rand() % 100 + 1;
        char rt[3];
        sprintf(rt, "%d", r);
        strcat(name_jobb, ",");
        strcat(name_jobb, rt);
        // printf("Gyerek2 kuldi: %s\n", name_jobb);
        close(pipevissza_jobb[0]);
        write(pipevissza_jobb[1], name_jobb, sizeof(name_jobb));
      }
      write(pipevissza_jobb[1], ".", sizeof(name_jobb));
      close(pipevissza_jobb[1]);
      close(pipejobb[0]);
      wait();
    }
  } else {
    // gyerek 1
    while (1) {
      close(pipebal[1]);
      read(pipebal[0], name_bal, sizeof(name_bal));
      printf("Gyerek1: %s\n", name_bal);
      if (strcmp(name_bal, ".") == 0) {
        break;
      }
      int r = rand() % 100 + 1;
      char rt[3];
      sprintf(rt, "%d", r);
      strcat(name_bal, ",");
      strcat(name_bal, rt);
      // printf("Gyerek1 kuldi: %s\n", name_bal);
      close(pipevissza_bal[0]);
      write(pipevissza_bal[1], name_bal, sizeof(name_bal));
    }
    write(pipevissza_bal[1], ".", sizeof(name_bal));
    close(pipevissza_bal[1]);
    close(pipebal[0]);
    wait();
  }
  fclose(ptr);
}

void listByPlace() {
  FILE *ptr;
  ptr = fopen("nyuszik.txt", "r");
  char home[30];
  char str[90];
  char copy[90];
  printf("Adja meg a lakóhelyet, ami alapján listázni szeretne: \n");
  gets(home);
  int c = fgetc(ptr);
  if (c == EOF) {
    perror("Nincsenek felvett adatok!");
    fclose(ptr);
    exit(0);
  } else {
    ungetc(c, ptr);
  }
  while (fgets(str, 90, ptr) != NULL) {
    str[strcspn(str, "\n")] = 0;
    memcpy(copy, str, sizeof(str));
    char *fst = strtok(copy, ",");
    char *scnd = strtok(NULL, ",");
    if (strcmp(home, scnd) == 0) {
      printf("%s\n", str);
    }
  }
  fclose(ptr);
}

void deleteLine() {
  FILE *ptr;
  FILE *write;
  ptr = fopen("nyuszik.txt", "r");
  write = fopen("temp.tmp", "a");
  char str[90];
  char copy[90];
  char name[50];
  printf("Adja meg a törölni kívánt nyúl nevét!\n");
  gets(name);
  int c = fgetc(ptr);
  if (c == EOF) {
    perror("Nincs törölhető adat!\n");
    remove("nyuszik.txt");
    rename("temp.tmp", "nyuszik.txt");
    fclose(ptr);
    fclose(write);
    exit(0);
  } else {
    ungetc(c, ptr);
  }
  while (fgets(str, 90, ptr) != NULL) {
    str[strcspn(str, "\n")] = 0;
    memcpy(copy, str, sizeof(str));
    char *ch = strtok(copy, ",");
    if (strcmp(name, ch) != 0) {
      fprintf(write, "%s\n", str);
    }
  }
  fclose(ptr);
  fclose(write);
  remove("nyuszik.txt");
  rename("temp.tmp", "nyuszik.txt");
}

bool validatePlace(char place[]) {
  char *places[] = {"Barátfa",     "Lovas",  "Szula",         "Kígyós-patak",
                    "Malom telek", "Páskom", "Káposztás kert"};
  int len = sizeof(places) / sizeof(places[0]);

  int count = 0;
  for (int i = 0; i < len; i++) {
    if (strcmp(places[i], place) != 0) {
      count++;
    }
  }
  if (count < 7) {
    return true;
  } else {
    return false;
  }
}

bool validateName(char name[]) {
  bool valid = true;
  FILE *ptr;
  ptr = fopen("nyuszik.txt", "r");
  char str[90];
  char copy[90];
  int c = fgetc(ptr);
  if (c == EOF) {
    valid = true;
  } else {
    ungetc(c, ptr);
  }
  while (fgets(str, 90, ptr) != NULL) {
    str[strcspn(str, "\n")] = 0;
    memcpy(copy, str, sizeof(str));
    char *ch = strtok(copy, ",");
    if (strcmp(name, ch) == 0) {
      valid = false;
    }
  }
  fclose(ptr);
  return valid;
}

void modifyLine() {
  FILE *ptr;
  FILE *write;
  ptr = fopen("nyuszik.txt", "r");
  write = fopen("temp.tmp", "a");
  char str[90];
  char copy[90];
  char name[50];
  char newname[50];
  char home[30];
  int hmt;
  printf("Adja meg a módosítani kívánt nyúl nevét!\n");
  gets(name);
  printf("Adja meg a nevet (akkor is, ha nem változik)!\n");
  gets(newname);
  if (strlen(newname) > 50) {
    perror("A név nem lehet hosszabb 50 karakternél!\n");
    exit(0);
  }
  if (validateName(newname) == false) {
    perror("A név már fel van véve a rendszerbe!\n");
    exit(0);
  }
  printf("Adja meg a lakóhelyet: \n");
  gets(home);
  if (validatePlace(home) == false) {
    perror("Nem megfelelő a helységnév!");
    exit(0);
  }
  printf("Hányszor vett részt?: \n");
  scanf("%d", &hmt);
  int c = fgetc(ptr);
  if (c == EOF) {
    perror("Nincs módosítható adat!\n");
    remove("nyuszik.txt");
    rename("temp.tmp", "nyuszik.txt");
    fclose(ptr);
    fclose(write);
    exit(0);
  } else {
    ungetc(c, ptr);
  }
  while (fgets(str, 90, ptr) != NULL) {
    str[strcspn(str, "\n")] = 0;
    memcpy(copy, str, sizeof(str));
    char *ch = strtok(copy, ",");
    if (strcmp(name, ch) == 0) {
      fprintf(write, "%s,%s,%d\n", newname, home, hmt);
    } else {
      fprintf(write, "%s\n", str);
    }
  }
  fclose(ptr);
  fclose(write);
  remove("nyuszik.txt");
  rename("temp.tmp", "nyuszik.txt");
}

void printToFile() {
  FILE *ptr;
  ptr = fopen("nyuszik.txt", "a+");
  if (ptr == NULL) {
    printf("Nem lehet megnyitni a filet!\n");
  }
  char name[50];
  char home[30];
  int hmt;
  printf("Adja meg a nevet: \n");
  gets(name);
  if (strlen(name) > 50) {
    perror("A név nem lehet hosszabb 50 karakternél!\n");
    exit(0);
  }
  if (validateName(name) == false) {
    perror("A név már fel van véve a rendszerbe!\n");
    exit(0);
  }
  printf("Adja meg a lakóhelyet: \n");
  gets(home);
  if (validatePlace(home) == false) {
    perror("Nem megfelelő a helységnév!");
    exit(0);
  }
  printf("Hányszor vett részt?: \n");
  scanf("%d", &hmt);
  fprintf(ptr, "%s,%s,%d\n", name, home, hmt);
  fclose(ptr);
}

void printAll() {
  FILE *ptr;
  char c;
  ptr = fopen("nyuszik.txt", "r");
  c = fgetc(ptr);
  while (c != EOF) {
    printf("%c", c);
    c = fgetc(ptr);
  }
  fclose(ptr);
}

int main(int argc, char const *argv[]) {
  if (argc == 1) {
    printf("Kérjük adjon meg egy kapcsolót az alábbiak közül: \n");
    printf("--a -> Nyuszi hozzáadása \n");
    printf("--d -> Sor törlése \n");
    printf("--m -> Sor módosítása \n");
    printf("--rl -> Nevek listázása terület szerint \n");
    printf("--fl -> Minden név listázása \n");
    printf("--c -> Verseny elindítása \n");
    printf("--help -> Segítség a helységnevekben\n");
  }

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

  return 0;
}
