#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>

struct echo_msg {
	unsigned int seq;
	unsigned int reserve;
	char msg[32];
};

int main (int argc, char *argv[]) {
	int s, count, datalen;
	in_port_t port = atoi(argv[1]);           //送信相手のポート番号
	struct sockaddr_in myskt; //自ソケット
	struct sockaddr_in skt;   //相手のソケット
	struct in_addr ipaddr;    //送信相手のIPアドレス
	char rbuf[512];           //受信用バッファ
	char sbuf[512];           //送信用のバッファ
	socklen_t sktlen;
	struct echo_msg s_msg;

	if ((s = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}

	//portの設定

	bzero(&myskt, sizeof myskt);
	myskt.sin_family = PF_INET;
	myskt.sin_port = htons(port);
	myskt.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(s, (struct sockaddr *)&myskt, sizeof myskt) < 0) {
		perror("bind");
		exit(1);
	}

	for (;;) {
		sktlen = sizeof skt;
		printf("waiting\n");
		if ((count = recvfrom(s, &s_msg, sizeof s_msg, 0, (struct sockaddr *)&skt, &sktlen)) < 0) {
			perror("recvfrom");
			exit(1);
		}
		s_msg.seq = s_msg.seq + 1;
		printf("receive\n");
		printf("seq : %d, msg : %s\n", s_msg.seq, s_msg.msg);
		//if ((count = sendto(s, sbuf, datalen, 0, (struct sockaddr *)&skt, sizeof skt)) < 0) {
		if ((count = sendto(s, &s_msg, sizeof s_msg, 0, (struct sockaddr *)&skt, sizeof skt)) < 0) {
		perror("sendto");
			exit(1);
		}
		printf("%s\n", s_msg.msg);
		if (s_msg.seq >= 10 || strcmp(s_msg.msg, "FIN\n") == 0) break;
	}
	close(s);
	exit(1);
}
