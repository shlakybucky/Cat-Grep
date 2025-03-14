#include "s21_cat.h"

int main(int argc, char *argv[]) {
  flags new_flags = {0};
  if (argc > 1) {
    yes_arguments(new_flags, argc, argv);
  } else {
    no_arguments();
  }
  return 0;
}

void define_flag(flags *new_flags, int argc, char *argv[]) {
  int entered_flag;
  const struct option long_flags[] = {
      {"number-nonblank", no_argument, NULL, 'b'},
      {"number", no_argument, NULL, 'n'},
      {"squeeze-blank", no_argument, NULL, 's'},
      {NULL, 0, NULL, 0}};

  while ((entered_flag =
              getopt_long(argc, argv, "+beEnstTv", long_flags, NULL)) != -1) {
    switch (entered_flag) {
      case 'b':
        new_flags->number_filled_lines = 1;
        new_flags->number_all_lines = 0;
        break;
      case 'e':
        new_flags->put_dollar = 1;
        new_flags->unprintable_symbols = 1;
        break;
      case 'E':
        new_flags->put_dollar = 1;
        break;
      case 'n':
        if (!new_flags->number_filled_lines) {
          new_flags->number_all_lines = 1;
        }
        break;
      case 's':
        new_flags->squeeze_empty_lines = 1;
        break;
      case 't':
        new_flags->show_tabs = 1;
        new_flags->unprintable_symbols = 1;
        break;
      case 'T':
        new_flags->show_tabs = 1;
        break;
      case 'v':
        new_flags->unprintable_symbols = 1;
        break;
      case '?':
        new_flags->mistake = 1;
        printf("cat: invalid option\n");
        break;
    }
  }
}

void yes_arguments(flags new_flags, int argc, char *argv[]) {
  FILE *users_file = NULL;
  define_flag(&new_flags, argc, argv);
  if (!new_flags.mistake) {
    while (optind < argc) {
      users_file = fopen(argv[optind], "rb+");
      if (users_file != NULL) {
        do_flags(users_file, new_flags);
        fclose(users_file);
      } else {
        fprintf(stderr, "s21_cat: %s: No such file or directory\n",
                argv[optind]);
      }
      optind++;
    }
  }
}

void do_flags(FILE *users_file, flags new_flags) {
  char previous_symb = '\n', current_symb = 0;
  int squeeze = 0, line_number = 1;
  for (previous_symb = '\n'; (current_symb = fgetc(users_file)) != EOF;
       previous_symb = current_symb) {
    if (previous_symb == '\n') {
      if (new_flags.squeeze_empty_lines) {
        if (current_symb == '\n') {
          if (squeeze) {
            continue;
          }
          squeeze = 1;
        } else {
          squeeze = 0;
        }
      }
      if (new_flags.number_all_lines &&
          (!new_flags.number_filled_lines || current_symb != '\n')) {
        if (previous_symb == '\n') {
          printf("%6d\t", line_number);
          line_number++;
        }
      }
    }
    if (new_flags.number_filled_lines) {
      if (previous_symb == '\n' && current_symb != '\n') {
        printf("%6d\t", line_number);
        line_number++;
      }
    }
    if (new_flags.unprintable_symbols) {
      if (iscntrl(current_symb) && (current_symb != '\t') &&
          (current_symb != '\n')) {
        if (current_symb == '\177') {
          printf("^%c", current_symb - 64);
        } else {
          printf("^%c", current_symb + 64);
        }
        continue;
      }
    }
    if (new_flags.put_dollar) {
      if (current_symb == '\n') {
        printf("$");
      }
    }
    if (new_flags.show_tabs && current_symb == '\t') {
      printf("^");
      printf("I");
      continue;
    }
    printf("%c", current_symb);
  }
}

void no_arguments() {
  char symb = -1;
  while ((symb = getchar()) != EOF) {
    printf("%c", symb);
  }
}
