#include<stdio.h>

int hex2dd(char);
int main(int argc, char **argv)
{
  char *hex;
  int dd[4];
  int i = 0, j;
  if(argc != 2) {
    fprintf(stderr, "usage: %s <a hex number of 32 bits, like 0xnnnnnnnn>\n", argv[0]);
    return 0;
  }
  else {
    hex = argv[1];
    for(i = 0; i < 4; i++) {
      j = i * 2 + 2;
      dd[i] = 16 * hex2dd(hex[j]) + hex2dd(hex[j + 1]);
    }
    printf("%d.%d.%d.%d\n", dd[0], dd[1], dd[2], dd[3]);
  }
  return 0;
}

int hex2dd(char hex)
{
  if(hex >= '0' && hex <= '9')
    return hex - '0';
  else
    return hex - 'a' + 10;
}
