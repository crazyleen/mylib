/*
 * FILE: packet_eap.h
 * client for 802.1x
 *
 */

#ifndef PACKET_802_1x_H
#define PACKET_802_1x_H
#include <pcap.h>
#include <sys/types.h>
#define PACKET_SIZE 64



struct packet_eap {
	unsigned char *start;
	unsigned char *id;
	unsigned char *passwd;
	unsigned char *logoff;
};

struct packet_eap * get_packet(void);
//802.1x的MD5加密处理（最主要的区别）
u_char * md5_challenge(const u_char pkt_data[], char passwd[]);
void make_passwd_pkt(u_char packet_passwd[], const u_char pkt_data[]);
int login_loop(pcap_t *pcap_handle);
#endif
