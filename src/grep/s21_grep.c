#include "s21_grep.h"

int main(int argc, char *argv[]) {
  flags entered_flags = {0};
  char pattern[BUFFER] = {0};
  char ch = 0;

  if (argc < 3) {
    while (ch != '\0') {
      scanf("%c", &ch);
    }
  } else {
    define_flags(&entered_flags, argc, argv, pattern);
    if (!entered_flags.mistake) {
      start_grep(&entered_flags, argc, argv, pattern);
    }
  }

  return 0;
}

void define_flags(flags *entered_flags, int argc, char *argv[],
                  char pattern[]) {
  const struct option long_flags[] = {{NULL, 0, NULL, 0}};
  int users_flags = 0;

  while ((users_flags = getopt_long(argc, argv, "e:ivclnshof:", long_flags,
                                    NULL)) != -1) {
    switch (users_flags) {
      case 'e':
        if (!entered_flags->l) {
          entered_flags->e = 1;
          strcat(pattern, optarg);
          strcat(pattern, "|");
        }
        break;
      case 'i':
        entered_flags->i = 1;
        break;
      case 'v':
        entered_flags->v = 1;
        entered_flags->o = 0;
        break;
      case 'c':
        entered_flags->c = 1;
        entered_flags->o = 0;
        entered_flags->n = 0;
        entered_flags->h = 0;
        break;
      case 'l':
        entered_flags->l = 1;
        entered_flags->o = 0;
        entered_flags->n = 0;
        entered_flags->h = 0;

        break;
      case 'n':
        if (!entered_flags->l && !entered_flags->c) entered_flags->n = 1;
        break;
      case 's':
        entered_flags->s = 1;
        break;
      case 'h':
        if (!entered_flags->l) entered_flags->h = 1;
        break;
      case 'o':
        if (!entered_flags->v && !entered_flags->l && !entered_flags->c)
          entered_flags->o = 1;
        break;
      case 'f':
        entered_flags->f = 1;
        entered_flags->s = 0;
        read_pattern_from_file(pattern, argv);
        break;
      case '?':
        entered_flags->mistake = 1;
        break;
    }
  }
  if (entered_flags->e != 1) {
    strcat(pattern, argv[optind]);
    optind++;
  }
  if (entered_flags->e == 1) {
    int len = strlen(pattern);
    pattern[len - 1] = '\0';
  }
}

void start_grep(flags *entered_flags, int argc, char *argv[], char pattern[]) {
  int cflags = REG_EXTENDED;
  int check = argc - optind;
  regex_t regex = {0};
  regmatch_t start;

  if (entered_flags->i == 1) {
    cflags |= REG_ICASE;
  }
  if ((regcomp(&regex, pattern, cflags) != 0) && !entered_flags->s) {
    fprintf(stderr, "Could not compile regex\n");
  } else {
    while (optind < argc) {
      char buffer[BUFFER] = {0};
      int lines_num = 1, counter = 0;
      FILE *users_file = fopen(argv[optind], "rb+");
      if (users_file == NULL && !entered_flags->s) {
        printf("No such file or directory\n");
      } else {
        while ((fgets(buffer, sizeof(buffer), users_file)) != NULL) {
          if (strchr(buffer, '\n') == NULL) {
            strcat(buffer, "\n");
          }
          int match = regexec(&regex, buffer, 0, &start, cflags);
          if (entered_flags->v) match = (!match);
          if (!match) {
            if (check > 1 && !entered_flags->h && !entered_flags->o &&
                !entered_flags->l && !entered_flags->c)
              printf("%s:", argv[optind]);
            if (!entered_flags->c && !entered_flags->l) {
              if (entered_flags->n && !entered_flags->o)
                printf("%d:", lines_num);
              if (entered_flags->o)
                o_flag(buffer, &start, &regex, argv, check, entered_flags,
                       lines_num);
              if (!entered_flags->o) printf("%s", buffer);
            }
            counter++;
          }
          lines_num++;
        }
        if (entered_flags->c && !entered_flags->l) {
          if (check > 1 && !entered_flags->h) printf("%s:", argv[optind]);
          printf("%d\n", counter);
        }
        if (entered_flags->l && !entered_flags->h && !entered_flags->o &&
            !entered_flags->c)
          printf("%s\n", argv[optind]);
        if (entered_flags->c && entered_flags->l /*&& !entered_flags->h*/) {
          if (check > 1 && !entered_flags->h) {
            printf("%s:", argv[optind]);
          }
          if (counter > 0) {
            printf("%d\n", 1);
          } else {
            printf("%d\n", 0);
          }
          if (!entered_flags->h) printf("%s\n", argv[optind]);
        }
      }
      if (users_file != NULL) {
        fclose(users_file);
      }
      optind++;
    }
  }
  regfree(&regex);
}

void o_flag(char *buffer, regmatch_t *start, regex_t *regex, char *argv[],
            int check, flags *flag, int lines_num) {
  char *cursor = buffer;
  while (!regexec(regex, cursor, 1, start, 0)) {
    if (check > 1 && !flag->h) printf("%s:", argv[optind]);
    if (flag->n) printf("%d:", lines_num);
    printf("%.*s\n", (int)(start->rm_eo - start->rm_so), cursor + start->rm_so);
    cursor += start->rm_eo;
  }
}

void read_pattern_from_file(char pattern[], char *argv[]) {
  char string[BUFFER] = {0};
  FILE *pattern_file = fopen(argv[optind], "r");
  if (pattern_file == NULL) {
    printf("No such file or directory\n");
  } else {
    while (!feof(pattern_file)) {
      if ((fgets(string, (sizeof(string) - 1), pattern_file)) != NULL) {
        if (string[strlen(string - 1)] == '\n' && strlen(string) - 1 != 0) {
          string[strlen(string) - 1] = '\0';
        }
      }
      if (pattern[0] != '\0') {
        strcat(pattern, "|");
        strcat(pattern, string);
      } else {
        strcat(pattern, string);
      }
    }
  }
}