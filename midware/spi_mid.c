 
#include "ddl.h"
#include "spi.h"
#include "gpio.h"


#include "spi_mid.h"
void WriteData(unsigned int i)
{
	Spi_SetCS(M0P_SPI0,  FALSE  );
	
	Gpio_ClrIO(GpioPortA, GpioPin15);//cs
	//P05=1;//rs
	Gpio_SetIO(GpioPortB, GpioPin4);
	//SendDataSPI(i);
	while(Spi_GetStatus(M0P_SPI0, SpiTxe) == FALSE)    //等待发送缓冲器空
	{;}
	Spi_SendData(M0P_SPI0,i );//tx_buf[tx_cnt++]
	 
	Spi_SetCS(M0P_SPI0, TRUE);
	Gpio_SetIO(GpioPortA, GpioPin15);//cs=1
}

void WriteComm(unsigned int i)
{
	
	Spi_SetCS(M0P_SPI0,  FALSE  );
	Gpio_ClrIO(GpioPortA, GpioPin15);//cs
	Gpio_ClrIO(GpioPortB, GpioPin4);//rs
	//SendDataSPI(i);
	while(Spi_GetStatus(M0P_SPI0, SpiTxe) == FALSE) //等待发送缓冲器空
	{;}
	Spi_SendData(M0P_SPI0,i );//tx_buf[tx_cnt++]
	//CS0 = 1;
	//delay1us(1);
	Spi_SetCS(M0P_SPI0, TRUE);
	Gpio_SetIO(GpioPortA, GpioPin15);//cs=1
}

void LcdspiInit(void)
{
    stc_spi_cfg_t  SpiInitStruct;
    Sysctrl_SetPeripheralGate(SysctrlPeripheralSpi0,TRUE);

    //SPI0模块配置：主机
    SpiInitStruct.enSpiMode = SpiMskMaster;   //配置位主机模式
    SpiInitStruct.enPclkDiv = SpiClkMskDiv2;  //波特率：pclk/2
    SpiInitStruct.enCPHA    = SpiMskCphafirst;//第一边沿采样
    SpiInitStruct.enCPOL    = SpiMskcpollow;  //极性为低
    Spi_Init(M0P_SPI0, &SpiInitStruct);
}

