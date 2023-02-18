

#include "sysconfig.h"
#include "sysctrl.h"
#include "flash.h"

void App_ClkCfg(void)
{
//    stc_sysctrl_clk_cfg_t stcCfg;
//    
//    ///< ��Ҫʹ�õ�ʱ��ԴHCLKС��24M���˴�����FLASH ���ȴ�����Ϊ0 cycle(Ĭ��ֵҲΪ0 cycle)
//    Flash_WaitCycle(FlashWaitCycle0);
//    ///< ʱ�ӳ�ʼ��ǰ����������Ҫʹ�õ�ʱ��Դ���˴�����RCHΪ24MHz ��Ĭ��ֵΪ4MHz��
//    Sysctrl_SetRCHTrim(SysctrlRchFreq24MHz);
//    
//    ///< ѡ���ڲ�RCH��ΪHCLKʱ��Դ; �����ע�Ͷ���8m��Ӧ��
//    stcCfg.enClkSrc    = SysctrlClkRCH;//HCLK 24M
//    ///< HCLK SYSCLK/?
//    stcCfg.enHClkDiv   = SysctrlHclkDiv1;//
//    ///< PCLK ΪHCLK/?
//    stcCfg.enPClkDiv   = SysctrlPclkDiv1;//ΪHCLK 24m
//    ///< ϵͳʱ�ӳ�ʼ��
//    Sysctrl_ClkInit(&stcCfg);  
//	Sysctrl_ClkSourceEnable(SysctrlClkRCH, TRUE);			//ϵͳʱ��Դʹ��

	
	
    en_flash_waitcycle_t	enFlashWait;
    stc_sysctrl_pll_cfg_t	stcPLLCfg;
    
    //�ṹ���ʼ������ 
    DDL_ZERO_STRUCT(stcPLLCfg); 
    
    enFlashWait = FlashWaitCycle1;                      //���ȴ���������Ϊ1����HCLK����24MHzʱ��������Ϊ1������Ҫ��HCLK�ǲ��Ǵ���24M������ǵĻ������������
    Flash_WaitCycle(enFlashWait);                       // Flash �ȴ�1������
    
    stcPLLCfg.enInFreq    = SysctrlPllInFreq4_6MHz;     //RCH 4MHz
    stcPLLCfg.enOutFreq   = SysctrlPllOutFreq36_48MHz;  //PLL ���48MHz
    stcPLLCfg.enPllClkSrc = SysctrlPllRch;              //����ʱ��Դѡ��RCH
    stcPLLCfg.enPllMul    = SysctrlPllMul12;            //4MHz x 12 = 48MHz
    Sysctrl_SetPLLFreq(&stcPLLCfg);
    Sysctrl_SetPLLStableTime(SysctrlPllStableCycle16384);//�ȶ�ʱ��ѡ��
    Sysctrl_ClkSourceEnable(SysctrlClkPLL, TRUE); //ʱ��Դѡ��
    
    Sysctrl_SysClkSwitch(SysctrlClkPLL);///< ʱ���л� 
}

