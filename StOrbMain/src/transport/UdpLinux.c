/*
 * $Id: UdpLinux.c 1063 2009-02-09 02:24:33Z hideki $
 * Copyright (C) 2008 AIST, National Institute of Advanced Industrial Science and Technology
 */

#ifdef SOCKET_CHANNEL_USE_UDP_LINUX
#include "UdpLinux.h"

struct sockaddr_in addr;
int sock = 0;
char rcv_buff[BUF_SIZE];
static int rcv_len = 0, current = 0;

void UdpLinux_initialize() {
#ifdef UDP_DEBUG
	printf("Udp_initialize\n");
#endif
		
	if (sock == 0) {
		sock = socket(AF_INET, SOCK_DGRAM, 0);

		addr.sin_family = AF_INET;
		addr.sin_port = htons(UDP_PORT);
		addr.sin_addr.s_addr = INADDR_ANY;

		bind(sock, (struct sockaddr *)&addr, sizeof(addr));
	}
}

unsigned int UdpLinux_send(unsigned char* start, unsigned int length) {
	unsigned int sndlen;
#ifdef UDP_DEBUG
	int i;

	printf("Udp_send\n");
#endif

	sndlen = sendto(sock, start, length, 0, (struct sockaddr*)&addr, sizeof(addr));

#ifdef UDP_DEBUG
	printf("send data = ");
	for (i = 0; i < sndlen; i++)
		printf("%02x ", start[i]);
	printf("\nlength = %d\n", sndlen);
#endif

	return sndlen;
}

unsigned int UdpLinux_recv(unsigned char* start, unsigned int length) {
	int len = 0;
	int addrlen;
#ifdef UDP_DEBUG
	int i;

	printf("Udp_recv\n");
#endif

	if (rcv_len <= 0) {
		addrlen = sizeof(addr);
		len = recvfrom(sock, rcv_buff, BUF_SIZE, 0, (struct sockaddr*)&addr, &addrlen);
		current = 0;
	}

	memcpy(start, &rcv_buff[current], length);
	rcv_len = len - length;
	current += length;

#ifdef UDP_DEBUG
	printf("recv data = ");
	for (i = 0; i < length; i++)
		printf("%02x ", start[i]);
	printf("\nlength = %d\n", length);
#endif

	return length;
}
#endif

