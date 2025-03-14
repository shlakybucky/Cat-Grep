#ifndef S21_GREP_H
#define S21_GREP_H

#define BUFFER 2048
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int s;
  int h;
  int o;
  int f;
  int mistake;
  int many_files;
} flags;

void start_grep(flags* entered_flags, int argc, char* argv[], char pattern[]);
void define_flags(flags* entered_flags, int argc, char* argv[], char* pattern);

void process_file(int check, flags* entered_flags, char* argv[], int* match,
                  int* counter, int* lines_num, int* string_num, regex_t* regex,
                  regmatch_t* start, int cflags);
void o_flag(char* buffer, regmatch_t* start, regex_t* regex, char* argv[],
            int check, flags* flag, int lines_num);

void read_pattern_from_file(char pattern[], char* argv[]);

#endif