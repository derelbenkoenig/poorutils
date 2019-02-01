#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

static void copy_file(FILE *file, const char *name);

int main(int argc, char *argv[])
{
	if (argc < 2) {
		copy_file(fdopen(0, "r"), "stdin");
	} else {
		for (int i = 1; i < argc; i++) {
			copy_file(fopen(argv[i], "r"), argv[i]);
		}
	}
}

static void copy_file(FILE *file, const char *name)
{
	int c;
	if (NULL == file) {
		dprintf(2, "errno %d: could not open file '%s'\n", errno, name);
		exit(1);
	}
#define WRITE_CHAR(x) if (EOF == putchar(x)) exit(1)
	while (EOF != (c = fgetc(file))) {
		WRITE_CHAR(c);
		WRITE_CHAR(c);
	}
#undef WRITE_CHAR
	if (EOF == fclose(file)) {
		dprintf(2, "errno %d: could not close file '%s'\n", errno, name);
		exit(1);
	}
}
