#ifndef S21_GREP_H
#define S21_GREP_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <string.h>

#define BUFFER 15000

typedef struct {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int f;
  int h;
  int s;
  int o;
  int print_file;
  char str[BUFFER];
  char arg[BUFFER];
} Flag;

Flag processing(int argc, char *argv[]);
void record_arg(Flag *flag, int argc, char *argv[]);
void options_c_l(Flag *flag, int *file_match, int *str_match, char *argv[]);
void option_f(Flag *flag);
void option_o(Flag flag, regex_t reg, int str_num, char *argv[]);
void processing_match(Flag flag, regex_t reg, int str_num, int *str_match,
                      int *file_match, char *argv[]);
void output(char *argv[], Flag flag);

#endif

