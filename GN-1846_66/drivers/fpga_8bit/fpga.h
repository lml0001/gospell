#ifndef FPGA_H
#define FPGA_H

#define FPGA_MAJOR	211
#define FPGA_MINOR	1

/**	�ڲ���������С.�û��ṩ��������С����ΪMEM_BUFSIZE.*/
#define FPGA_IO_BUFF_SIZE		  (1024)   
#define FPGA_IO_MAX_PAYLOAD_SIZE  (1020)   

#define FPGA_IOC_MAGIC  'f'

/**	fpga-IO�ռ����ַ.*/
#define FPGA_BASE_ADDRESS			0x60000000//���������FPGAʹ�ã�ʹ��NCS5

/** ��������.*/
#define FPGA_IOC_CLEAR 	_IO(FPGA_IOC_MAGIC,   1)

/**	�������*/
#define FPGA_IOC_ADD 	_IOW(FPGA_IOC_MAGIC,  2, struct fpga_cmd)

/**	��������.*/
#define FPGA_IOC_SEND 	_IO(FPGA_IOC_MAGIC,   3)


#define FPGA_IOC_MAXNR 	3

#define FPGA_CMD_NUM	600

/**
 * @(addr):ƫ�Ƶ�ַ.
 * @(value):ֵ.
 * ˫�ֽ�Ϊ��λ.
 *    addr:0x0000  value:0x0000
 *    addr:0x0001  value:0x0000
 *    addr:0x0002  value:0x0000
 *		
 */
struct fpga_cmd
{
	unsigned short addr;
	unsigned short value;
};

#endif

