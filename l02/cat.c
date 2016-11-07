#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>

#define BUFSIZE 1024*1024
char buffer[BUFSIZE];

/**
 * Copies data from intfd into outfd
 * file descriptor.
 * Returns number of copied bytes or
 * -1 on error (and leaves errno set)
 */
ssize_t copyFds(int infd, int outfd)
{
	ssize_t totalyTotalWritten = 0;
	while (1) {
		ssize_t bytesRead = 0;
		bytesRead = read(infd, buffer , sizeof(buffer));
		if (bytesRead == 0) {
			fprintf(stderr, "EOF\n");
			return totalyTotalWritten;
		}
		if (bytesRead < 0) {
			if (errno != EINTR && errno != EAGAIN) {
				return -1;
			}
			bytesRead = 0;
		}

		ssize_t totalWritten = 0;
		while (totalWritten < bytesRead) {
			ssize_t bytesWritten = write(outfd,
				buffer + totalWritten,
				bytesRead - totalWritten);
			if (bytesWritten == -1) {
				if (errno != EINTR && errno != EAGAIN) {
					return -1;
				}
				bytesWritten = 0;
			}
			totalWritten += bytesWritten;
			totalyTotalWritten += bytesWritten;
		}
	}
	return 0;
}

int main(int argc, char* argv[])
{
	int infd = 0;
	int outfd = 1;

	if (argc < 3) {
		fprintf(stderr, "Usage: %s infile outfile\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	if (strcmp("-", argv[1])) {
		infd = open(argv[1], O_CREAT|O_WRONLY|O_TRUNC, 0644);
	}

	if (strcmp("-", argv[2])) {
		outfd = open(argv[2], O_RDONLY);
	}

	if (copyFds(infd, outfd) == -1) {
		perror("Error copying");
	}
	return 0;
}
