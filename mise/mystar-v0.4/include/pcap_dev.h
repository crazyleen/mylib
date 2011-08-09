/*
 * pcap_dev.h
 * crazyleen <357228736@qq.com>
 */

#ifndef PCAP_DEV_H
#define PCAP_DEV_H
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pcap.h>

struct pcap_dev_t {
	char *dev;
	pcap_t *handle;//pcap���������
	char errbuf[PCAP_ERRBUF_SIZE];
};

//׼�����豸
struct pcap_dev_t * pcap_get_dev(void);

#endif

