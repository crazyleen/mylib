/*
 * FILE: packet_eap.h
 * client for 802.1x
 *
 */

#include "include/packet_eap.h"
#include "include/md5.h"
#include "include/blog.h"
#include <string.h>
#include <stdio.h>

char username[] = {"cheery"};
char passwd[] = "cheery";

//开始
unsigned char packet_start[]={
	0x01, 0x80, 0xc2, 0x00, 0x00, 0x03,//dst mac
	0x00, 0x23, 0x54, 0x0e, 0xe5, 0xd8,//src mac
	0x88, 0x8e, //Type: 802.1x authentication
		0x01, //Version:v1
		0x01, //Type:  Start (1)
		0x00, 0x00//Length 0
};

/*
//	EAP	Request, Identity [RFC3748](服务器请求发送帐号包)
unsigned char packet_id_r[] = {
	0x00, 0x23, 0x54, 0x0e, 0xe5, 0xd8,//dst mac
	0x00, 0x03, 0x0f, 0x00, 0x6b, 0x56, //src mac
	0x88, 0x8e, //Type: 802.1x authentication
		//Type: 802.1x authentication-----------------------
		0x01, //version: v1 
		0x00, //Type: eap package(0) 
		0x00, 0x05, //length 5 
			//extensile authentication protocol----------------
			0x01,	//code: request (1)
			0x01,	//id:1
			0x00, 0x05, //length 5 
			0x01	//Type: identity[rfc3748] (1)
};
*/

//用户数据包
unsigned char packet_id[PACKET_SIZE] = { 
	0x01, 0x80, 0xc2, 0x00, 0x00, 0x03, //dst mac
	0x00, 0x23, 0x54, 0x0e, 0xe5, 0xd8, //local mac
	0x88, 0x8e, //Type: 802.1x authentication
		//Type: 802.1x authentication-----------------------
		0x01, //version: v1 
		0x00, //Type: eap package(0)
		0x00, 0x0b, //length 11 (length of protocol(follow))
			//extensile authentication protocol----------------
			0x02, //code:response(2)
           	0x01, //id:1
			0x00, 0x0b, //length
			0x01,//Type:  identity[rfc3748] (1)
			0x63, 0x68, 0x65, 0x65, 0x72, 0x79 //identity:cheery（用户名）
};

/*
//EAP	Request, MD5-Challenge [RFC3748]（服务器要求发送密码包）
unsigned char packet_passwd_r[] = {
	0x00, 0x23, 0x54, 0x0e, 0xe5, 0xd8,//dst mac
	0x00, 0x03, 0x0f, 0x00, 0x6b, 0x56, //src mac
	0x88, 0x8e, //Type: 802.1x authentication
		//Type: 802.1x authentication-----------------------
		0x01, //version: v1 
		0x00, //Type: eap package(0) 
		0x00, 0x16, //length 22
			//extensile authentication protocol----------------
			0x01,	//code: request (1)
			0x02,	//id:2
			0x00, 0x16, //length 22 
			0x04,	//Type:  MD5-Challenge [RFC3748] (4)
			0x10,	//Value-Size: 16
			0xf7,0x03,0x91,0x4f,0x6c,0x0d,0x6b,//Value: F703914F6C0D6B6AB79F00A586585CB0
			0x6a,0xb7,0x9f,0x00,0xa5,0x86,0x58,0x5c,0xb0
};
*/

//EAP	Response, MD5-Challenge [RFC3748]（MD5密码包）
unsigned char packet_passwd[PACKET_SIZE] = {
	0x01, 0x80, 0xc2, 0x00, 0x00, 0x03, //dst mac
	0x00, 0x23, 0x54, 0x0e, 0xe5, 0xd8, //local mac
	0x88, 0x8e, //Type: 802.1x authentication
		//Type: 802.1x authentication-----------------------
		0x01, //version: v1 
		0x00, //Type: eap package(0)
		0x00, 0x1c,//Length: 28
			//Extensible Authentication Protocol
			0x02, //Code: Response (2)
			0x02, //Id: 2
			0x00, 0x1c,//Length: 28
			0x04,	//Type:  MD5-Challenge [RFC3748] (4)
			0x10,  	//Value-Size: 16
			0x25, 0xa7, 0x0d, 0x93, 0x36, 0x40, //Value: 0D933640C5FFED1D2F3E3BA2D2AB
			0xc5, 0xff, 0xed, 0x1d, 0x2f, 0x3e, 
			0x3b, 0xa2, 0xd2, 0xab, 
			0x63, 0x68, 0x65, 0x65, 0x72, 0x79	//Extra data (6 bytes): 636865657279（用户名）
};

/*
// EAP	Success(服务器发回验证成功通知）
unsigned char packet_success[] = {
	0x00, 0x23, 0x54, 0x0e, 0xe5, 0xd8,//dst mac
	0x00, 0x03, 0x0f, 0x00, 0x6b, 0x56, //src mac
	0x88, 0x8e, //Type: 802.1x authentication
		//Type: 802.1x authentication-----------------------
		0x01, //version: v1 
		0x00, //Type: eap package(0) 
		0x00, 0x04, //length 4
			//extensile authentication protocol----------------
			0x03,	//Code: Success (3)
			0x00,	//id:0
			0x00, 0x04, //length 4
};


// EAP	failed (服务器发回验证失败通知）
unsigned char packet_failed[] = {
	0x00, 0x23, 0x54, 0x0e, 0xe5, 0xd8,//dst mac
	0x00, 0x03, 0x0f, 0x00, 0x6b, 0x56, //src mac
	0x88, 0x8e, //Type: 802.1x authentication
		//Type: 802.1x authentication-----------------------
		0x01, //version: v1 
		0x00, //Type: eap package(0) 
		0x00, 0x04, //length 4
			//extensile authentication protocol----------------
			0x04,	//Code: failed (4)
			0x00,	//id:0
			0x00, 0x04, //length 4
};
*/
unsigned char packet_logoff[]= {
	0x01, 0x80, 0xc2, 0x00, 0x00, 0x03,//dst mac
	0x00, 0x23, 0x54, 0x0e, 0xe5, 0xd8,//src mac
	0x88, 0x8e, 
		0x01, //version:v1
		0x02, //Type: Logoff (2)
		0x00, 0x00 //Length: 0
};

struct packet_eap packet = {
	.start 		= 	packet_start,
	.id 		= 	packet_id,
	.passwd		=	packet_passwd,
	.logoff		=	packet_logoff
};

struct packet_eap * get_packet(void)
{
	return &packet;
}


/*
 * 802.1x的MD5加密处理（最主要的区别）
 * 返回值：指向密码包的value(16)的指针
 * para1: Request, MD5-Challenge [RFC3748]（服务器要求发送密码包）
 * para2：用户密码
 */
u_char * md5_challenge(const u_char pkt_data[], char passwd[])
{
	u_char  md5_data[256];// = id, passwd, value （smu）
	u_char *p, *q;
	int  md5_len = 1;

	md5_data[0]=pkt_data[19];	//md5_data=id(1)

	//md5_data += passwd(?)
	for(p = md5_data + md5_len, q = passwd; *q != '\0'; ){
		*(p++) = *(q++);		
		md5_len++;
	}

	memcpy(md5_data + md5_len, pkt_data + 24, 16);
	md5_len+=16;

	//密码包的value(16)=hash（id(1) + passwd(?) + value(16)）；
	return ComputeHash(md5_data, md5_len);//point to static u_char array[16]	
}



//==========================================================================================
	//接受处理数据包
int login_loop(pcap_t *pcap_handle)
{
	struct pcap_pkthdr *pkt_header;
	const u_char *pkt_data;//存储数据包的内容
	int pcap_rec;
	int time_cnt = 0;
	int time_max = 500;//设置超时时间
	struct packet_eap *p = get_packet();//数据包

	//start验证
	pcap_sendpacket(pcap_handle,p->start,18 + p->start[17]);
	while ((pcap_rec = pcap_next_ex(pcap_handle,&pkt_header,&pkt_data)) != -1){
		if((pcap_rec == 0) || (pkt_data[12]!=0x88) || (pkt_data[13]!=0x8e)){	
			if(time_cnt++ > time_max){//超时处理
				printf("%s time out!!! need reconnect...\n", what_time());
				time_cnt = 0;
			}
			continue;
		}

        switch(pkt_data[18]){
			case 0x01://code:request
                if ( pkt_data[22]==0x01){ 	//Type: identity[rfc3748] (1)
                    printf("%s response, identity\n", what_time());
                    p->id[19]=pkt_data[19];
                    pcap_sendpacket(pcap_handle,p->id,18 + p->id[17]);   
                }

                if ( pkt_data[22]==0x04){	//Type:  MD5-Challenge [RFC3748] (4)  
					make_passwd_pkt(p->passwd, pkt_data);
                    pcap_sendpacket(pcap_handle,p->passwd,18 + p->passwd[17]);  
					printf("%s resopnse, MD5-challenge\n", what_time());
                }
                break;
           
            case 0x03://code:sussess
                printf("%s %s login successfully\n", what_time(), username);
				printf("%s press ctrl+c to logout...\n", what_time());
                break;

            case 0x04://code:failed
                printf("%s %s login failed. unknow error\n", what_time(), username);
                break;
            }   
       
	}

	return -1;
//==========================================================================================
}

void make_passwd_pkt(u_char packet_passwd[], const u_char pkt_data[])
{
	u_char *md5_value;    // = hash(md5_data)

	//密码包的value(16)=hash（id(1) + passwd(?) + value(16)）；
	md5_value = md5_challenge(pkt_data, passwd);

	//packet_passwd[24:40] = md5_value
	memcpy(packet_passwd + 24, md5_value, 16);					

	//Length
	packet_passwd[17]=22+strlen(username);
	packet_passwd[21]=packet_passwd[17];
	
	//Id: 2
	packet_passwd[19]=pkt_data[19];

	//packet_passwd[40:?] = username;
	strcpy((char *)(packet_passwd + 40), username);

	return;
}








