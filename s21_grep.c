#include "s21_grep.h"

int main(int argc, char *argv[]) {
  Flag flag = processing(argc, argv);
  while (optind < argc) {
    output(argv, flag);
    optind++;
  }
}

Flag processing(int argc, char *argv[]) {
  int opt = 0;
  Flag flag = {};
  while ((opt = getopt_long(argc, argv, "e:ivclnhsf:o", NULL, NULL)) != -1) {
    switch (opt) {
      case 'e':
        flag.e = 1;
        strcat(flag.arg, optarg);
        strcat(flag.arg, "|");
        break;
      case 'i':
        flag.i = 1;
        break;
      case 'v':
        flag.v = 1;
        break;
      case 'c':
        flag.c = 1;
        break;
      case 'l':
        flag.l = 1;
        break;
      case 'n':
        flag.n = 1;
        break;
      case 'f':
        option_f(&flag);
        flag.f = 1;
        break;
      case 'h':
        flag.h = 1;
        break;
      case 's':
        flag.s = 1;
        break;
      case 'o':
        flag.o = 1;
        break;
    }
  }
  record_arg(&flag, argc, argv);
  return flag;
}

void record_arg(Flag *flag, int argc, char *argv[]) {
  if (!flag->e && !flag->f) {
    if (argc > optind) {
      strcat(flag->arg, argv[optind]);
    }
    optind++;
  }
  if ((argc > optind + 1) && !flag->h) flag->print_file = 1;

  if (flag->e || flag->f) flag->arg[strlen(flag->arg) - 1] = '\0';

  if (flag->c || flag->l || flag->v) flag->o = 0;
}

void options_c_l(Flag *flag, int *file_match, int *str_match, char *argv[]) {
  if (flag->c) {
    if (flag->print_file) printf("%s:", argv[optind]);
    if (flag->l && *str_match) *str_match = 1;
    printf("%d\n", *str_match);
  }
  if (flag->l && *file_match) printf("%s\n", argv[optind]);
}

void option_f(Flag *flag) {
  char f_str[BUFFER];
  FILE *pattern;
  pattern = fopen(optarg, "r");
  if (pattern != NULL) {
    while (fgets(f_str, BUFFER, pattern) != NULL) {
      f_str[strcspn(f_str, "\n")] = '\0';
      strcat(flag->arg, f_str);
      strcat(flag->arg, "|");
    }

    fclose(pattern);
  } else if (!flag->s) {
    fprintf(stderr, "grep: %s: No such file or directory\n", optarg);
  }
}

void option_o(Flag flag, regex_t reg, int str_num, char *argv[]) {
  regmatch_t regmat[1];
  char *ptr = flag.str;
  while (regexec(&reg, ptr, 1, regmat, 0) != REG_NOMATCH) {
    if (flag.print_file) printf("%s:", argv[optind]);
    if (flag.n) printf("%d:", str_num);
    for (int i = regmat[0].rm_so; i < regmat[0].rm_eo; i++) {
      putchar(ptr[i]);
    }
    ptr += regmat[0].rm_eo;
    printf("\n");
  }
}

void processing_match(Flag flag, regex_t reg, int str_num, int *str_match,
                      int *file_match, char *argv[]) {
  regmatch_t regmat[1];
  int match = regexec(&reg, flag.str, 1, regmat, 0);  // *
  if (flag.v) match = !match;
  if (!match) {
    if (!flag.c && !flag.l && !flag.o) {
      if (flag.print_file) printf("%s:", argv[optind]);

      if (flag.n) printf("%d:", str_num);
      if (strchr(flag.str, '\n') == NULL) strcat(flag.str, "\n");
      printf("%s", flag.str);
    }
    *file_match = 1;
    (*str_match)++;
  }
}

void output(char *argv[], Flag flag) {
  FILE *file;
  regex_t reg;
  int str_num = 1;
  int str_match = 0;
  int file_match = 0;
  int flag_i = REG_EXTENDED;
  if (flag.i) flag_i = REG_EXTENDED | REG_ICASE;
  file = fopen(argv[optind], "r");
  if (file != NULL) {
    if (!regcomp(&reg, flag.arg, flag_i)) {
      while (fgets(flag.str, BUFFER, file) != NULL) {
        if (flag.o) {
          option_o(flag, reg, str_num, argv);
          str_num++;
          continue;
        }
        processing_match(flag, reg, str_num, &str_match, &file_match, argv);
        str_num++;
      }
      options_c_l(&flag, &file_match, &str_match, argv);
      regfree(&reg);
    }
    fclose(file);
  } else if (!flag.s) {
    fprintf(stderr, "grep: %s: No such file or directory\n", argv[optind]);
  }
}
