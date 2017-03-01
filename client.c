#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <arpa/inet.h>

struct echo_msg {
	unsigned int seq;
	unsigned int reserve;
	char msg[32];
};

int main (int argc, char *argv[]) {
	int s, count, datalen, i;
	in_port_t port = atoi(argv[2]);           //送信相手のポート番号
	struct sockaddr_in myskt;
	struct sockaddr_in skt;   //相手のソケット
	struct in_addr ipaddr;    //送信相手のIPアドレス
	char rbuf[512];
	char sbuf[512];           //送信用のバッファ
	socklen_t sktlen;

	struct echo_msg c_msg;
	c_msg.seq = 0;

	//myportの設定

	//bzero(&myskt, sizeof myskt);
	//myskt.sin_family = AF_INET;
	//myskt.sin_port = htons(myport);
	//myskt.sin_addr.s_addr = htonl(INADDR_ANY);


	for (i = 0; i < 10; i++) {
		if (i < 9) {
			printf("Input message : ");
			fgets(c_msg.msg, 32, stdin);
		} else {
			strcpy(c_msg.msg, "FIN\n");
		}
		if ((s = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
			perror("socket");
			exit(1);
		}

		sktlen = sizeof skt;

		skt.sin_family = AF_INET;
		skt.sin_port = htons(port);
		skt.sin_addr.s_addr = inet_addr(argv[1]);
		if ((count = sendto(s, &c_msg, sizeof c_msg, 0, (struct sockaddr *)&skt, sizeof skt)) < 0) {
			perror("sendto");
			exit(1);
		}
		printf("sended\n");

		if ((count = recvfrom(s, &c_msg, sizeof c_msg, 0, (struct sockaddr *)&skt, &sktlen)) < 0) {
			perror("recvfrom");
			exit(1);
		}
		if (strcmp(c_msg.msg, "FIN\n") == 0) break;
	}
}
