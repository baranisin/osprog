#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

void err(const char *msg) { perror(msg); exit(EXIT_FAILURE); }

/**
 * Copies data from intfd into outfd
 * file descriptor.
 * Returns number of copied bytes or
 * -1 on error (and leaves errno set)
 */
ssize_t copyFds(int infd, int outfd)
{
	char buffer[64*1024];
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

int main()
{
	int sockfd;
	int ret = 0;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
		err("socket");

	uint16_t portno = 1234;
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &addr, sizeof(addr)) == -1)
		err("bind");

	if (listen(sockfd, 10) == -1)
		err("listen");

	while(1) {

		struct sockaddr_in clientAddr;
		socklen_t clientAddrSize = sizeof(clientAddr);
		int clientSock = accept(sockfd, (struct sockaddr *) &clientAddr,
				&clientAddrSize);
		if (clientSock == -1 && errno != EAGAIN)
			err("accept");

		if (clientSock != -1) {
			// new client
			fprintf(stderr, "New connection from %s:%d\n",
					inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
			copyFds(clientSock, 1);
		}

	}

	close(sockfd);
	return ret;
}

/* vim: set sw=4 sts=4 ts=4 noet : */
