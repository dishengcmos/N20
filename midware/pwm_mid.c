

#include "pwm_mid.h"

#include "gpio.h"
#include "flash.h"
#include "timer3.h"

#define LED_PORT       GpioPortB
#define LED_PIN        GpioPin6	

void ledInit(void)
{												
    stc_gpio_cfg_t           stcLEDPortCfg;		
												
    //�ṹ���ʼ������							
    DDL_ZERO_STRUCT(stcLEDPortCfg);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE); //GPIO ����ʱ��ʹ��
    
    stcLEDPortCfg.enDir  = GpioDirOut;
    Gpio_Init(LED_PORT, LED_PIN, &stcLEDPortCfg);   //PD14����ΪLED���

}


//ʱ�ӳ�ʼ��
void App_ClockCfg_pwm(void)
{
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

//Timer3 Port�˿�����
void App_Timer3PortCfg(void)
{
    stc_gpio_cfg_t               stcTIM3Port; 
    
    //�ṹ���ʼ������
    DDL_ZERO_STRUCT(stcTIM3Port); 
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE); //�˿�����ʱ��ʹ��
    
    stcTIM3Port.enDir  = GpioDirOut;
    
    Gpio_Init(LED_PORT, LED_PIN, &stcTIM3Port);
    Gpio_SetAfMode(LED_PORT,LED_PIN,GpioAf6);               //PB06����ΪTIM3_CH0A
}

void App_Timer3Cfg(uint16_t u16Period, uint16_t u16CHxACompare)
{
    uint16_t                     u16CntValue;
    uint8_t                      u8ValidPeriod;
    stc_tim3_mode23_cfg_t        stcTim3BaseCfg;
    stc_tim3_m23_compare_cfg_t   stcTim3PortCmpCfg;
    
    //�ṹ���ʼ������
    DDL_ZERO_STRUCT(stcTim3BaseCfg);
    DDL_ZERO_STRUCT(stcTim3PortCmpCfg);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralTim3, TRUE);   //Timer3����ʱ��ʹ��
        
    stcTim3BaseCfg.enWorkMode    = Tim3WorkMode2;             //��ݲ�ģʽ
    stcTim3BaseCfg.enCT          = Tim3Timer;                 //��ʱ�����ܣ�����ʱ��Ϊ�ڲ�PCLK PCLK24MHZ
    stcTim3BaseCfg.enPRS         = Tim3PCLKDiv8;            //ʱ��Դ��PCLK=24m
    stcTim3BaseCfg.enCntDir      = Tim3CntUp;                 //���ϼ����������ǲ�ģʽʱֻ��
    stcTim3BaseCfg.enPWMTypeSel  = Tim3IndependentPWM;        //�������PWM
    stcTim3BaseCfg.enPWM2sSel    = Tim3SinglePointCmp;        //����ȽϹ���
    stcTim3BaseCfg.bOneShot      = FALSE;                     //ѭ������
    stcTim3BaseCfg.bURSSel       = FALSE;                     //���������
												
    Tim3_Mode23_Init(&stcTim3BaseCfg);                        //TIM3 ��ģʽ23���ܳ�ʼ��
 
	Tim3_M23_ARRSet(u16Period, TRUE);                       //��������ֵ,��ʹ�ܻ���
    
    Tim3_M23_CCR_Set(Tim3CCR0A, u16CHxACompare);            //����CH0�Ƚ�ֵA 
    
    stcTim3PortCmpCfg.enCHxACmpCtrl   = Tim3PWMMode2;         //OCREFA�������OCMA:PWMģʽ2
    stcTim3PortCmpCfg.enCHxAPolarity  = Tim3PortPositive;     //�������
    stcTim3PortCmpCfg.bCHxACmpBufEn   = TRUE;                 //Aͨ���������
    stcTim3PortCmpCfg.enCHxACmpIntSel = Tim3CmpIntNone;       //Aͨ���Ƚ��жϿ���:��
 
    Tim3_M23_PortOutput_Cfg(Tim3CH0, &stcTim3PortCmpCfg);     //�Ƚ�����˿�����	
    
    u8ValidPeriod = 0;                                        //�¼������������ã�0��ʾ��ݲ�ÿ�����ڸ���һ�Σ�ÿ+1�����ӳ�1������
    Tim3_M23_SetValidPeriod(u8ValidPeriod);                   //�����������
																
    u16CntValue = 0;
    Tim3_M23_Cnt16Set(u16CntValue);                           //���ü�����ֵ
	
    Tim3_ClearAllIntFlag();                                   //���жϱ�־
    //Tim3_Mode23_EnableIrq(Tim3UevIrq);                        //ʹ��TIM3 UEV�����ж�
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
    EnableNvic(TIM3_IRQn, IrqLevel3, FALSE);                   //TIM3�ж�ʹ��
}

void pwmStart(void)
{
     //App_ClockCfg_pwm();                       //ʱ�ӳ�ʼ��
	
      App_Timer3PortCfg();                       //Timer3 Port�˿�����
    
      App_Timer3Cfg(500, 0);     //Timer3 ����:���� 0x9000; CH0/1/2ͨ��A�Ƚ�ֵ0x6000; CH0/1/2ͨ��B�Ƚ�ֵ0x3000
    
      Tim3_M23_EnPWM_Output(TRUE, FALSE);        //�˿����ʹ��
    
      Tim3_M23_Run();                            //���С�
}
void compareSet(uint16_t u16CHxACompare)
{
    Tim3_M23_CCR_Set(Tim3CCR0A, u16CHxACompare);            //����CH0�Ƚ�ֵA 
}
