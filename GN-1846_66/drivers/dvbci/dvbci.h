#ifndef FPGA_H
#define FPGA_H

#define FPGA_CI_MAJOR	230
#define FPGA_CI_MINOR	0


#define FPGA_IOC_MAGIC  'f'

/*FPGA的寄存器直接操作*/

#define FPGA_CI_OP_ADDR					0x4000
#define FPGA_CI_OP_DATA					0x4001
#define FPGA_CI_OP_MODE					0x4002
#define FPGA_CI_OP_RESET				0x4003
#define FPGA_CI_OP_SLOT_STATUS			0x4004

#define FPGA_CI_OP_MODE_SPACE_ATTR		0x01
#define FPGA_CI_OP_MODE_SPACE_IO		0x02

#define FPGA_CI_OP_MODE_W				0x04
#define FPGA_CI_OP_MODE_R				0x00

#define FPGA_CI_OP_MODE_SLOT0			0x00
#define FPGA_CI_OP_MODE_SLOT1			0x08
#define FPGA_CI_OP_MODE_SLOT2			0x10
#define FPGA_CI_OP_MODE_SLOT3			0x20


#define FPGA_CI_OP_RESET_CLEAR			0x00
#define FPGA_CI_OP_RESET_SLOT0			0x01
#define FPGA_CI_OP_RESET_SLOT1			0x02
#define FPGA_CI_OP_RESET_SLOT2			0x04
#define FPGA_CI_OP_RESET_SLOT3			0x08

/*
CMD 地址定义如下：
0X4000   :   CI卡操作的地址
0X4001   ：  CI卡操作的数据
			Bit15 :  = 1 : read data Effect    0 : read out data no effect
			Bit14..8 :  0x00  : reserved
			Bit7..0 :      data out form CI  
0X4002   ：  CI卡操作的模式 
			Bit[1:0]: 空间选择：0: Com, 1：attr; 2: io        
			Bit[2]:   读写控制： 1：R; 0:W
			Bit[4..3]:   卡选择：0b00：卡1；0b01：卡2； 0b10 : 卡3；0b11:卡4；
			其余：保留为0； 
			0xff: 空闲
0X4003   ： 卡复位（高电平有效）
			Bit3..0 : 分别相对应卡4..1的复位；
0X4004   ： 卡状态 (0：有卡 1：无卡）
			Bit3..0 : 分别相对应卡4..1的CD；  BIT11..8 : 分别相对应卡4..1出错

0x4005	： 启动卡的TS流接口
例子：
A. 向卡写一个数据:
Command      Value       说明
0x4000        Address     操作的地址
0x4001        Data        操作的数据
0x4002        0x018      向卡4写入一个COM
B. 从卡中读一个数据:
Command      Value       说明
0x4000        Address     操作的地址
0x4002        0x01c      读卡4一个COM
// Wait (1us)  最长有可能超过1.5us
Read: 0x4001  	   Data   ::: out data    读出数据到ARM

C. 对卡1复位:
Command      Value       说明
0x4003        0x0         向0x4003地址写0
Wait (5us)         
0x4003        0x1         向0x4003地址写1
Wait (20us)
0x4003        0x0         向0x4003地址写0
D. 读卡状态
Command      Return Value      说明
0x4004         0x1           卡1无,其他为有卡

*/

#define FPGA_CI_IOC_CMD_WRITE 			_IO(FPGA_IOC_MAGIC, 1)
#define FPGA_CI_IOC_CMD_READ 			_IO(FPGA_IOC_MAGIC, 2)

typedef struct fpga_ci_cmd
{
	unsigned int	offset;
	unsigned short	value;
}FPGA_CI_CMD;


#define FPGA_CI_IOC_ATTRIBUTE 		_IO(FPGA_IOC_MAGIC, 3)

typedef struct fpga_ci_attr
{
	unsigned int slot;
	unsigned int bread;
	unsigned int offset;
	unsigned int value;
}FPGA_CI_ATTR;

#define FPGA_CI_IOC_IO		 		_IO(FPGA_IOC_MAGIC, 4)

typedef struct fpga_ci_io
{
	unsigned int slot;
	unsigned int bread;
	unsigned int offset;
	unsigned int value;
}FPGA_CI_IO;

#define FPGA_CI_IOC_RESET	 		_IO(FPGA_IOC_MAGIC, 5)

typedef struct fpga_ci_reset
{
	unsigned int slot;
	unsigned int value;
}FPGA_CI_RESET;

#define FPGA_CI_IOC_STATUS	 		_IO(FPGA_IOC_MAGIC, 6)

#define FPGA_CI_CARD_INSERED		0/*0表示有卡，1表示无卡*/
#define FPGA_CI_CARD_INSERED_SLOT0	(0x0001)
#define FPGA_CI_CARD_INSERED_SLOT1	(0x0002)
#define FPGA_CI_CARD_INSERED_SLOT2	(0x0004)
#define FPGA_CI_CARD_INSERED_SLOT3	(0x0008)

#define FPGA_CI_CARD_ERROR_SLOT0	(0x0080)
#define FPGA_CI_CARD_ERROR_SLOT1	(0x0100)
#define FPGA_CI_CARD_ERROR_SLOT2	(0x0200)
#define FPGA_CI_CARD_ERROR_SLOT3	(0x0400)

#define FPGA_IOC_MAXNR 	6

typedef struct fpga_ci_status
{
	unsigned int slot;
	unsigned int value;
}FPGA_CI_STATUS;

#endif

