/******************************************************************************/
/**
*@file    W5300.h      
*@brief   header of device W5300 driver                     
*@author  yangbm
*@version V1.0.0
*@date    2009.03.11
*@details v1.0.0  2009.03.11 yangbm create */
/******************************************************************************/
#ifndef __W5300_H__
#define __W5300_H__

/*****************************************************************************/
/**@brief   macro                                                    	     */
/*****************************************************************************/
#define ETHERNET_MAX_NUM 		1							/*网络芯片数量*/
#define MAC_ADDR_12				0x4853						/*MAC地址低2字节值*/
#define W5300_RST_BIT       	0x0002						/*W5300复位屏蔽位*/
#define MTU_DEFAULT_W5300   	512							/*网络驱动层最大MTU*/
#define MASK_SUBNET				0xFFFFFF00					/*子网掩码*/
#define W5300_MAP_ADDRESS		0x101000L					/*W5300基地址*/
#define COMMON_REG_BASE     	W5300_MAP_ADDRESS			/*通用寄存器基地址*/	
#define SOCKET_REG_BASE     	W5300_MAP_ADDRESS+0x0200	/*SOCKET寄存器基地址*/	
#define SOCKET_REV_INT_BIT  	0x04					    /*SOCKET接收中断屏蔽位*/	
#define SOCKET_SENOK_TIMEROUT_BIT  	0x18					/*SOCKET发送结束与超时中断屏蔽位*/	
#define SOCKET_REG_SIZE     	0x40						/*SOCKET寄存器组大小*/								
#define W5300_INT_START_BIT 	4							/*网络中断在中断机制中的起始位置*/
#define	SPACE_ADDR_W5300   		0x400						/*网络芯片间的地址空间*/
/** Maxmium number of socket */
#define	MAX_SOCK_NUM		8

/* Mode Register address */
#define MR              (W5300_MAP_ADDRESS)
/*
#define MR0             MR
#define MR1             (MR + 1)
*/

/* MODE register values */
#define MR_FIFOSWAP        0x100 // FIFO SWAP
#define MR_WR_TIMING(X)    ((X & 0x07) << 11)
#define MR_SAMPLING(X)     ((X & 0x03) << 9)
#define MR_RST             0x80 // reset
#define MR_MEMTEST         0x20 // memory test
#define MR_PB              0x10 // ping block
#define MR_PPPOE           0x08 // enable pppoe
#define MR_LB              0x04 // little or big endian selector in indirect mode
#define MR_IND             0x01 // enable indirect mode


#define IDM_AR(i)          (W5300_MAP_ADDRESS + SPACE_ADDR_W5300*(i) + 0x02)
/*
#define IDM_AR0         IDM_AR
#define IDM_AR1         (IDM_AR + 1)
*/

#define IDM_DR(i)          (W5300_MAP_ADDRESS + SPACE_ADDR_W5300*(i) + 0x04)
/*
#define IDM_DR0         IDM_DR
#define IDM_DR1         (IDM_DR + 1)
*/

/* Interrupt Register */
#define IR(i)              (COMMON_REG_BASE + SPACE_ADDR_W5300*(i)+ 0x02)


/* Interrupt mask register */
#define IMR(i)             (COMMON_REG_BASE + SPACE_ADDR_W5300*(i) + 0x04)


/* Interrupt De-asserted Time register */
#define ICFGR(i)          (COMMON_REG_BASE + SPACE_ADDR_W5300*(i) + 6)


/* Source MAC Register address */
#define SHAR(i)            (COMMON_REG_BASE + SPACE_ADDR_W5300*(i) + 8)
/*
#define SHAR0           SHAR
#define SHAR1           (SHAR + 1)
#define SHAR2           (SHAR + 2)
#define SHAR3           (SHAR + 3)
#define SHAR4           (SHAR + 4)
#define SHAR5           (SHAR + 5)
*/

/* Gateway IP Register address */
#define GAR(i)             (COMMON_REG_BASE + SPACE_ADDR_W5300*(i) + 0x10)

/* Subnet mask Register address */
#define SUBR(i)            (COMMON_REG_BASE + SPACE_ADDR_W5300*(i)  + 0x14)


/* Source IP Register address */
#define SIPR(i)            (COMMON_REG_BASE + SPACE_ADDR_W5300*(i)  + 0x18)


/* Timeout register address
1 is 100us */
#define RTR(i)             (COMMON_REG_BASE + SPACE_ADDR_W5300*(i) + 0x1C)


/* Retry count reigster */
#define RCR(i)             (COMMON_REG_BASE + SPACE_ADDR_W5300*(i) + 0x1E)


/* Transmit memory size reigster */
#define TMS01R(i)          (COMMON_REG_BASE + SPACE_ADDR_W5300*(i) + 0x20)
#define TMS23R(i)          (TMS01R(i) + 2)
#define TMS45R(i)          (TMS01R(i) + 4)
#define TMS67R(i)          (TMS01R(i) + 6)


/* Receive memory size reigster */
#define RMS01R(i)          (COMMON_REG_BASE + SPACE_ADDR_W5300*(i) + 0x28)
#define RMS23R(i)          (RMS01R(i) + 2)
#define RMS45R(i)          (RMS01R(i) + 4)
#define RMS67R(i)          (RMS01R(i) + 6)



/** Memory Type Register
'1' - TX memory
'0' - RX memory
**/
#define MTYPER(i)          (COMMON_REG_BASE+ SPACE_ADDR_W5300*(i) + 0x30)


/* Authentication type register address in PPPoE mode */
#define PATR(i)            (COMMON_REG_BASE + SPACE_ADDR_W5300*(i) + 0x32)

#define PPPALGOR(i)      (COMMON_REG_BASE + SPACE_ADDR_W5300*(i) + 0x34)

#define PTIMER(i)          (COMMON_REG_BASE + SPACE_ADDR_W5300*(i) + 0x36)

#define PMAGICR(i)         (COMMON_REG_BASE + SPACE_ADDR_W5300*(i) + 0x38)

#define PSTATER(i)       (COMMON_REG_BASE + SPACE_ADDR_W5300*(i) + 0x3A)

#define PSIDR(i)           (COMMON_REG_BASE + SPACE_ADDR_W5300*(i) + 0x3C)

#define PDHAR(i)           (COMMON_REG_BASE + SPACE_ADDR_W5300*(i)+ 0x40)

/* Unreachable IP register address in UDP mode */
#define UIPR(i)            (COMMON_REG_BASE + SPACE_ADDR_W5300*(i) + 0x48)

/* Unreachable Port register address in UDP mode */
#define UPORTR(i)          (COMMON_REG_BASE + SPACE_ADDR_W5300*(i) + 0x4C)

/* Fragment Register */
#define FMTUR(i)           (COMMON_REG_BASE SPACE_ADDR_W5300*(i) + 0x4E)

/* Retransmitted Count Register of SOCKET n */
#define  SN_RTCR(i,n)      (COMMON_REG_BASE + SPACE_ADDR_W5300*(i) + 0x50 + (n))

/* Buffer Ready Config Register of BRDYn PIN (0 <= n <= 3) */
#define PN_BRDYR(i, n)     (COMMON_REG_BASE + SPACE_ADDR_W5300*(i) + 0x60 + n*4)

/* Buffer Depth Config Register of BRYNn PIN (0 <= n <= 3) */
#define PN_BDPTHR(i, n)    (COMMON_REG_BASE + SPACE_ADDR_W5300*(i) + 0x60 + n*4 + 2)

/* W5300 ID register */
#define IDR(i)             (COMMON_REG_BASE + SPACE_ADDR_W5300*(i) + 0xFE)

/********************************* socket register ****************************/
/* Mode register of SOCKET n */
#define SN_MR(i,n)        (SOCKET_REG_BASE + SPACE_ADDR_W5300*(i) + (n) * SOCKET_REG_SIZE + 0x00)

/* Command register of SOCKET n */
#define SN_CR(i, n)        (SOCKET_REG_BASE + SPACE_ADDR_W5300*(i) + (n) * SOCKET_REG_SIZE + 0x02)

/* Interrupt mask register of SOCKET n */
#define SN_IMR(i, n)       (SOCKET_REG_BASE + SPACE_ADDR_W5300*(i) + (n) * SOCKET_REG_SIZE + 0x04)

/* Interrupt register of SOCKET n */
#define SN_IR(i, n)        (SOCKET_REG_BASE + SPACE_ADDR_W5300*(i) + (n) * SOCKET_REG_SIZE + 0x06)

/* socket status register */
#define SN_SSR(i,n)       (SOCKET_REG_BASE + SPACE_ADDR_W5300*(i) + (n) * SOCKET_REG_SIZE + 0x08)

/* source port register */
#define SN_PORTR(i,n)     (SOCKET_REG_BASE + SPACE_ADDR_W5300*(i) + (n) * SOCKET_REG_SIZE + 0x0A)

/* Peer MAC register address */
#define SN_DHAR(i,n)      (SOCKET_REG_BASE + SPACE_ADDR_W5300*(i) + (n) * SOCKET_REG_SIZE + 0x0C)

/* Destination port register of SOCKET n */
#define SN_DPORTR(i,n)    (SOCKET_REG_BASE + SPACE_ADDR_W5300*(i) + (n) * SOCKET_REG_SIZE + 0x12)

/* Destination IP register of SOCKET n */
#define SN_DIPR(i,n)      (SOCKET_REG_BASE + SPACE_ADDR_W5300*(i) + (n) * SOCKET_REG_SIZE + 0x14)

/* Maximum Segment Size register of SOCKET n */
#define SN_MSSR(i,n)      (SOCKET_REG_BASE + SPACE_ADDR_W5300*(i) + (n) * SOCKET_REG_SIZE + 0x18)

/* Protocol of IP Header field register of SOCKET n */
#define SN_PROTOR(i,n)		(SOCKET_REG_BASE + SPACE_ADDR_W5300*(i) + (n) * SOCKET_REG_SIZE + 0x1A)

/* IP Type of Service(TOS) Register of SOCKET n */
#define SN_TOSR(i,n)      (SOCKET_REG_BASE + SPACE_ADDR_W5300*(i) + n * SOCKET_REG_SIZE + 0x1C)

/* IP Time to live(TTL) Register of SOCKET n */
#define SN_TTLR(i,n)       (SOCKET_REG_BASE + SPACE_ADDR_W5300*(i) + (n) * SOCKET_REG_SIZE + 0x1E)

/* Transmit Size Register of SOCKET n (Byte count) */
#define SN_TX_WRSR(i,n)		(SOCKET_REG_BASE + SPACE_ADDR_W5300*(i) + (n) * SOCKET_REG_SIZE + 0x20)

/* Transmit free memory size register of SOCKET n (Byte count) */
#define SN_TX_FSR(i,n)       (SOCKET_REG_BASE + SPACE_ADDR_W5300*(i) + (n) * SOCKET_REG_SIZE + 0x0024)

/* Received data size register of SOCKET n (Byte count) */
#define SN_RX_RSR(i, n)       (SOCKET_REG_BASE + SPACE_ADDR_W5300*(i) + (n) * SOCKET_REG_SIZE + 0x0028)

#define SN_FRAGR(i,n)        (SOCKET_REG_BASE + SPACE_ADDR_W5300*(i) + (n)* SOCKET_REG_SIZE + 0x002C)

/* FIFO register of SOCKET n, For data transmittion */
#define SN_TX_FIFOR(i,n)     (SOCKET_REG_BASE + SPACE_ADDR_W5300*(i) + (n) * SOCKET_REG_SIZE + 0x2E)

/* FIFO register of SOCKET n, For data reception */
#define SN_RX_FIFOR(i,n)     (SOCKET_REG_BASE + SPACE_ADDR_W5300*(i) + (n) * SOCKET_REG_SIZE + 0x30)

/* TX Memory Base Address Register of SOCKET n */
#define SN_TX_SADR(i,n)      (SOCKET_REG_BASE + SPACE_ADDR_W5300*(i) + (n) * SOCKET_REG_SIZE + 0x32)

/* RX Memory Base Address Register of SOCKET n */
#define SN_RX_SADR(i,n)      (SOCKET_REG_BASE + SPACE_ADDR_W5300*(i) + (n) * SOCKET_REG_SIZE + 0x34)

/* TX memory read pointer register of SOCKET n */
#define SN_TX_RD(i,n)        (SOCKET_REG_BASE + SPACE_ADDR_W5300*(i) + (n) * SOCKET_REG_SIZE + 0x36)

/* TX memory write pointer register of SOCKET n */
#define SN_TX_WR(i,n)        (SOCKET_REG_BASE + SPACE_ADDR_W5300*(i) + (n) * SOCKET_REG_SIZE + 0x38)

/* TX memory ack pointer register of SOCKET n */
#define SN_TX_ACK(i,n)       (SOCKET_REG_BASE + SPACE_ADDR_W5300*(i) + (n) * SOCKET_REG_SIZE + 0x3A)

/* RX memory read pointer register of SOCKET n */
#define SN_RX_RD(i,n)		   (SOCKET_REG_BASE + SPACE_ADDR_W5300*(i) + (n) * SOCKET_REG_SIZE + 0x3C)

/* RX memory write pointer of SOCKET n */
#define SN_RX_WR(i,n)        (SOCKET_REG_BASE + SPACE_ADDR_W5300*(i) + (n) * SOCKET_REG_SIZE + 0x3E)

/* IR register values */
#define IR_CONFLICT			0x80 /* check ip confict*/
#define IR_UNREACH			0x40 /* get the destination unreachable message in UDP sending*/
#define IR_PPPoE			0x20 /* get the PPPoE close message*/
#define IR_FMTU				0x10
#define IR_SOCK(ch)			(0x01 << ch) /* check socket interrupt*/

/* SN_MR values */
#define SN_MR_ALIGN        0x100 /*2 Byte Alignment Data Trnascation(No use TCP Header)*/
#define SN_MR_MULTI        0x80 /* support multicating*/	
#define SN_MR_ND           0x20 /* No Delayed Ack(TCP) flag*/



#define SN_MR_CLOSE        0x00 /* unused socket*/
#define SN_MR_TCP          0x01 /* TCP*/
#define SN_MR_UDP          0x02 /* UDP*/
#define SN_MR_IPRAW        0x03 /* IP LAYER RAW SOCK*/
#define SN_MR_MACRAW       0x04 /* MAC LAYER RAW SOCK*/
#define SN_MR_PPPOE        0x05 /* PPPoE*/


/* SN_CR values */
#define SN_CR_OPEN         0x01 /* initialize or open socket*/
#define SN_CR_LISTEN       0x02 /* wait connection request in tcp mode(Server mode)*/
#define SN_CR_CONNECT      0x04 /* send connection request in tcp mode(Client mode)*/
#define SN_CR_DISCON       0x08 /* send closing reqeuset in tcp mode*/
#define SN_CR_CLOSE        0x10 /* close socket*/
#define SN_CR_SEND         0x20 /* updata txbuf pointer, send data*/
#define SN_CR_SEND_MAC     0x21 /* send data with MAC address, so without ARP process*/
#define SN_CR_SEND_KEEP    0x22 /* send keep alive message*/
#define SN_CR_RECV         0x40 /* update rxbuf pointer, recv data*/

#define SN_CR_PCON      0x23  
#define SN_CR_PDISCON   0x24  
#define SN_CR_PCR       0x25  
#define SN_CR_PCN       0x26  
#define SN_CR_PCJ       0x27 

/* SN_IR values */
#define SN_IR_PRECV     0x80 
#define SN_IR_PFAIL     0x40  
#define SN_IR_PNEXT     0x20 
                         
#define SN_IR_SEND_OK      0x10 /* complete sending*/
#define SN_IR_TIMEOUT      0x08 /* assert timeout*/
#define SN_IR_RECV         0x04 /* receiving data*/
#define SN_IR_DISCON       0x02 /* closed socket*/
#define SN_IR_CON          0x01 /* established connection*/

/* SN_SSR values */
#define SOCK_CLOSED        0x00 /* closed*/
#define SOCK_INIT          0x13 /* init state*/
#define SOCK_LISTEN        0x14 /* listen state*/
#define SOCK_SYNSENT       0x15 /* connection state*/
#define SOCK_SYNRECV       0x16 /* connection state*/
#define SOCK_ESTABLISHED   0x17 /* success to connect*/
#define SOCK_FIN_WAIT      0x18 /* closing state*/
#define SOCK_CLOSING       0x1A /* closing state*/
#define SOCK_TIME_WAIT     0x1B /* closing state*/
#define SOCK_CLOSE_WAIT    0x1C /* closing state*/
#define SOCK_LAST_ACK      0x1D /* closing state*/
#define SOCK_UDP           0x22 /* udp socket*/
#define SOCK_IPRAW         0x32 /* ip raw mode socket*/
#define SOCK_MACRAW        0x42 /* mac raw mode socket*/
#define SOCK_PPPOE         0x5F /* pppoe socket*/

/* IP PROTOCOL */
#define IPPROTO_IP         0   /* Dummy for IP */
#define IPPROTO_ICMP       1   /* Control message protocol */
#define IPPROTO_IGMP       2   /* Internet group management protocol */
#define IPPROTO_GGP        3   /* Gateway^2 (deprecated) */
#define IPPROTO_TCP        6   /* TCP */
#define IPPROTO_PUP        12  /* PUP */
#define IPPROTO_UDP        17  /* UDP */
#define IPPROTO_IDP        22  /* XNS idp */
#define IPPROTO_ND         77  /* UNOFFICIAL net disk protocol */
#define IPPROTO_RAW        255 /* Raw IP packet */


/* Buffer Ready Pin Config Value*/
#define Pn_BRDYR_EN        (1<<7)
#define Pn_BRDYR_TX        (1<<6)
#define Pn_BRDYR_HIGH      (1<<5)
#define Pn_BRDYR_CH(n)     (n & 0x07)
#define   W5300_NUM_MAX		2
#define	  W5300_SOCK_NUM	8
/*****************************************************************************/
/**@brief   Sock地址结构                                                   	     */
/*****************************************************************************/
typedef   struct  tagSockAddr
{
	uint16_t  sin_family;  /*协议族*/    /*应该是决定是udp，还是TCP协议 20170808*/
    uint16_t  sin_port;  /*端口号*/
    uint32_t  sin_addr;  /*网络IP地址*/
}SSockAddr;	

#define   W300_REV_BUF_LEN		512/*接收BUF长度*/
/*****************************************************************************/
/**@brief   接收存储区结构                                                   	     */
/*****************************************************************************/
typedef   struct  tagW5300RevBuf
{
	uint32_t  ui_Addr;						/*接收数据网络地址*/	
	uint16_t  us_Len;						/*接收数据长度*/
	uint8_t   auc_Buf[W300_REV_BUF_LEN];	/*数据存储区*/
}W5300RevBuf;

#define  MAX_W5300_QUEUE_LEN		24	/*接收队列长度*/
/*****************************************************************************/
/**@brief   接收队列结构                                                   	     */
/*****************************************************************************/
typedef  struct  tagW5300Queue
{
	uint8_t  uc_Hdr;							/*队列头*/
	uint8_t  uc_Tail;							/*队列尾*/
	uint8_t  uc_Cnt;							/*队列内数据元素数量*/
	W5300RevBuf  ast_Buf[MAX_W5300_QUEUE_LEN];	/*数据存储区*/
	 
}W5300Queue;
/*****************************************************************************/
/**@brief   发送存储区结构                                                   	     */
/*****************************************************************************/
typedef  struct   tagW5300SndBuf
{
	SSockAddr  st_SockAddr;					/*发送数据目的网络地址*/
	uint16_t   us_Len;						/*发送数据长度*/
	uint8_t    auc_Buf[W300_REV_BUF_LEN];	/*数据存储区*/	
}W5300SndBuf;

/*****************************************************************************/
/**@brief   发送队列结构                                                   	     */
/*****************************************************************************/
typedef  struct  tagW5300SndQueue
{
	uint8_t  uc_Hdr;							/*队列头*/
	uint8_t  uc_Tail;							/*队列尾*/
	uint8_t  uc_Cnt;							/*队列内数据元素数量*/
	W5300SndBuf ast_Buf[MAX_W5300_QUEUE_LEN];	/*数据存储区*/	
}SW5300SndQueue;

/*****************************************************************************/
/**@brief   网络接口信息结构                                                   	     */
/*****************************************************************************/
typedef   struct  tagNetInfor
{
	uint8_t  uc_Init;				/*初始化标志*/
	uint8_t  uc_FistSnd;			/*第一次发送标志*/
	W5300Queue  	st_RevQueue;	/*接收队列*/
	SW5300SndQueue  st_SndQueue;	/*发送队列*/
}SNetInfor;

/*****************************************************************************/
/**@brief   模块外部接口                                                     	     */
/*****************************************************************************/
extern  uint8_t W5300_Init(IN uint8_t  uc_Port , IN uint32_t  ui_IpAddr,IN uint32_t ui_GateWay, 
							IN uint32_t ui_Mask, IN uint8_t uc_Ver);

extern  uint8_t  WD5300_SetSocketUDP(IN uint8_t uc_Port, uint8_t uc_SockNum, SSockAddr *pst_SockAddr);

extern  uint8_t W5300_SockSnd(uint8_t  uc_Port, uint8_t uc_SockNum, SSockAddr *pst_SockAddr, 
								uint8_t auc_Buf[],  uint16_t us_Len);
								
extern	uint16_t   W5300_SockRev(uint8_t  uc_Port, uint8_t uc_SockNum, uint32_t*  pui_Addr, 
								 uint8_t auc_Buf[], uint16_t us_Len);


#endif /*W5300_H_*/
