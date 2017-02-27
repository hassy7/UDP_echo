#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main (int argc, char *argv[]) {
	int s, count, datalen;
	in_port_t myport;
	in_port_t port;           //送信相手のポート番号
	struct sockaddr_in myskt;
	struct sockaddr_in skt;   //相手のソケット
	struct in_addr ipaddr;    //送信相手のIPアドレス
	char rbuf[512];
	char sbuf[512];           //送信用のバッファ
	socklen_t sktlen;

	if ((s = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}

	//myportの設定

	bzero (&myskt, sizeof myskt);
	myskt.sin_family = AF_INET;
	myskt.sin_port = htons(myport);
	myskt.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(s, (struct sockaddr *)&myskt, sizeof myskt) < 0) {
		perror("bind");
		exit(1);
	}

	for (;;) {
		sktlen = sizeof skt;
		if ((count = recvfrom(s, rbuf, sizeof rbuf, 0, (struct sockaddr *)&skt, &sktlen)) < 0) {
			perror("recvfrom");
			exit(1);
		}

		skt.sin_family = AF_INET;
		skt.sin_port = htons(port);
		skt.sin_addr.s_addr = htonl(ipaddr.s_addr);
		if ((count = sendto(s, sbuf, datalen, 0, (struct sockaddr *)&skt, sizeof skt)) < 0) {
			perror("sendto");
			exit(1);
		}
	}
}
