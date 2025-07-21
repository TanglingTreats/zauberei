#include "compiler.h"
#include <stdio.h>
#include <stdlib.h>

char *getSource(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    printf("ERROR: Unable to open file.\n");
    return NULL;
  }

  // ---- FILE OPS ----
  fseek(file, 0L, SEEK_END);
  size_t fileSize = ftell(file);
  rewind(file);

  char *buffer = malloc(fileSize + 1);
  size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);

  buffer[bytesRead] = '\0';

  fclose(file);
  // ---- END FILE OPS ----

  return buffer;
}

int main(int argc, const char** argv) {
  // Open and read file contents

  if (argc == 1) {
    printf("Start repl\n");
  } else {
    char *buffer = getSource(argv[1]);
    if(buffer == NULL) {
      printf("ERROR: No such file.\n");
      return 1;
    }
    printf("----- SRC -----\n");
    printf("\n%s\n", buffer);
    printf("----- END SRC -----\n\n");

    compile(buffer);
  }

  return 0;
}
