#ifndef __MULTI_HWL_INTERNAL_H__
#define __MULTI_HWL_INTERNAL_H__

#include "platform_conf.h"
#include "multi_private.h"

#define HWL_DEBUG {}
#define HWL_TRACE {}

/* check range */
#define chk_range(begin ,end ,now )  ( !(  (begin)<=(now) &&(now)<=(end) ) )


#define HWL_CONST_MAX_QAM_CHN_NUM				4
#define		HWL_MSG_MAX_SIZE			(1024)



/*TagID*/
#define ICPL_TAGID_HEADBEAT 			0x01
#define ICPL_TAGID_DPB_TS				0x0A
#define ICPL_TAGID_ETH_CHN_PARAM		0x0B
#define ICPL_TAGID_PIDMAP				0x05
#define ICPL_TAGID_INPUT_ETH_SUB_PARAM	0x03
#define ICPL_TAGID_OUTPUT_ETH_SUB_PARAM	0x04
#define ICPL_TAGID_PHY					0x0C
#define ICPL_TAGID_CHANNELRATE			0x02

#define ICPL_TAGID_PIDSTATICS			0x08

#define ICPL_TAGID_IPSTATICS			0x08

#define ICPL_TAGID_CW_SEND				0x06
#define ICPL_TAGID_CW_SWITCH			0x07

#define ICPL_TAGID_CA_ENABLE			0x06
#define ICPL_TAGID_PSI_NUM				0x0D
#define ICPL_TAGID_GROUP_BROADCAST		0x03
#define ICPL_TAGID_PIDCOPY_ORIGINAL			0x0f
#define ICPL_TAGID_PIDCOPY_DESTINED			0x10
#define ICPL_TAGID_GROUP_MODULOR_REGISTER 0x011

#define ICPL_TAGID_PCR_VERYFY			0X12
#define ICPL_TAGID_STREAM_D_NOTIFY		0X13
#define ICPL_TAGID_STREAM_ES_NOTIFY		0X14
#define ICPL_TAGID_PIDMAP_PORT_SET				0X15
#define ICPL_TAGID_PIDMAP_SOURCE_TO_DES_SET	0X16
#define ICPL_TAGID_OUTPUT_RATE_96M_BL85KM		0X17
#define ICPL_TAGID_VOD_INPUT_PARAM		0X18
#define ICPL_TAGID_VODPATCRC			0x09
#define ICPL_TAGID_ENCRYPT			0X20
#define ICPL_TAGID_PASSMODE			0X21
#define ICPL_TAGID_IGMP_MODE		0X22
#define ICPL_TAGID_STABLE_DPB_TS		0X23
#define ICPL_TAGID_MODULE_RESET		0X24

#define ICPL_TAGID_FPGA_ETH_TS		0X31

#define ICPL_TAGID_SFN_STATUS		0X33

#define HWL_CONST__IPV4		0x04
#define HWL_CONST__IPV6		0x06



/**	���ͨ����Ŀ��*/
#define HWL_CONST_CHANNEL_NUM_MAX		256
#define HWL_CONST_SUBCHN_NUM_MAX		250


//#define HWL_CONST_PID_NUM_MAX			255
#define HWL_CONST_IPPORT_NUM_MAX		250




#define HWL_CONST_PIDSOURCE_MAX		125
#define HWL_CONST_PIDDESTINED_MAX		250


/**	Ӳ��ͨ��״̬��ѯӦ��������Ŀ��.*/
#define HWL_CONST_PHYINFO_NUM_MAX  	10

/**	PIDӳ�����õ������Ŀ��.*/
#define HWL_CONST_PIDMAP_NUM_MAX	4096

/**	PID COPY Դ���PIDӳ��������Ŀ�� */
#define HWL_CONST_PIDCOPY_NUM_MAX	512


/*	18.1 CPU�Ե������йص�оƬ�ļĴ������úͶ�ȡ: (PHY_Ch = 0)	*/
#define HWL_CONST_CHIPID_DS1775	0X10
#define HWL_CONST_CHIPID_ADF4360	0X14
#define HWL_CONST_CHIPID_AD5245	0X18
#define HWL_CONST_CHIPID_BL85MM	0X20

#define HWL_CONST_READ		0
#define HWL_CONST_WRITE	1


/*	19.CPU��TS����˿ں�PCR У���MODE ����: (PHY_Ch = 0) */
#define HWL_CONST_VERIFY_YES	1
#define HWL_CONST_VERIFY_NO	0



/**	�������40mS[��]����PCR��*/
#define HWL_CONST_PCR_VERIFY_MODE_40MS_INSERT_YES		1
#define HWL_CONST_PCR_VERIFY_MODE_40MS_INSERT_NO		0


#define HWL_CONST_MODULAR_STAND_J83A		0
#define HWL_CONST_MODULAR_STAND_J83B		1
#define HWL_CONST_MODULAR_STAND_J83C		2


/* 	24.����������ʶ�Ӧ��96MHz��������(BL85KMģ�������Ҫ),   */
#define HWL_CONST_MODULAR_MODE_QPSK		0
#define HWL_CONST_MODULAR_MODE_8QPSK		1
#define HWL_CONST_MODULAR_MODE_16QAM		2
#define HWL_CONST_MODULAR_MODE_32QAM		3
#define HWL_CONST_MODULAR_MODE_64QAM		4
#define HWL_CONST_MODULAR_MODE_128QAM		5
#define HWL_CONST_MODULAR_MODE_256QAM 		6
#define HWL_CONST_MODULAR_MODE_512QAM		7
#define HWL_CONST_MODULAR_MODE_1024QAM		8


/* 	25.VODģʽ������TS����������: */
#define HWL_CONST_VOD		1		//VODģʽ
#define HWL_CONST_MUTOR	0		//���ù㲥��Ŀ


/* 	����оƬ�洢����󳤶�.*/
#define HWL_CONST_ENCRYPT_MAX_BUFF	80




/* 	29. ��DPB���������TS����188����(CPU-��FPGA)  TAG = 0x23   */
#define HWL_CONST_PACKET_ECM		1
#define HWL_CONST_PACKET_PSI		0


#define HWL_SUCCESS		0
#define HWL_FAILED		-1


#define HWL_CONST_REQUEST_TABLE_MAX_SIZE	100
#define HWL_CONST_RESPONSE_TABLE_MAX_SIZE	100




#define SUCCESS				0
#define ERROR				-1
#define ERROR_BAD_PARAM		-2


/*FPGA IIC���*/
#define ICPL_IIC_IC_ID_AD9789_START				0x01
#define ICPL_IIC_IC_ID_DS1775					0x10
#define ICPL_IIC_IC_ID_ADF4360					0x14
#define ICPL_IIC_IC_ID_AD5245					0x18
#define ICPL_IIC_IC_ID_ADF4350					0x15
#define ICPL_IIC_IC_ID_FPGA_DTMB				0x24




#define ICPL_IIC_REG_AD5245						0x00

#define ICPL_IIC_REG_ADF4360_COUNTERLATCH		0x00
#define ICPL_IIC_REG_ADF4360_R_COUNTERLATCH		0x10
#define ICPL_IIC_REG_ADF4360_N_COUNTERLATCH		0x20

#define ICPL_IIC_REG_DS1775_REG_CONF_L			0x01
#define ICPL_IIC_REG_DS1775_REG_CONF_H			0x11

#define ICPL_IIC_REG_DS1775_REG_TEMP_LOW_L		0x02
#define ICPL_IIC_REG_DS1775_REG_TEMP_LOW_H		0x12

#define ICPL_IIC_REG_DS1775_REG_TEMP_HIGHT_L	0x03
#define ICPL_IIC_REG_DS1775_REG_TEMP_HIGHT_H	0x13


#define HWL_AD9789_MODULE_NUM					1
#define HWL_AD9789_MODULE_SUB_CHENNEL_NUM		4

#define HWL_BL85KMM_MODULE_NUM					2
#define HWL_BL85KMM_MODULE_SUB_CHENNEL_NUM		16


#define ICPL_IIC_REG_AD9789_QAM_SP_INFO			0x80
#define ICPL_IIC_REG_AD9789_QBSK_SP_INFO		0x60
#define ICPL_BUFFER_DEVICE_TYPE_BL85KMM			0x00



/************************************************************************************/
/* 	IP��ַ����ṹ .*/
/************************************************************************************/
typedef struct
{
	U8	part[6];
} HWL_IPAddressV6_t;

#define HWL_CONST_MAC_LEN	8

typedef struct
{
	S8 	mac[HWL_CONST_MAC_LEN]; //Mac ��ַ.
} HWL_MacAddress_t;


S32 __HWL_DataPerformLock(U32 tagid, void *fun, void *data);
S32 ____HWL_DataPerchaseLock(S32 tagid, void *data, U32 size, void *__fun, U32 TickMark);


/** just copy */
#define __HWL_DataPerchaseLock(tagid,address,size)  ____HWL_DataPerchaseLock(tagid,address,size,NULL, 0)
#define HWL_DataPerchaseLock(tagId, data)   ____HWL_DataPerchaseLock(tagId, &data, sizeof(data), NULL, 0)
#define HWL_DataPerchaseLockSync(tagId, data, mark)   ____HWL_DataPerchaseLock(tagId,&data,sizeof(data), NULL, mark)



/************************************************************************************/
/*	1.�������ݽӿ�*/
/************************************************************************************/
typedef struct tagHWL_NicInfo
{
	U8	flag;
	U32	rate;
} HWL_NicInfo_t;

typedef struct tagHWL_HeatBeat
{
	U16 temperature;
	U32 InTSRate;
	U32 OutTsRate;
	U32 resetWord;
	S32	psiNumber;
	U8	readError;
	U8	writeError;
	U8 	buffFlag;
	U32	chipPll;
	U8 shortStatus;

} HWL_HeatBeat_t;

void HWL_HeatBeatShow(HWL_HeatBeat_t *headbeat);


/**	�������ʲ�ѯ.*/
S32 HWL_InputChnRateRequest();

/**	������ʲ�ѯ.*/
S32 HWL_OutputChnRateReqeust();
S32 HWL_OutputChnRateReqeustForS();


/************************************************************************************/
/* �鲥�ļ������˳�	*/


#define HWL_CONST_GROUP_BROADCAST_ADD			1		//����
#define HWL_CONST_GROUP_BROADCAST_DELETE		0		//�˳�

typedef struct tagHWL_IPAddressV4
{
	U8	part[4];
} HWL_IPAddressV4_t;



typedef struct tagHWL_GroupBroadcast
{
	S32 operator;						//[HWL_CONST_GRPOUP-*]
	U8	physicChannelId;				//��������ͨ���˿ں�.
	U8	groupNo;						//����������ı��
	HWL_IPAddressV4_t *ipaddressTable;		//IP��ַ��
	S32 ipaddressSize;

} HWL_GroupBroadcastTable_t;


/**	�鲥�ļ������˳� */
S32 HWL_GroupBroadTableCastSet( HWL_GroupBroadcastTable_t *groupCast);

/************************************************************************************/
/*	2.��DPB�����TS����188����(CPU-��FPGA)  TAG = 0x0a */
/************************************************************************************/

typedef struct tagHWL_DPBTsInsert
{
	U8  physicChannelId;
	U16 logicChannelId;
	U8	*tsDataBuff;
	S32	tsDataBuffSize;

} HWL_DPBTsInsert_t;



typedef struct
{
	U32 asiRate;
	U32 qamRate;

} HWL_AllRateInfo_t ;

U16 HWL_AllRateInfo( HWL_AllRateInfo_t  *rateInfo);


/************************************************************************************/
/*	4.���TS �������� (CPU-��FPGA)  TAG = 0x04.*/
/************************************************************************************/



typedef struct tagHWL_OutPutTsParam
{
	U8	physicChannelId;		//01:phy IP port = 01
	S16 outputChannelId;		//TS_Index : ���TS���߼���š�0..15
	S16 portNo;
	S8	ipVersion;
	HWL_IPAddressV4_t addressv4;
	HWL_IPAddressV4_t addressv6;

} HWL_OutPutTsParam_t;





/************************************************************************************/
/*	5.����ͨ���������ýӿ�*/
/************************************************************************************/
typedef struct
{
	U8	physicChannelId;
	S8	ipVersion;

	HWL_MacAddress_t mac;
	U32 addressv4;
	U32 addressv6;

} HWL_PhyChannelParam_t;





/************************************************************************************/
/*	6. �������PIDӳ��ͽ�Ŀ����*/
/************************************************************************************/



//	PIDӳ����Ϣ.
typedef struct
{
	U16	inputLogicChannelId;		//����TSͨ����
	U16 inputPid;					//����PID
	U16 outputLogicChannelId;	//���TSͨ����
	U16 outputPid;				//����PID
	U8	groupIndex;				//ʹ�õĿ�������.
	U8	disflag;					/**	�ڲ�ʹ�ô��ֶΣ�������� */
	U8	isCA;					//�Ƿ����.
	U8	inputPhyChannelId;			//		�����ֶ�.�ɺ���
	U8 	outputPhyChannelId;		//����ͨ����ʶ.

	U16	serialNo;
	U8	des_flag;
	U8  index;					//�������.

} HWL_PidMapItem_t;


/************************************************************************************/
/* 7.����TS��UDP�˿ڵ� �������� */
/************************************************************************************/


typedef struct tagHWL_InputUdpPort
{
	U8	physicChannelId;

	U16 inputLogicChannelId;				//����TSͨ���� [0..249]
	U16 portNumber;						//Ҫ����TS��Ŀ�Ķ˿ںš�

	U8	serialNumber;					//�� IP : PortNumber����ɷ�ʽ���������еķ�ʽ���õ��������.(0..N-1)

	U8   ipVersion;

	union
	{
		HWL_IPAddressV4_t v4;	//����IP�˿ڵ�Դ�˿ڵ�ַ(32bits).
		HWL_IPAddressV6_t v6;	//����IP�˿ڵ�Դ�˿ڵ�ַ(32bits).
	} originalAddress;


	union
	{
		HWL_IPAddressV4_t v4;	//����IP�˿ڵ�Դ�˿ڵ�ַ(32bits).
		HWL_IPAddressV6_t v6;	//����IP�˿ڵ�Դ�˿ڵ�ַ(32bits).
	} destinedAddress;

} HWL_InputUdpPort_t;



S32 HWL_InputUdpPortSend(HWL_InputUdpPort_t *inputUpdPort );



/************************************************************************************/
/* 10. Ӳ��ͨ��״̬��ѯ */
/************************************************************************************/


typedef struct tagHWL_PhyStatusQsk
{
	U8	requestType;				//��ѯ���͡�
	U8	physicChannelId;			//Ӳ��ͨ����ʶ.

} HWL_PhyStatusQsk_t;




/**
*	@PhysicTypeӲ������
-- 0 : ASI        attu : 0 : input   1:output
-- 1 : 1000M IP
-- 2 : 100M IP
-- 3 : E3/DS3
-- 4 :  AD9789���ư�   In/OUT : 1 out  MAX_CH = 16
-- 5 :  ����Logic���ư� In/OUT : 1 out  MAX_CH = 16/32
-- 6 :  Board ID : In/OUT : = 0 : MAX_CH  2Bytes : ID31..24  ID23..16
-- 7 :  Board ID : In/OUT : = 0 : MAX_CH  2Bytes : ID15..8   ID7..0
-- 0x80 -- haveware version
-- 0x81 -- Toncy Log
@InOrOut:�������� [ HWL_CONTS_INPUT| HWL_CONST_OUTPUT]
@MaxChNum:����ͨ·�����֧�ֵ��߼�ͨ·.
*/

typedef struct tagHWL_PhyStatusRsk
{
	U8	physicChannelId;			//{HWL_CONST_PHY_* }
	U8	inOrOut;					//[HWL_CONST_INPUT | HWL_CONST_OUTPUT ]
	U16 maxLogicChannelNum;	//����ͨ·�����֧�ֵ��߼�ͨ·��

} HWL_PhyStatusRsk_t;



/**	FPGA�汾..*/
typedef struct tagHWL_FpgaVersion
{
	U8 v;
	U32 year;
	U8 month;
	U8 day;

} HWL_FpgaVersion_t;


/**	Ӳ��ͨ����Ϣ�ظ�.*/
typedef struct
{
	HWL_PhyStatusRsk_t table[HWL_CONST_PHYINFO_NUM_MAX];
	U32 			tableSize;

} HWL_PhyStatusRskTable_t;


typedef struct
{
	HWL_FpgaVersion_t fpga;
	HWL_PhyStatusRskTable_t PhyStatusRskTable;
	U32 chipSn;

} HWL_PhyStatusRskResponse_t;


void HWL_PhyStatusRskResponseShow(HWL_PhyStatusRskResponse_t *hwlPhyStatusTable);

/*���ò���TS*/
typedef struct
{
	U16 save;			//(0..N-1)Ϊ�洢��λ��,  
	U8	allNumber;
	U8	sendNumber;
	U16	timeInterval;	
	U16	outLogChannel;

	U8 	packetType;		// if bit3 = 1 ,  except  ECM used as ODD or Even   , if  bit3 = 0 : no effect    
	U8	evenOrOdd;		
	U8	casChannelNo;	// CAS channel Number.   00: channel 1  ��  11:channel 4 

	U8	*tsBuff;
	U32	tsBuffLen;

}HWL_DPBTsInserter;



/************************************************************************************/
/*	���ʲ�ѯ.*/
/************************************************************************************/

#define HWL_CONST_CHANNEL_INFO_REQ_INPUT_RATE		0x01		//��������
#define HWL_CONST_CHANNEL_INFO_REQ_OUTPUT_RATE		0x02		//�������


/**	��ѯ�ṹ�塣*/
typedef struct tagHWL_ChannelRateInfo
{
	U8	requestType;		//[HWL_CONST_TSINFO_REQ_INPUT_RATE |
	// HWL_CONST_TSINFO_REQ_OUTPUT_RATE]
	U8	physicChannelId;
	U16 totalRate;		//������

} HWL_ChannelRateInfo_t;



/**	���Ͳ�ѯͨ����������.*/
S32 HWL_ChannelRateInfoSend(HWL_ChannelRateInfo_t *);


typedef struct tagHWL_ChannelRate
{
	U16	logicChannelId;
	U32 logicChannelRate;

} HWL_ChannelRate_t;


typedef struct tagHWL_ChannelRateArray
{
	U8	requestType;	//[ HWL_CONST_TSINFO_REQ_INPUT_RATE |
	//  HWL_CONST_TSINFO_REQ_OUTPUT_RATE ]
	U8	physicChannelId;
	U32 totalRate;		//������

	HWL_ChannelRate_t channelRateArray[HWL_CONST_CHANNEL_NUM_MAX];
	U32	channelRateArraySize;

} HWL_ChannelRateArray_t;




#if 0

/************************************************************************************/
/* PIDͳ�ƵĶ˿����� */
/************************************************************************************/

/**	��ѯ�ṹ�塣*/
typedef struct tagHWL_PidStatics
{
	U8	physicChannelId;
	U16	logicChannelId;

} HWL_PidStatics_t;



/**	PID ͳ�ƵĶ˿�����.*/
S32 HWL_PidStaticsSend(HWL_PidStatics_t *pidSearch);

/**	 PID ͳ�ƵĶ˿����ʲ�ѯ����.*/
S32 HWL_PidStaticsSearch(HWL_PidStatics_t *pidSearch);


typedef struct tagHWL_PidRate
{
	U16	pidVal;
	U16 pidRate;

} HWL_PidRate_t;



/**	�ظ��ṹ��*/
typedef struct tagHWL_PidRateArray
{
	//HWL_PidStatics_t search;	//��ʶ��ѯ����.
	U8 physicChannelId;			//
	U16 totalRate;				//ͨ��������
	HWL_PidRate_t pidRateArray[HWL_CONST_PID_NUM_MAX];
	U32 channelRateArraySize;

} HWL_PidRateArray_t;

#endif


#if 0

/************************************************************************************/
/* ����IP�˿�����ͳ�Ʋ�ѯ|��� */
/************************************************************************************/


typedef struct tagHWL_InputIPPortStatistic
{
	U8	physicChannelId;
} HWL_InputIPPortStatistic_t;



typedef struct tagHWL_InputIPPortStatisticInfo
{
	HWL_IPAddressV4_t addressv4;
	U16 portNumber;
	U16 portRate;
} HWL_InputIPPortStatisticInfo_t;



typedef struct tagHWL_InputIPPortStatisticInfoArray
{
	U8	physicChannelID;
	U16	totalRate;
	HWL_InputIPPortStatisticInfo_t *ipPortInfoArray;
	U32		ipPortInfoArraySize;

} HWL_InputIPPortStatisticInfoArray_t;




/**	����IP�˿�����ͳ����Ϣ���..*/
S32 HWL_InputIPPortStatisticClear( HWL_InputIPPortStatistic_t *statistic );

/**	����IP�˿�����ͳ����Ϣ��ѯ..*/
S32 HWL_InputIPPortStatisticSend(HWL_InputIPPortStatistic_t *statistic);


S32 HWL_InputIPPortStatisticArrayGet(HWL_InputIPPortStatisticInfoArray_t *array);


#endif

typedef struct tagHWL_ConditionAccess
{
	U8 enabled;
} HWL_ConditionAccess_t;


/************************************************************************************/
/* ����IP�˿�����ͳ�Ʋ�ѯ|��� */
/************************************************************************************/

typedef struct
{
	U8	physicChannelId;
}HWL_EthDetectionParam;


typedef struct
{
	HWL_IPStatisticsArray		m_IPArray;
	HWL_PIDStatisticsArray		m_PIDArray;
}HWL_PIDETHStatistcs;


/************************************************************************************/
/*	15.PSI����Buffer��С  	*/
/************************************************************************************/

typedef struct tagHWL_PSIBuffSize
{
	U8 stableTsPackgeNum;			//��ǰ�ܽ�������PSI��TS������
	U8 immediateTsPackgeNum;		//��ʱPSI�ܽ��ܵ�TS��������
} HWL_PSIBuffSize_t;


void HWL_PSIBuffSizeShow(HWL_PSIBuffSize_t *ratearray);



/**PID��ѯ�ṹ�塣*/
typedef struct tagHWL_PidStatics
{
	U8	physicChannelId;
	U16	logicChannelId;

} HWL_PidStatics_t;




/************************************************************************************/
/*	copy PID ������: (PHY_Ch = 0)	*/
/************************************************************************************/


/**	 ��ҪCOPY��PIDԴ�����á�
*
*	@(id):�û���Ҫ��ÿ��ӳ����Ϣ����һ��ID����ID����Ψһ��
*		�������Ӧ��Ŀ�ı����м���һ��������Ӧ��Ŀʱ��Ŀ�ı��еı����ID����ʹ�ID��ͬ��
*		�Ա�ʾ��Ϊӳ���ϵ.
*/
typedef struct tagHWL_PIDSource
{
	U16	id;
	U8	physicInputId;
	U16 originalChannelID;			//Դͨ��
	U16	originalPid;					//ԴPID


	/*	���³�Ա����Ҫ�û�����...*/
	U16 destinedPidOutListOffset;		//Ŀ��PID�����LIST�е�ƫ����
	U16 destinedPidOutListNum;		//Ŀ��PID�����LIST�еĸ���

} HWL_PIDSource_t;



typedef struct tagHWL_PIDCopy
{
	U8	physicChannel;
	U8	flag;
	HWL_PIDSource_t  *pidsourceTable;
	S32		pidsourceSize;

} HWL_PIDCopyOriginalTable_t;



void HWL_PIDCopyOriginalTableShow(HWL_PIDCopyOriginalTable_t *original);

/**	17.2 ��ҪCOPY��PIDĿ��PID�����á�
*	@(id):��ֵ����Ϊ���Ӧ��Դ���е�HWL_PIDSource_t.id��ͬ
*/

typedef struct tagHWL_PIDdestined
{
	U16	id;
	U16 destinedChannelID;			//Դͨ��
	U16	destinedPid;					//ԴPID

} HWL_PIDdestined_t;



typedef struct tagHWL_PIDCopyDestinedTable
{
	U8 flag;
	U8	physicChannel;
	HWL_PIDdestined_t  *pidDestinedTable;
	S32		pidDestinedSize;

} HWL_PIDCopyDestinedTable_t;



void HWL_PIDCopyDestinedTableShow(HWL_PIDCopyDestinedTable_t *destined);



/************************************************************************************/
/*	18.1 CPU�Ե������йص�оƬ�ļĴ������úͶ�ȡ: (PHY_Ch = 0)	*/
/************************************************************************************/


typedef struct tagHWL_ModularChipRegister
{
	U8	chipID;		//[ CONST_CHIPID_* ]
	U8	rOrw;		//[HWL_CONST_READ|HWL_CONST_WRITE]
	U8	address;		//��ȡ�����ÿ��Ƶ�оƬ�ڼĴ�����ַ.
	U8	value;		//���ÿ��Ƶ�оƬ�ڼĴ�����ֵ

} HWL_ModularChipRegister_t;

typedef struct tagHWL_E3DS3ChipRegister
{
U32						ChipID;
U8						ReadControl;
U8						E3DS3Select;					/*0 ..3 �ֱ����E3/DS3�Ӱ������ͨ��1..4  */
}HWL_E3DS3ChipRegister_t;


typedef struct tagHWL_ModularChipRegisterTable
{
	U8	physicChannel;
	HWL_ModularChipRegister_t	*moduleRegisterTable;
	S32		moduleRegisterSize;

} HWL_ModularChipRegisterTable_t;




/************************************************************************************/
/*	19.CPU��TS����˿ں�PCR У���MODE ����: (PHY_Ch = 0) */
/************************************************************************************/




typedef struct tagHWL_ChannelInputPcrMode
{
	U8	physicChannel;
	U8	verify;
	U8	verifyMode;			//[HWL_CONST_PCR_VERIFY_40MS_INSERT_YES|
	//HWL_CONST_PCR_VERIFY_40MS_INSERT_NO ]
} HWL_ChannelInputPcrMode_t;

S32 HWL_ChannelInputPcrModeSend( HWL_ChannelInputPcrMode_t *veryfyMode);



/************************************************************************************/
/*	20.����������ʱFPGA����CPU  */
/************************************************************************************/

typedef struct tagHWL_InputStreamNotifier
{
	U16	udpPort;
	U8   channelId;
	U8	program;

} HWL_InputStreamNotifier_t;


typedef struct tagHWL_InputStreamNotifierTable
{
	U8	physicChannel;
	HWL_InputStreamNotifier_t *streamTable;
	U32				streamSize;
} HWL_InputStreamNotifierTable_t;



S32 HWL_InputStreamNotifierTableGet( HWL_InputStreamNotifierTable_t *streamNotifierTable);



/************************************************************************************/
/*	21.����˿ںŽ�Ŀ�������б仯ʱ����CPU   */
/************************************************************************************/


typedef struct tagHWL_InputPortEsNotifier
{
	U8 idx;
	U8 esType;			// PES��������,������һ��,
	U16 esPid;			//PES���� PID ;����޴�PES,  ES_Type = 0xff    ES_PID = 0xffff
} HWL_InputPortEsNotifier_t;



typedef struct tagHWL_InputPortEsNotifierTable
{
	U8 physicChannel;
	HWL_InputStreamNotifier_t streamIdentifier;
	HWL_InputPortEsNotifier_t *esTable;
	U32				esTableSize;

} HWL_InputPortEsNotifierTable_t;


S32 HWL_InputPortEsNotifierTableGet( HWL_InputPortEsNotifierTable_t *esnotifier);




/************************************************************************************/
/*	22.1 ӳ��Դ(�����߼�ͨ����UDP�˿�)������ */
/************************************************************************************/

typedef struct tagHWL_PidMapMapOriginalPort
{
	HWL_IPAddressV4_t  inputIpAddress;
	U8			inputlogicChannelID;			//�����IP�߼�ͨ����
	U8			udpPort;						// UDP�˿�
	U16			pidMapTableBeginIdx;				// PIDӳ������ʼλ��
} HWL_PidMapMapOriginalPort_t;



typedef struct tagHWL_PidMapMapOriginalPortTable
{
	HWL_PidMapMapOriginalPort_t *pidMapPortTable;
	U32			idMapPortTableSize;
} HWL_PidMapMapOriginalPortTable_t;



S32 HWLPidMapMapOriginalPortTableSend( HWL_PidMapMapOriginalPortTable_t *pidMapPortTable);





/************************************************************************************/
/*	22.2 ԴPID��Ŀ��ͨ����Ŀ��PIDӳ�������   */
/************************************************************************************/

typedef struct tagHWL_PidMapOriginalToDestined
{
	U16	originalPid;			//ԴPID.
	U8	destinedChannelID;	//Ŀ��ͨ����.
	U16	destinedPid;			//Ŀ��PID.
} HWL_PidMapOriginalToDestined_t;


typedef struct tagHWL_PidMapItemTable
{
	U8 flag;
	HWL_PidMapOriginalToDestined_t *pidItemTable;
	U32	pidItemSize;

} HWL_PidMapOriginalToDestinedTable_t;



S32 HWL_PidMapOriginalToDestinedTableSend( HWL_PidMapOriginalToDestinedTable_t *pidMapTable);


/************************************************************************************/
/* 	24.����������ʶ�Ӧ��96MHz��������(BL85KMģ�������Ҫ),   */
/************************************************************************************/





typedef struct tagHWL_OutputRate96Mhz
{

	U8	modularStand;	//HWL_CONST_MODULAR_STAND_*
	U8	modularMode;	//HWL_CONST_MODULAR_MODE_*
	F64  symbolRate;		// etc:6.875..

} HWL_OutputRate96Mhz_t;


S32 HWL_OutputRate96MhzSend( HWL_OutputRate96Mhz_t *  );



/************************************************************************************/
/* 25.VODģʽ������TS����������: */
/************************************************************************************/




typedef struct tagHWL_VodInputTsParam
{
	U8 physicChannel;			// ����TS������˿ڱ��(0..N-1)
	U16 channelID	;			//�����б��е����(0..N-1)   (���NΪ1023)(����˳��Ϊ��0��N-1)
	HWL_IPAddressV4_t  inputIpAddress;	//����TS��IP��ַ
	U16	inputPort;				//����TS��UDP�˿ں�.

	U8	vodOrMutor;				// Vodģʽ����ģʽ.[HWL_CONST_VOD|HWL_CONST_MUTOR ]


	union
	{
		U16 	inputPortLogicNo;	//����˿ڵ��߼���� :
		struct
		{
			U8 outputQamChannelID;	//���QAMͨ�����(0..N-1)
			U8 outputQamProgramID;	//���QAMͨ���еĽ�Ŀ���(0..63)  ��� Num >= 64  ΪMPTS ,
		};
	} info;

} HWL_VodInputTsParam_t;

S32 HWL_VodInputTsParamSend( HWL_VodInputTsParam_t *vodInputTsParam  );


/************************************************************************************/
/* 26.1	�����оƬ��ͨѶ */
/************************************************************************************/


typedef struct tagHWL_EncryptChip
{

	U8	mainNo;
	U8	subNo;
	U8	length;
	U8 	buff[10];

} HWL_EncryptChip_t;



/************************************************************************************/
/* 25.1	VODģʽ�� PAT�汾��CRC32������ */
/************************************************************************************/

typedef struct tagHWL_VodPatAndCrcSet
{
	U8 	outputChannelId;
	U8 	outputProgram;
	U8 	patVersion;
	U32 patCrc32;
	U16 pmtPid;
	U8 	esNumber;
	U8 	pmtVersion;
	U32 pmtCrc32;

} HWL_VodPatAndCrcSet_t;


S32 HWL_VodPatAndCrcSetSend( HWL_VodPatAndCrcSet_t *vodPatCrc  );



/************************************************************************************/
/* 30. �������ģ��ĸ�λ��־   */
/************************************************************************************/

typedef struct tagHWL_ModuleReset
{
	U8	moduleId;
} HWL_ModuleReset_t;

S32 HWL_ModuleResetSend( HWL_ModuleReset_t *moduleReset  );


/**	��ѯ������Ϣ,�����������ָ�FPGA..*/
S32 HWL_HeatBeatSend();


/**	��ѯӲ��״̬��Ϣ����� .*/
S32 HWL_PhyStatusQskSend();

#define HWL_CONST_PHY_ASI				0x0
#define HWL_CONST_PHY_1000MIP			0x01
#define HWL_CONST_PHY_100MIP			0x02
#define HWL_CONST_PHY_ESDS3				0x03
#define HWL_CONST_PHY_AD9789			0x04
#define HWL_CONST_PHY_USLOGIC			0x05
#define HWL_CONST_PHY_BOARD_ID0X6		0x06
#define HWL_CONST_PHY_BOARD_ID0X7		0X7
#define HWL_CONST_PHY_HARDVER			0X80
#define HWL_CONST_PHY_TONCYLOG			0X81

#define HWL_CONST_INPUT		0
#define HWL_CONST_OUTPUT	1

#define HWL_CONST_FPGA_DEV_PATH		"/dev/fpgaconfig"


#if 0

/**	PID ͳ�ƵĶ˿�����.
*	@(inOrOut):��������TS.{HWL_CONST_INPUT| HWL_CONST_OUTPUT }
*/
S32 HWL_PidStatisticSend(U8 inOrOut, U8 logicChannelId);


/**	PID �˿�ͳ�Ʋ�ѯ.*/
S32 HWL_PidStatisticSearch(U8 inOrOut);


/**	xxxSize��xxxItem��������ѭ������ .*/

/**	����PID �˿�ͳ�������е���Ŀ����
*	@(return):size.//
.*/
U32 HWL_PidStatisticArraySize();


/**	����PIDͳ����Ϣ���ݼ���.
*	@(index): [0-size). size��HWL_PidStatisticArraySize()ȷ��,
*	@(pidRate):���뻺��.����������ʵ�����������ڴ�.
*/
S32 HWL_PidStatisticArrayItem(U32 index, HWL_PidRate_t *pidRate);

#endif



/**	��ȡPSI����Buffer��С*/
S32 HWL_PSIBuffSizeGet(HWL_PSIBuffSize_t *psiBuff);




/************************************************************************************/
/*	оƬ�Ĵ�������.	*/
/************************************************************************************/

/*	CPU�Ե������йص�оƬ�ļĴ������úͶ�ȡ: (PHY_Ch = 0)	*/
S32 HWL_ModularChipTableSend(HWL_ModularChipRegister_t *table, U32 size);


S32 HWL_FPGAWrite(U8 *Buff, U32 Size);
/************************************************************************************/
/**	PIDӳ�� ����.*/
/************************************************************************************/



/**	�������������ṩ ��PIDӳ���Ĳ��� ��
*	HWL_PidMapArrayClear...���ڴ����ݴ��ӳ�����ա�
*	HWL_PidMapArrayAppend...���ڴ��м����µ�PIDӳ�����.		..MAX-4096
*	HWL_PidMapArrayApply..�ú����Ὣ�ڴ��д洢��PIDӳ��������򣬲����͸�FPGA.
*/
void HWL_PidMapArrayClear();
S32  HWL_PidMapArrayAppend(HWL_PidMapItem_t *pidMap);
void HWL_PidMapArrayApply();




/************************************************************************************/
/**	PID���� ����.*/
/************************************************************************************/


/**	��������PIDӳ�����.
*	�ڲ����޸�@(pidMapArray).����������͵�FPGA.
*/


/** 	PID����.
*	(@pidMap.inputLogicChannelId)��ʼ��
*/
S32	HWL_PidSearchStart(U16 channelId, U16 pid, U8 filter, U8 FlagOrCount);

/**	PID����ֹͣ.*/
S32	HWL_PidSearchStop(U16 channelId, U16 pid, U8 filter);




/**	�ص���������.�û�����ֱ�Ӵ�����������.*/
typedef  void (*HWL_CallBack_t)(U8 *buff, U32 bufflen);


/**	��ģ���ʼ��*/
void HWL_InterInit();


void __HWL_Pool(void *noused);

U32 HWL_ChannelRateArraySize();


/**��ѯ��ͨ������*/
U32 HWL_ChannelRateArrayItem(U32 index, HWL_ChannelRate_t *channelRate);





/**	�����ֶ���ṹ��.*/
typedef struct tagICPL_Cmd_t
{
	U8 tagid;
	U8 *buff;
	U32 buff_len;
	//mutex;

	void *data;
	U32 data_len;
	void (*perform)(struct tagICPL_Cmd_t *, void *data);
	HANDLE32 mutex;
	U8	length;
	void (*callback)(struct tagICPL_Cmd_t *, void *data);		//�ص�����..
	U32 time;		//���յ��Ĵ���.
	U32 timeLimit;	//�����յĴ���.������մ��� time>=timeLimit,���ڴ�TAGID��send��������ִ��.


	U32	m_RecvTick;
} ICPL_Cmd_t;



void ICPL_Cmd_Lock(ICPL_Cmd_t *cmd);
void ICPL_Cmd_UnLock(ICPL_Cmd_t *cmd);


typedef  void (*ICPL_CmdFun_t )(struct tagICPL_Cmd_t *, void *data);
typedef  void (*ICPL_CmdPickFun_t )(const struct tagICPL_Cmd_t *, void *data,  U32 );



typedef ICPL_Cmd_t ICPL_CmdRequest_t;
typedef ICPL_Cmd_t ICPL_CmdResponse_t;




/**	����������Ԥ���仺��������.�󲿷�����û�����Ҫ�Լ����������ݣ��ȱ��������Ż���*/
#define ICPL_CMD_HEAT_REQUEST_BUFF_LEN 			4
#define ICPL_CMD_HEAT_RESPONSE_BUFF_LEN 		0x0c
#define ICPL_CMD_DPB_TSINSERT_REQUEST_BUFF_LEN	 	22
#define ICPL_CMD_OUTTSPARAM_BUFF_LEN 			10
#define ICPL_CMD_PHYCHANNELPARAM_LEN			0x06
#define ICPL_CMD_PHY_REQUEST_BUFF_LEN			0x00

#define ICPL_CMD_PHYPARAM_REQUEST_BUFF_LEN 	22
#define ICPL_CMD_PIDMAP_REQUEST_BUFF_LEN		14
#define ICPL_CMD_PIDMAP_RESPONSE_SEARCH_BUFF_LEN		55

#define ICPL_CMD_PHY_RESPONSE_BUFF_LEN			100
#define ICPL_CMD_TS_RATE_REQUEST_BUFF_LEN		100


/**
���ö�����Ϊ:
CPU---->FPGA.

1. 	�ӱ��в����������ʵ��.
2.	ʹ�����û��Զ��庯���Ͳ���������ʵ���е�������.
3.	����DRL����㴫�͵�FPGA.

FPGA--->CPU.
1.	��FPGA�ж�ȡ�����ݡ�
2.	ͨ�����������е�TAGID��������ƶ���ʵ��.
3.	ʹ�ÿ��ƶ����еĽ���������������������Ϊ����ṹ��.
3.	ʹ�ó���ṹ�����XX.

*/

/* ��̬���� */

/*
typedef enum  tagICPL_ActionId
{
ICPL_ACTION_ID_HEATBEAT,
ICPL_ACTION_ID_OUTTSPARAM,
ICPL_ACTION_ID_PHYPARAM,
ICPL_ACTION_ID_PIDMAP,
}ICPL_ActionId_t;
*/


/**	ʹ�ñ�ģ��ǰ����Ҫ�ȵ��ô˺��������ӳ����ʼ���ȹ���.	*/
void ICPL_CmdArrayInit();



/**	���Һ�����ͨ��tagID������Ӧ�Ĵ����塣������ģ���ڲ�ʹ�á�
*/
ICPL_Cmd_t *ICPL_CmdResponseFind(U8 tagID);



/**	�ṩ�Զ��崦��ظ����ݵĲ���.
*	�����ô˺���ע��ÿ�������ֻظ��Ĵ�������
*	@(tagID):������ID;
*	@(userfun):ICPL_CmdFun_t.�����Ǵ����͡�����: ICPL_OPtorPidMapSearchDemo .
*	@(userdata):�û��Զ������ݡ�
*
*
*	������������tagID���ҵ�������֮�󣬻���á�
*	cmd->perform(cmd->userdata);
*	ICPL_CmdResponseOPtorSet�����ļ���:
*			cmd->perform=userfun
*			cmd->data=userdata;
*/
ICPL_CmdFun_t ICPL_CmdResponseCallbackSet(U8  tagID, ICPL_CmdFun_t userfun);



#define ICPL_CheckOk		1
#define ICPL_CheckNo 		0
#define ICPL_CheckRange(a,b,c) 	((c)<=b)




void ICPL_Cmd_Lock(ICPL_Cmd_t *cmd);
void ICPL_Cmd_UnLock(ICPL_Cmd_t *cmd);




#define ICPL_CMD_IDX_TAGID		0
#define ICPL_CMD_IDX_LENGTH	 	1



/**	����������Ԥ���仺��������.�󲿷�����û�����Ҫ�Լ����������ݣ��ȱ��������Ż���*/
#define ICPL_CMD_HEAT_REQUEST_BUFF_LEN 			4
#define ICPL_CMD_HEAT_RESPONSE_BUFF_LEN 		0x0c
#define ICPL_CMD_DPB_TSINSERT_REQUEST_BUFF_LEN	 	22
#define ICPL_CMD_OUTTSPARAM_BUFF_LEN 			10
#define ICPL_CMD_PHYCHANNELPARAM_LEN			0x06
#define ICPL_CMD_PHY_REQUEST_BUFF_LEN			0x00

#define ICPL_CMD_PHYPARAM_REQUEST_BUFF_LEN 	22
#define ICPL_CMD_PIDMAP_REQUEST_BUFF_LEN		14
#define ICPL_CMD_PIDMAP_RESPONSE_SEARCH_BUFF_LEN		55

#define ICPL_CMD_PHY_RESPONSE_BUFF_LEN			100
#define ICPL_CMD_TS_RATE_REQUEST_BUFF_LEN		100


/**
���ö�����Ϊ:
CPU---->FPGA.

1. 	�ӱ��в����������ʵ��.
2.	ʹ�����û��Զ��庯���Ͳ���������ʵ���е�������.
3.	����DRL����㴫�͵�FPGA.

FPGA--->CPU.
1.	��FPGA�ж�ȡ�����ݡ�
2.	ͨ�����������е�TAGID��������ƶ���ʵ��.
3.	ʹ�ÿ��ƶ����еĽ���������������������Ϊ����ṹ��.
3.	ʹ�ó���ṹ�����XX.

*/

/* ��̬���� */


/**
*	��С�˶���:
*	ICPL_BIG_ENDIAN:		���,��λ�ڵ͵�ַ.
*	ICPL_LITTLE_ENDIAN:	С�ˣ���λ�ڸߵ�ַ.
*/
#define ICPL_BIG_ENDIAN			0
#define ICPL_LITTLE_ENDIAN		1

/**	����ϵͳ��С�˱��뷽ʽ.[ICPL_BIG_ENDIAN|ICPL_LITTLE_ENDIAN ].*/
S32	ICPL_CodeMode();



/**	��ʼ���ڲ�ģ���ڴ�.*/
void ICPL_CodeInit();



#define ICPL_CODE_MODE_COPY	    0	//ԭ������.
#define ICPL_CODE_MODE_U8		1	//����ת��
#define ICPL_CODE_MODE_U16		2	//����ת��
#define ICPL_CODE_MODE_U24		3
#define ICPL_CODE_MODE_U32		4	//����ת��



typedef struct
{
	void	*address;
	U32		m_StartIndex;		//��ʼ�±�.
	U32		m_DataSize;		//����.
	U8		mode;			//
} ICPL_ByteInfo_t;



/**	����.*/
void ICPL_ByteInfoDecode( ICPL_ByteInfo_t  *byteInfo, U32 size, U8 *buff, U32 buffLen  );

/**	����.*/
void ICPL_ByteInfoEncode( ICPL_ByteInfo_t  *byteInfo, U32 size, U8 *buff, U32 buffLen  );


/**	�ظ��������������. */
S32 ICPL_OPtorHeatBeatParser(ICPL_CmdResponse_t  *cmd, HWL_HeatBeat_t *headbeat);
S32 ICPL_OPtorPhyRskResponseParser(ICPL_CmdRequest_t  *cmd, HWL_PhyStatusRskResponse_t *phyRsk);
S32 ICPL_OPtorChannelRateParser(ICPL_CmdRequest_t  *cmd, HWL_ChannelRateArray_t *);
S32 ICPL_OPtorStatisticsArrayParser(ICPL_CmdResponse_t  *cmd, HWL_PIDETHStatistcs *param);
//S32 ICPL_OPtorInputIPPortStatisticParser(ICPL_CmdRequest_t  *cmd, HWL_InputIPPortStatisticInfoArray_t *statistic);
S32 ICPL_OPtorPSIBuffSizeParser(ICPL_CmdRequest_t  *cmd, HWL_PSIBuffSize_t *psiBuff);
S32 ICPL_OPtorInputStreamNotifierTableParser(ICPL_CmdResponse_t  *cmd, HWL_InputStreamNotifierTable_t *table);
S32 ICPL_OPtorInputPortEsNotifierTableParser(ICPL_CmdResponse_t  *cmd, HWL_InputPortEsNotifierTable_t *table);
S32 ICPL_OPtorEncryptChipParser(ICPL_CmdRequest_t  *cmd, HWL_EncryptChip_t *encryptChip);


/** ����������������.	*/
S32 ICPL_OPtorHeatBeatMake(ICPL_CmdResponse_t  *cmd, HWL_HeatBeat_t *headbeat);
S32 ICPL_OPtorDPBTsInsertMake(ICPL_CmdRequest_t  *cmd, HWL_DPBTsInsert_t *param);
S32 ICPL_OPtorOutTsParamMake(ICPL_CmdResponse_t  *cmd, HWL_OutPutTsParam_t *param);
S32 ICPL_OPtorPhyChannelParamMake(ICPL_CmdResponse_t  *cmd, HWL_PhyChannelParam_t *param);
S32 ICPL_OPtorInputUdpPortMake(ICPL_CmdResponse_t  *cmd, HWL_InputUdpPort_t *param);
S32 ICPL_OPtorPhyQskMake(ICPL_CmdRequest_t  *cmd, HWL_PhyStatusQsk_t *phyQsk);
S32 ICPL_OPtorChannelRateInfoMake(ICPL_CmdRequest_t  *cmd, HWL_ChannelRateInfo_t *param);
S32 ICPL_OPtorPidStatisticMake(ICPL_CmdRequest_t  *cmd, HWL_PidStatics_t *pidSearch);
S32 ICPL_OPtorPidStatisticSearchMake(ICPL_CmdRequest_t  *cmd, HWL_PidStatics_t *pidSearch);
S32 ICPL_OPtorInputIPPortStatisticClearMake(ICPL_CmdRequest_t  *cmd, HWL_EthDetectionParam *statistic);
S32 ICPL_OPtorInputIPPortStatisticSendMake(ICPL_CmdRequest_t  *cmd, HWL_EthDetectionParam *statistic);
S32 ICPL_OPtorControlWordMake(ICPL_CmdRequest_t  *cmd,  HWL_ControlWord_t  *cw );
S32 ICPL_OPtorControlWordSwitchMake(ICPL_CmdRequest_t  *cmd,  HWL_ControlWord_t  *cw );
S32 ICPL_OPtorCondtionAccessMake(ICPL_CmdRequest_t  *cmd, HWL_ConditionAccess_t *access);
S32 ICPL_OPtorGroupBroadcastTableMake(ICPL_CmdRequest_t  *cmd,  HWL_GroupBroadcastTable_t  *groupcast );
S32 ICPL_OPtorPIDCopyOriginalTableMake(ICPL_CmdRequest_t  *cmd, HWL_PIDCopyOriginalTable_t *pidCopy);
S32 ICPL_OPtorPIDCopyDestinedTableMake(ICPL_CmdRequest_t  *cmd,  HWL_PIDCopyDestinedTable_t *pidTable);
S32 ICPL_OPtorModularChipRegisterTableMake(ICPL_CmdRequest_t  *cmd, HWL_ModularChipRegisterTable_t *registerTable);
S32 ICPL_OPtorChannelInputPcrModeMake(ICPL_CmdRequest_t  *cmd, HWL_ChannelInputPcrMode_t *veryfyMode);
S32 ICPL_OPtorPassModeMake(ICPL_CmdRequest_t  *cmd, HWL_PassMode_t *  );
S32 ICPL_OPtorVodPatAndCrcSetMake(ICPL_CmdRequest_t  *cmd, HWL_VodPatAndCrcSet_t *param);
S32 ICPL_OPtorDPBStableTsInsertMake(ICPL_CmdRequest_t  *cmd, HWL_DPBTsInserter *param);
S32 ICPL_OPtorDPBStableTsInsertClearMake(ICPL_CmdRequest_t  *cmd, HWL_DPBTsInserter *param);
S32 ICPL_OPtorModuleResetMake(ICPL_CmdRequest_t  *cmd, HWL_ModuleReset_t *moduleReset);
S32 ICPL_OPtorPidMapClearMake(ICPL_CmdRequest_t  *cmd, HWL_PidMapItem_t *param);
S32 ICPL_OPtorPidMapOriginalToDestinedTableMake(ICPL_CmdRequest_t  *cmd, HWL_PidMapOriginalToDestinedTable_t *pidMapTable);
S32 ICPL_OPtorOutputRate96MhzMake(ICPL_CmdRequest_t  *cmd, HWL_OutputRate96Mhz_t *rateSet);
S32 ICPL_OPtorPidMapSetMake(ICPL_CmdRequest_t  *cmd, HWL_PidMapItem_t *param);
S32 ICPL_ReplicatePidMapSRCClearSetMake(ICPL_CmdRequest_t  *cmd, HWL_PidMapItem_t *_param);
S32 ICPL_ReplicatePidMapDestClearSetMake(ICPL_CmdRequest_t  *cmd, HWL_PidMapItem_t *_param);
S32 ICPL_OPtorEncryptChipReadMake(ICPL_CmdRequest_t  *cmd, HWL_EncryptChip_t *encryptChip);
S32 ICPL_OPtorEncryptChipWriteMake(ICPL_CmdRequest_t  *cmd, HWL_EncryptChip_t *encryptChip);
S32 ICPL_OPtorVodInputTsParamMake(ICPL_CmdRequest_t  *cmd, HWL_VodInputTsParam_t *vodInputTsParam);
S32 ICPL_OPtorPidMapMapOriginalPortTableMake(ICPL_CmdRequest_t  *cmd, HWL_ModularChipRegisterTable_t *registerTable);


void ICPL_CmdArrayClear(void);

void HWL_RequestBuffInit(void);
void HWL_RequestBuffDestory(void);

void HWL_InterInit(void);
void HWL_InterDestory(void);

U8 HWLL_GetHardwareVersion(void);











#endif
