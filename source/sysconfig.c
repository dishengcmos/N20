

#include "sysconfig.h"
#include "sysctrl.h"
#include "flash.h"

void App_ClkCfg(void)
{
//    stc_sysctrl_clk_cfg_t stcCfg;
//    
//    ///< 因要使用的时钟源HCLK小于24M：此处设置FLASH 读等待周期为0 cycle(默认值也为0 cycle)
//    Flash_WaitCycle(FlashWaitCycle0);
//    ///< 时钟初始化前，优先设置要使用的时钟源：此处设置RCH为24MHz （默认值为4MHz）
//    Sysctrl_SetRCHTrim(SysctrlRchFreq24MHz);
//    
//    ///< 选择内部RCH作为HCLK时钟源; 后面的注释都是8m了应该
//    stcCfg.enClkSrc    = SysctrlClkRCH;//HCLK 24M
//    ///< HCLK SYSCLK/?
//    stcCfg.enHClkDiv   = SysctrlHclkDiv1;//
//    ///< PCLK 为HCLK/?
//    stcCfg.enPClkDiv   = SysctrlPclkDiv1;//为HCLK 24m
//    ///< 系统时钟初始化
//    Sysctrl_ClkInit(&stcCfg);  
//	Sysctrl_ClkSourceEnable(SysctrlClkRCH, TRUE);			//系统时钟源使能

	
	
    en_flash_waitcycle_t	enFlashWait;
    stc_sysctrl_pll_cfg_t	stcPLLCfg;
    
    //结构体初始化清零 
    DDL_ZERO_STRUCT(stcPLLCfg); 
    
    enFlashWait = FlashWaitCycle1;                      //读等待周期设置为1（当HCLK大于24MHz时必须至少为1），主要看HCLK是不是大于24M，如果是的话必须设置这个
    Flash_WaitCycle(enFlashWait);                       // Flash 等待1个周期
    
    stcPLLCfg.enInFreq    = SysctrlPllInFreq4_6MHz;     //RCH 4MHz
    stcPLLCfg.enOutFreq   = SysctrlPllOutFreq36_48MHz;  //PLL 输出48MHz
    stcPLLCfg.enPllClkSrc = SysctrlPllRch;              //输入时钟源选择RCH
    stcPLLCfg.enPllMul    = SysctrlPllMul12;            //4MHz x 12 = 48MHz
    Sysctrl_SetPLLFreq(&stcPLLCfg);
    Sysctrl_SetPLLStableTime(SysctrlPllStableCycle16384);//稳定时间选择
    Sysctrl_ClkSourceEnable(SysctrlClkPLL, TRUE); //时钟源选择
    
    Sysctrl_SysClkSwitch(SysctrlClkPLL);///< 时钟切换 
}

