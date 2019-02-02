#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

static FILE *stdin_file = NULL;


static FILE* open_stdin(void);
static void close_stdin(void);

static void close_regular_file(FILE *file, const char *filename);

static void do_close_file(FILE *file, const char *filename);

static void copy_file(FILE *file, const char *name);

int main(int argc, char *argv[])
{
	if (argc < 2) {
		copy_file(open_stdin(), "stdin");
	} else {
		for (int i = 1; i < argc; i++) {
			if ('-' == argv[i][0] && 0 == argv[i][1]) {
				copy_file(open_stdin(), "stdin");
			} else {
				copy_file(fopen(argv[i], "r"), argv[i]);
			}
		}
	}
	close_stdin(); // iff it was opened
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
	close_regular_file(file, name);
}

static void close_regular_file(FILE *file, const char *filename)
{
	if (file != stdin_file) {
		do_close_file(file, filename);
	}
}

static FILE *open_stdin(void)
{
	if (NULL == stdin_file) {
		stdin_file = fdopen(0, "r");
	}
	return stdin_file;
}

static void close_stdin(void)
{
	if (NULL != stdin_file) {
		do_close_file(stdin_file, "stdin");
	}
}

static void do_close_file(FILE *file, const char *filename)
{
	if (EOF == fclose(file)) {
		dprintf(2, "errno %d: could not close file '%s'\n", errno, filename);
		exit(1);
	}
}
