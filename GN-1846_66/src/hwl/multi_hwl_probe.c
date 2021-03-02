/******************************************************************************

                  ��Ȩ���� (C), 2005-2018, GOSPELL���޹�˾

 ******************************************************************************
  �� �� ��   : multi_hwl_probe.c
  �� �� ��   : 1.0
  ��    ��   : ���
  ��������   : 2018��5��24��
  ����޸�   :
  ��������   : ̽��ӿڰ����͡�
  �����б�   :
  �޸���ʷ   :

   1.��    ��          : 2018��5��24��
      ��    ��          : ���
      �޸�����   : �����ļ�

******************************************************************************/
#include "multi_hwl_probe.h"
#include "multi_hwl.h"

#if 0
static ModuleTypeProbe InterfaceBoard[] = 
{
  { INTERFACE_BOARD_SDI, Init_SDI_Board, Terminate_SDI_Board, Probe_SDI_Board, FALSE},
  { INTERFACE_BOARD_HDMI, Init_Hdmi_Board, Terminate_Hdmi_Board, Probe_Hdmi_Board, FALSE}
  { INTERFACE_BOARD_4K, Init_HdmiLt6911c_Board, Terminate_HdmiLt6911c_Board, Probe_HdmiLt6911c_Board, FALSE}
};
#else
static ModuleTypeProbe InterfaceBoard[] = 
{
#if defined(GN1846)
  { INTERFACE_BOARD_SDI, Init_SDI_Board, Terminate_SDI_Board, Probe_SDI_Board, FALSE}
#endif  
  
#if defined(GN1866)
  { INTERFACE_BOARD_SDI, Init_SDI_Board, Terminate_SDI_Board, Probe_SDI_Board, FALSE}
#endif  
    
  
#if defined(GN1866B)
  { INTERFACE_BOARD_4K, Init_HdmiLt6911c_Board, Terminate_HdmiLt6911c_Board, Probe_HdmiLt6911c_Board, FALSE }
#endif    
};
#endif

/*****************************************************************************
* FUNCTION:HWL_Probe_InterfaceBoard
*
* DESCRIPTION: Probe the type of interface board.
*	
* INPUT:
*	  
* OUTPUT:
*	
*
* RETURN:
*
* NOTES:
*   
* HISTORY:
*	
*	Review: 
******************************************************************************/
static S32  s_InterfaceBoardType = INTERFACE_BOARD_UNKNOWN;

void HWL_Probe_InterfaceBoard(void)
{
    S32 i = 0;
    BOOL result = FALSE;
	
    for (i = 0; i < ARRAY_LENGTH(InterfaceBoard); i++)
    {
        if (InterfaceBoard[i].m_InitModuleCB != NULL)
    	 {
            InterfaceBoard[i].m_InitModuleCB();
    	 }

		 if (InterfaceBoard[i].m_ProbeModuleCB != NULL)
		 {
            InterfaceBoard[i].m_Is_Exit = InterfaceBoard[i].m_ProbeModuleCB();
		 }

		 if (InterfaceBoard[i].m_DestoryModuleCB != NULL)
		 {
            InterfaceBoard[i].m_DestoryModuleCB();
		 }

		 if (s_InterfaceBoardType == INTERFACE_BOARD_UNKNOWN)
		 {
			 if (InterfaceBoard[i].m_Is_Exit == TRUE)
			 {
				 s_InterfaceBoardType = InterfaceBoard[i].m_MuduleType;
			 }
		 }
    }

#if defined(GN1846) || defined(GN1866)
	if (s_InterfaceBoardType == INTERFACE_BOARD_UNKNOWN)
	{
		s_InterfaceBoardType = INTERFACE_BOARD_HDMI;
	}
#endif  


	GLOBAL_TRACE(("Board Type = %d\n", s_InterfaceBoardType));
}

/*****************************************************************************
* FUNCTION:HWL_Get_BoardType
*
* DESCRIPTION: Get the type of the current interface board. 
*	
* INPUT:
*	  
* OUTPUT:
*	
*
* RETURN:
*
* NOTES:
*   
* HISTORY:
*	
*	Review: 
******************************************************************************/
S32 HWL_Get_BoardType(void)
{
    return s_InterfaceBoardType;
}

/* EOF */

