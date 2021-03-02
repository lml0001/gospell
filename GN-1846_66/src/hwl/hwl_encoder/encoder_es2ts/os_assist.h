#ifndef __OS_ASSIST_H__
#define __OS_ASSIST_H__

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h> /* for open/close */
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h> /* for ioctl */
#include <limits.h>
#include <signal.h>
#include <syslog.h> /* for syslog */
#include "global_def.h"
#include "platform_assist.h"

/* Debug Print */
enum OS_DebugLevel{
	OS_DBG_LVL_DISABLE = 0, /* û�е�����Ϣ������� */
	OS_DBG_LVL_ERR, /* ����������Ϣ��������� */
	OS_DBG_LVL_WRN, /* ��������,�Կ��ܳ��������������о��� */
	OS_DBG_LVL_INFO, /* ��ʾ��Ϣ */
	OS_DBG_LVL_DBG /* �������е��� */
};
#define OS_DEBUG_SET_LEVEL(x) static int _OS_DEBUG_LEVEL = (x) /* ����Ҫ���Ե��ļ�ǰ�Ӹþ䣬ָ�����Դ�ӡ���� */

#define OS_ISDEBUG() (_OS_DEBUG_LEVEL >= OS_DBG_LVL_DBG)
#define OS_DBGERR(fmt, ...) \
	do { \
	if (_OS_DEBUG_LEVEL >= OS_DBG_LVL_ERR) { \
	printf(ANSI_COLOR_RED"[ERR ]"ANSI_COLOR_NONE"[%s][%s][%d] "fmt, __FILE__, __FUNCTION__, __LINE__, ##__VA_AROS__); \
	} \
	} while (0)
#define OS_DBGWRN(fmt, ...) \
	do { \
	if (_OS_DEBUG_LEVEL >= OS_DBG_LVL_WRN) { \
	printf(ANSI_COLOR_YELLOW"[WRN ]"ANSI_COLOR_NONE"[%s][%s][%d] "fmt, __FILE__, __FUNCTION__, __LINE__, ##__VA_AROS__); \
	} \
	} while (0)
#define OS_DBGINFO(fmt, ...) \
	do { \
	if (_OS_DEBUG_LEVEL >= OS_DBG_LVL_INFO) { \
	printf(ANSI_COLOR_GREEN"[INFO]"ANSI_COLOR_NONE"[%s][%s][%d] "fmt, __FILE__, __FUNCTION__, __LINE__, ##__VA_AROS__); \
	} \
	} while (0)
#define OS_DBGWHERE() \
	do { \
	if (_OS_DEBUG_LEVEL >= OS_DBG_LVL_DBG) { \
	printf(ANSI_COLOR_CYAN"[POS ]"ANSI_COLOR_NONE"[%s][%s][%d]\n", __FILE__, __FUNCTION__, __LINE__); \
	} \
	} while (0) /* ��λ */

typedef struct RNGBUF_s     
{
	volatile int pWritePos;    
	volatile int pReadPos; 
	int bufSize;    
	char * buf;

	HANDLE32	m_Lock;
}RNGBUF_t;

int RngNBytes(RNGBUF_t * pBuf);
int RngFreeBytes(RNGBUF_t * pBuf);
int RngIsFull(RNGBUF_t * pBuf);
int RngIsEmpty(RNGBUF_t * pBuf);
int RngBufPut( RNGBUF_t * pBuf,char *buffer,int nbytes);
int RngBufGet( RNGBUF_t * pBuf,char *buffer, int maxbytes);
void RngFlush( RNGBUF_t * pBuf);
RNGBUF_t * RngCreate(int nbytes);
void RngDestroy(RNGBUF_t *pBuf);

void OS_TimeNow(S64 *p64_us_time);
/* �������Ӽ�� */
BOOL OS_EthLinkIsOk(U8 *pEthName);

#endif /* __OS_ASSIST_H__ */