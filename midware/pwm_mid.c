

#include "pwm_mid.h"

#include "gpio.h"
#include "flash.h"
#include "timer3.h"

#define LED_PORT       GpioPortB
#define LED_PIN        GpioPin6	

void ledInit(void)
{												
    stc_gpio_cfg_t           stcLEDPortCfg;		
												
    //结构体初始化清零							
    DDL_ZERO_STRUCT(stcLEDPortCfg);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE); //GPIO 外设时钟使能
    
    stcLEDPortCfg.enDir  = GpioDirOut;
    Gpio_Init(LED_PORT, LED_PIN, &stcLEDPortCfg);   //PD14设置为LED输出

}


//时钟初始化
void App_ClockCfg_pwm(void)
{
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

//Timer3 Port端口配置
void App_Timer3PortCfg(void)
{
    stc_gpio_cfg_t               stcTIM3Port; 
    
    //结构体初始化清零
    DDL_ZERO_STRUCT(stcTIM3Port); 
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE); //端口外设时钟使能
    
    stcTIM3Port.enDir  = GpioDirOut;
    
    Gpio_Init(LED_PORT, LED_PIN, &stcTIM3Port);
    Gpio_SetAfMode(LED_PORT,LED_PIN,GpioAf6);               //PB06设置为TIM3_CH0A
}

void App_Timer3Cfg(uint16_t u16Period, uint16_t u16CHxACompare)
{
    uint16_t                     u16CntValue;
    uint8_t                      u8ValidPeriod;
    stc_tim3_mode23_cfg_t        stcTim3BaseCfg;
    stc_tim3_m23_compare_cfg_t   stcTim3PortCmpCfg;
    
    //结构体初始化清零
    DDL_ZERO_STRUCT(stcTim3BaseCfg);
    DDL_ZERO_STRUCT(stcTim3PortCmpCfg);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralTim3, TRUE);   //Timer3外设时钟使能
        
    stcTim3BaseCfg.enWorkMode    = Tim3WorkMode2;             //锯齿波模式
    stcTim3BaseCfg.enCT          = Tim3Timer;                 //定时器功能，计数时钟为内部PCLK PCLK24MHZ
    stcTim3BaseCfg.enPRS         = Tim3PCLKDiv8;            //时钟源是PCLK=24m
    stcTim3BaseCfg.enCntDir      = Tim3CntUp;                 //向上计数，在三角波模式时只读
    stcTim3BaseCfg.enPWMTypeSel  = Tim3IndependentPWM;        //独立输出PWM
    stcTim3BaseCfg.enPWM2sSel    = Tim3SinglePointCmp;        //单点比较功能
    stcTim3BaseCfg.bOneShot      = FALSE;                     //循环计数
    stcTim3BaseCfg.bURSSel       = FALSE;                     //上下溢更新
												
    Tim3_Mode23_Init(&stcTim3BaseCfg);                        //TIM3 的模式23功能初始化
 
	Tim3_M23_ARRSet(u16Period, TRUE);                       //设置重载值,并使能缓存
    
    Tim3_M23_CCR_Set(Tim3CCR0A, u16CHxACompare);            //设置CH0比较值A 
    
    stcTim3PortCmpCfg.enCHxACmpCtrl   = Tim3PWMMode2;         //OCREFA输出控制OCMA:PWM模式2
    stcTim3PortCmpCfg.enCHxAPolarity  = Tim3PortPositive;     //正常输出
    stcTim3PortCmpCfg.bCHxACmpBufEn   = TRUE;                 //A通道缓存控制
    stcTim3PortCmpCfg.enCHxACmpIntSel = Tim3CmpIntNone;       //A通道比较中断控制:无
 
    Tim3_M23_PortOutput_Cfg(Tim3CH0, &stcTim3PortCmpCfg);     //比较输出端口配置	
    
    u8ValidPeriod = 0;                                        //事件更新周期设置，0表示锯齿波每个周期更新一次，每+1代表延迟1个周期
    Tim3_M23_SetValidPeriod(u8ValidPeriod);                   //间隔周期设置
																
    u16CntValue = 0;
    Tim3_M23_Cnt16Set(u16CntValue);                           //设置计数初值
	
    Tim3_ClearAllIntFlag();                                   //清中断标志
    //Tim3_Mode23_EnableIrq(Tim3UevIrq);                        //使能TIM3 UEV更新中断
//	Tim3_Mode0_DisableIrq();
//	
//	Tim3_Mode1_DisableIrq (Tim3CA0Irq);
//	Tim3_Mode1_DisableIrq (Tim3UevIrq);
//	Tim3_Mode1_DisableIrq (Tim3BkIrq);
//	Tim3_Mode1_DisableIrq (Tim3TrigIrq);
//	
// 	Tim3_Mode23_DisableIrq(Tim3CA0Irq);
// 	Tim3_Mode23_DisableIrq(Tim3UevIrq);
// 	Tim3_Mode23_DisableIrq(Tim3BkIrq);
// 	Tim3_Mode23_DisableIrq(Tim3TrigIrq);

 	Tim3_Mode23_DisableIrq(Tim3UevIrq);
    EnableNvic(TIM3_IRQn, IrqLevel3, FALSE);                   //TIM3中断使能
}

void pwmStart(void)
{
     //App_ClockCfg_pwm();                       //时钟初始化
	
      App_Timer3PortCfg();                       //Timer3 Port端口配置
    
      App_Timer3Cfg(500, 0);     //Timer3 配置:周期 0x9000; CH0/1/2通道A比较值0x6000; CH0/1/2通道B比较值0x3000
    
      Tim3_M23_EnPWM_Output(TRUE, FALSE);        //端口输出使能
    
      Tim3_M23_Run();                            //运行。
}
void compareSet(uint16_t u16CHxACompare)
{
    Tim3_M23_CCR_Set(Tim3CCR0A, u16CHxACompare);            //设置CH0比较值A 
}
