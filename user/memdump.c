#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

void memdump(char *fmt, char *data, int len);

int
main(int argc, char *argv[])
{
  if (argc == 1) {
    printf("Example 1:\n");
    int a[2] = {61810, 2026};
    memdump("ii", (char *)a, sizeof(a));

    printf("Example 2:\n");
    memdump("S", "a string", sizeof("a string"));

    printf("Example 3:\n");
    char *s = "another";
    memdump("s", (char *)&s, sizeof(s));

    struct sss {
      char *ptr;
      int num1;
      short num2;
      char byte;
      char bytes[8];
    } example;

    example.ptr = "hello";
    example.num1 = 1819438967;
    example.num2 = 100;
    example.byte = 'z';
    strcpy(example.bytes, "xyzzy");

    printf("Example 4:\n");
    memdump("pihcS", (char *)&example, sizeof(example));

    printf("Example 5:\n");
    memdump("sccccc", (char *)&example, sizeof(example));
  } else if (argc == 2) {
    // format in argv[1], up to 512 bytes of data from standard input.
    char data[512];
    int n = 0;
    memset(data, '\0', sizeof(data));
    while (n < sizeof(data)) {
      int nn = read(0, data + n, sizeof(data) - n);
      if (nn <= 0)
        break;
      n += nn;
    }
    memdump(argv[1], data, n);
  } else {
    printf("Usage: memdump [format]\n");
    exit(1);
  }
  exit(0);
}

void
memdump(char *fmt, char *data, int len)
{
  // Your code here.  `data` holds `len` valid bytes.
  int fmt_len = strlen(fmt);
  int cur_len = 0;
  for (int i = 0; i < fmt_len; i++) {
    switch (fmt[i]) {
      case 'i':
        if (cur_len + 4 > len) {
          printf("memdump: not enough data for '%c'\n", fmt[i]);
          return;
        }
        cur_len += 4;
        printf("%d\n", *((int*)data));
        data = data + 4;
        break;
      case 'p':
        if (cur_len + 8 > len) {
          printf("memdump: not enough data for '%c'\n", fmt[i]);
          return;
        }
        cur_len += 8;
        printf("%lx\n", *((uint64 *)data));
        data = data + 8;
        break;
      case 'h':
        if (cur_len + 2 > len) {
          printf("memdump: not enough data for '%c'\n", fmt[i]);
          return;
        }
        cur_len += 2;
        int x = (int)(*(uint16 *)(data));
        printf("%d\n", x);
        data = data + 2;
        break;
      case 'c':
        if (cur_len + 1 > len) {
          printf("memdump: not enough data for '%c'\n", fmt[i]);
          return;
        }
        cur_len += 1;
        printf("%c\n", *data);
        data = data + 1;
        break;
      case 's':
        if (cur_len + 8 > len) {
          printf("memdump: not enough data for '%c'\n", fmt[i]);
          return;
        }
        cur_len += 8;
        char *str = (char *)(*((uint64 *)data));
        printf("%s\n", str);
        data = data + 8;
        break;
      case 'S':
        printf("%s\n", data);
        if (cur_len + strlen(data) + 1 > len) {
          printf("memdump: not enough data for '%c'\n", fmt[i]);
          return;
        }
        cur_len += strlen(data) + 1;
        data = data + strlen(data) + 1;
        break;
      default:
        printf("default case\n");
    }
  }
}
