#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"
char separators[8];
static void sixfive_process_fd(int fd) {
  char c;
  int has_started = 1; 
  int j = 0;
  char tmp[256];
  while (read(fd, &c, 1) == 1) {
    if (strchr(separators, c) != 0) {
      if (has_started == 0) {
        has_started = 1;
        j = 0;
        continue;
      }
      if (j == 0) {
        has_started = 0;
        continue;
      }
      tmp[j] = '\0';
      int num = atoi(tmp);
      if (num % 5 == 0 || num % 6 == 0) {
        printf("%d\n", num);
      }
      j = 0;
      has_started = 1; 
    } else {
      if (c >= '0' && c <= '9') {
        tmp[j++] = c;
      } else {
        j = 0;
        has_started = 0;
      }
    }
  }
  if (j != 0) {
    tmp[j] = '\0';
    int num = atoi(tmp);
    if (num % 5 == 0 || num % 6 == 0) {
      printf("%d\n", num);
    }
  }
}
int
main(int argc, char *argv[])
{
  separators[0] = '-';
  separators[1] = '\r';
  separators[2] = '\t';
  separators[3] = '\n';
  separators[4] = '.';
  separators[5] = '/';
  separators[6] = ',';
  separators[7] = ' ';
  int fd;
  if (argc == 1) {
    fd = 0;
    sixfive_process_fd(fd);
    exit(0);
  }
  for (int i = 1; i < argc; i++) {
    if ((fd = open(argv[i], O_RDONLY)) < 0) {
      fprintf(2, "sixfive cannot open %s\n", argv[i]);
    }
    sixfive_process_fd(fd);
  }
  exit(0);
}
