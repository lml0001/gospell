#ifndef FPGA_H
#define FPGA_H

#define FPGA_CI_MAJOR	230
#define FPGA_CI_MINOR	0


#define FPGA_IOC_MAGIC  'f'

/*FPGA�ļĴ���ֱ�Ӳ���*/

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
CMD ��ַ�������£�
0X4000   :   CI�������ĵ�ַ
0X4001   ��  CI������������
			Bit15 :  = 1 : read data Effect    0 : read out data no effect
			Bit14..8 :  0x00  : reserved
			Bit7..0 :      data out form CI  
0X4002   ��  CI��������ģʽ 
			Bit[1:0]: �ռ�ѡ��0: Com, 1��attr; 2: io        
			Bit[2]:   ��д���ƣ� 1��R; 0:W
			Bit[4..3]:   ��ѡ��0b00����1��0b01����2�� 0b10 : ��3��0b11:��4��
			���ࣺ����Ϊ0�� 
			0xff: ����
0X4003   �� ����λ���ߵ�ƽ��Ч��
			Bit3..0 : �ֱ����Ӧ��4..1�ĸ�λ��
0X4004   �� ��״̬ (0���п� 1���޿���
			Bit3..0 : �ֱ����Ӧ��4..1��CD��  BIT11..8 : �ֱ����Ӧ��4..1����

0x4005	�� ��������TS���ӿ�
���ӣ�
A. ��дһ������:
Command      Value       ˵��
0x4000        Address     �����ĵ�ַ
0x4001        Data        ����������
0x4002        0x018      ��4д��һ��COM
B. �ӿ��ж�һ������:
Command      Value       ˵��
0x4000        Address     �����ĵ�ַ
0x4002        0x01c      ����4һ��COM
// Wait (1us)  ��п��ܳ���1.5us
Read: 0x4001  	   Data   ::: out data    �������ݵ�ARM

C. �Կ�1��λ:
Command      Value       ˵��
0x4003        0x0         ��0x4003��ַд0
Wait (5us)         
0x4003        0x1         ��0x4003��ַд1
Wait (20us)
0x4003        0x0         ��0x4003��ַд0
D. ����״̬
Command      Return Value      ˵��
0x4004         0x1           ��1��,����Ϊ�п�

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

#define FPGA_CI_CARD_INSERED		0/*0��ʾ�п���1��ʾ�޿�*/
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

