#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

// Gnlike actual cat we're not going to do formatting options and stuff, at least for now,
// so just do something like the simple_cat function, but also don't care about dest file
// block size, for simplicity
#define INBUF_SIZE 1024
#define OUTBUF_SIZE 2048

static void copy_stdin(void);

static void close_file(FILE *file, const char *filename);

static void copy_file(FILE *file, const char *name);

int main(int argc, char *argv[])
{
	if (argc < 2) {
		copy_stdin();
	} else {
		for (int i = 1; i < argc; i++) {
			if ('-' == argv[i][0] && 0 == argv[i][1]) {
				copy_stdin();
			} else {
				copy_file(fopen(argv[i], "r"), argv[i]);
			}
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
#define WRITE_CHAR(x) do { \
	if (EOF == putchar(x)) { \
		perror("failed to write char"); \
		exit(1); \
	}} while(0)
	while (EOF != (c = fgetc(file))) {
		WRITE_CHAR(c);
		WRITE_CHAR(c);
	}
#undef WRITE_CHAR
	close_file(file, name);
}

static void close_file(FILE *file, const char *filename)
{
	if (EOF == fclose(file)) {
		dprintf(2, "errno %d: could not close file '%s'\n", errno, filename);
		exit(1);
	}
}

static void copy_stdin(void)
{
	char inbuf[INBUF_SIZE], outbuf[OUTBUF_SIZE];
	int nread = 0;
	int nwritten = 0;
	do {
		nread = read(0, inbuf, INBUF_SIZE);
		if (-1 == nread) {
			perror("failed to read from stdin");
			exit(1);
		}
		if (0 == nread) break;
		for (int i = 0; i < nread; i++) {
			outbuf[2 * i] = inbuf[i];
			outbuf[1 + 2 * i] = inbuf[i];
		}
		nwritten = write(1, outbuf, 2 * nread);
		if (nwritten < nread) {
			perror("error writing output");
			exit(1);
		}
	} while (1);
}
