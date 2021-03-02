#ifndef __MULTI_HWL_PROBE_H__
#define __MULTI_HWL_PROBE_H__

#include "global_def.h"
#include "global_micros.h"

#define ARRAY_LENGTH(_arr_)      (sizeof(_arr_) / sizeof(_arr_[0]))

typedef enum{
  INTERFACE_BOARD_HDMI = 0,
  INTERFACE_BOARD_SDI,
  INTERFACE_BOARD_4K,
  INTERFACE_BOARD_UNKNOWN
} INTERFACE_BOARD_TYPE;

typedef void  (* Init_Module)(void);
typedef void  (* Destroy_Module)(void);
typedef BOOL (*Probe_Module)(void);

typedef struct {
 	INTERFACE_BOARD_TYPE 	m_MuduleType;
 	Init_Module     					m_InitModuleCB;
 	Destroy_Module  				m_DestoryModuleCB;
 	Probe_Module    					m_ProbeModuleCB;
 	BOOL            						m_Is_Exit;
}ModuleTypeProbe;

void HWL_Probe_InterfaceBoard(void);
S32 HWL_Get_BoardType(void);
#endif

/*EOF*/



