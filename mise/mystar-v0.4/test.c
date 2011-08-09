#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pcap.h>
#include "include/packet_eap.h"
#include "include/pcap_dev.h"
#include "include/blog.h"
#include "include/config.h"


void ctrl_c_op(int signo);

int main(int argc, char *argv[])
{	
	struct sigaction act;//ctrl^c
	act.sa_handler=ctrl_c_op;
	sigemptyset(&act.sa_mask);
	act.sa_flags=0;
	sigaction(SIGINT,&act,NULL);

	//打开网卡
	struct pcap_dev_t * pdev = pcap_get_dev();

	//登录&维持
	login_loop(pdev->handle);
	
	//出错 
	pcap_close(pdev->handle);
	printf("%s %s error\n", what_time(), pdev->dev);
	return -1;
}


//logout in ctrl^c 
void ctrl_c_op(int signo)
{
	struct packet_eap *pkt = get_packet();
	struct pcap_dev_t * pdev = pcap_get_dev();
	//logout
	pcap_sendpacket(pdev->handle,pkt->logoff,18 + pkt->logoff[17]);
	pcap_close(pdev->handle);
	printf("\n%s logout!\n", what_time());
	_exit(1);
}




