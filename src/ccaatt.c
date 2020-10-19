#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// Unlike actual cat we're not going to do formatting options and stuff, at least for now,
// so just do something like the simple_cat function, but also don't care about dest file
// block size, for simplicity
#define INBUF_SIZE 1024
#define OUTBUF_SIZE 2048

static void copy_stream(int);

static void close_fd(int fd, const char *filename);

int main(int argc, char *argv[])
{
	if (argc < 2) {
		copy_stream(0);
	} else {
		for (int i = 1; i < argc; i++) {
			int fd;
			if ('-' == argv[i][0] && 0 == argv[i][1]) {
				fd = 0;
			} else {
				fd = open(argv[i], O_RDONLY);
			}
			copy_stream(fd);
			if (fd != 0) {
				close_fd(fd, argv[i]);
			}
		}
	}
}

static void close_fd(int fd, const char *filename)
{
	int retval;
	if (-1 == (retval = close(fd))) {
		dprintf(2, "errno %d: could not close file '%s'\n", errno, filename);
		exit(1);
	}
}

static void copy_stream(int fd)
{
	char inbuf[INBUF_SIZE], outbuf[OUTBUF_SIZE];
	int nread = 0;
	int nwritten = 0;
	do {
		nread = read(fd, inbuf, INBUF_SIZE);
		if (-1 == nread) {
			perror("failed to read from stdin");
			exit(1);
		}

		if (0 == nread) break;
		for (int i = 0; i < nread; i++) {
			outbuf[2 * i] = inbuf[i];
			outbuf[1 + 2 * i] = inbuf[i];
		}

		int bytes_to_write = 2 * nread;

		nwritten = write(1, outbuf, bytes_to_write);
		/* assume that writes to stdout should always complete in one call */
		if (nwritten < bytes_to_write) {
			perror("error writing output");
			exit(1);
		}
	} while (1);
}
