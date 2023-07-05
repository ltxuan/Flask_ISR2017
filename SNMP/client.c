#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

void main()
{
	int sockfd = -1;
	struct sockaddr_in server_addr;
	char send_buffer[1024];
	time_t ticks;

    memset(send_buffer, 0, sizeof(send_buffer));
	memset(&server_addr, 0, sizeof(server_addr));

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(5000);

	//bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if(connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == 0) {
        sprintf(send_buffer, "pjsip_958252:failed;pjsip_958253:failed;");
		write(sockfd, send_buffer, strlen(send_buffer));

        sprintf(send_buffer, "pjsip_958254:failed;pjsip_958255:failed;");
        write(sockfd, send_buffer, strlen(send_buffer));
	}

	close(sockfd);
}