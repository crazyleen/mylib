/*
 *
 * crazyleen <357228736@qq.com>
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pcap.h>
#include "include/pcap_dev.h"

extern char * what_time(void);

//准备好设备
struct pcap_dev_t * pcap_get_dev(void)
{
	static struct pcap_dev_t dev = {
		.dev 	= NULL,
		.handle = NULL,
		.errbuf = ""
	};
	
	if(dev.dev != NULL)
		return &dev;

	//查找网卡
	dev.dev = pcap_lookupdev(dev.errbuf);
	if (dev.dev==NULL){
    	printf("%s find device failed, are you root?\n", what_time());
    	_exit(-1);
	}
	printf("%s find device: %s\n", what_time(), dev.dev);
	
	//打开网络设备，并且返回用于捕获网络数据包的数据包捕获描述字。对于此网络设备的操作都要基于此网络设备描述字。
	dev.handle = pcap_open_live(dev.dev,65536,1, 1000, dev.errbuf);
	if (dev.handle == NULL){
    	printf("%s open %s failed.\n", what_time(), dev.dev);
    	_exit(-1);
	}
    printf("%s open %s\n", what_time(), dev.dev);
	return &dev;
}


