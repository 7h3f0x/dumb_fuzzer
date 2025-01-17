#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    return 1;
  }

  FILE *f = fopen(argv[1], "rb");
  if (f == NULL) {
    return 2;
  }

  int w, h;
  fread(&w, 4, 1, f);
  fread(&h, 4, 1, f);

  unsigned char *buf = malloc(w * h);

  for (int j = 0; j < h; j++) {
    int b = fread(&buf[j * w], w, 1, f);
	if (b <= 0)
	{
		break;
	}
  }

  fclose(f);
  return 0;
}

