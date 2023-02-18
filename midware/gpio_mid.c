

#include "gpio.h"
#include "gpio_mid.h"

/**
 ******************************************************************************
 ** \brief  初始化外部GPIO引脚
 **
 ** \return 无
 ******************************************************************************/
void LcdgpioInit(void)
{	
    stc_gpio_cfg_t stcGpioCfg;
    
    ///< 打开GPIO外设时钟门控
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE); 
    ///< 端口方向配置->输出(其它参数与以上（输入）配置参数一致)
    stcGpioCfg.enDir = GpioDirOut;
    ///< 端口上下拉配置->
    stcGpioCfg.enPu = GpioPuDisable;
    stcGpioCfg.enPd = GpioPdDisable;
    ///< LED关闭
    Gpio_ClrIO(GpioPortB, GpioPin4);
    ///< GPIO IO LED端口初始化
    Gpio_Init(GpioPortB, GpioPin4, &stcGpioCfg);
    
	
    ///< 打开GPIO外设时钟门控
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE); 
    ///< 端口方向配置->输出(其它参数与以上（输入）配置参数一致)
    stcGpioCfg.enDir = GpioDirOut;
    ///< 端口上下拉配置->
    stcGpioCfg.enPu = GpioPuDisable;
    stcGpioCfg.enPd = GpioPdDisable;
    Gpio_SetIO(GpioPortA, GpioPin15);
    ///< GPIO IO LED端口初始化
    Gpio_Init(GpioPortA, GpioPin15, &stcGpioCfg);
	 
}

void LcdgpiospiInit(void)
{	
    stc_gpio_cfg_t GpioInitStruct;
    DDL_ZERO_STRUCT(GpioInitStruct);

    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);

    //SPI0引脚配置:主机
    GpioInitStruct.enDrv = GpioDrvH;
    GpioInitStruct.enDir = GpioDirOut;

    Gpio_Init(GpioPortA, GpioPin12,&GpioInitStruct);
    Gpio_SetAfMode(GpioPortA, GpioPin12,GpioAf6);        //配置引脚PA12作为SPI0_MOSI

    Gpio_Init(GpioPortA, GpioPin15,&GpioInitStruct);
    Gpio_SetAfMode(GpioPortA, GpioPin15,GpioAf1);         //配置引脚PA15作为SPI0_CS

    Gpio_Init(GpioPortB, GpioPin3,&GpioInitStruct);
    Gpio_SetAfMode(GpioPortB, GpioPin3,GpioAf1);         //配置引脚PB3作为SPI0_SCK

}




