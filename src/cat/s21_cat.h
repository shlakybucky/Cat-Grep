#ifndef S21_H
#define S21_H

#include <ctype.h>
#include <getopt.h>
#include <stdio.h>

typedef struct {
  int number_filled_lines;
  int put_dollar;
  int number_all_lines;
  int squeeze_empty_lines;
  int show_tabs;
  int unprintable_symbols;
  int mistake;
} flags;

void yes_arguments(flags new_flags, int argc, char *argv[]);

void define_flag(flags *new_flags, int argc, char *argv[]);

void do_flags(FILE *users_file, flags new_flags);

void no_arguments();

#endif
