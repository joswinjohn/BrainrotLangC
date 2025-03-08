#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAR_BUFFER 256
#define TABLE_SIZE 10

typedef struct {
  char hash_in[MAX_CHAR_BUFFER];
  char hash_out[MAX_CHAR_BUFFER];
} entry;

char *hash(char *hash_in) {
  int length = strnlen(hash_in, MAX_CHAR_BUFFER);
  char hash_out[MAX_CHAR_BUFFER];
  for (int i = 0; i < length; i++) {
    strcat(hash_out, hash_in[i]);
  }
}

int main(int argc, char **argv) {
  if (argc != 1) {
    printf("brlc [file]");
    return -1;
  }
  char cmd[128] = "sed \'s//g\' ";
  strcat(cmd, argv[1]);
  system(cmd);
}
