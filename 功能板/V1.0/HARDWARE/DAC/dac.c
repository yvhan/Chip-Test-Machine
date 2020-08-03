#include "dac.h"

DAC_HandleTypeDef DAC1_Handler;			//DAC句柄
DAC_ChannelConfTypeDef DACCH1_Config;	//DAC1通道1
DAC_ChannelConfTypeDef DACCH2_Config;	//DAC1通道2
	
//初始化DAC
void DAC1_Init(void)
{
    DAC1_Handler.Instance=DAC1;
    HAL_DAC_Init(&DAC1_Handler);               	//初始化DAC
 //通道1
    DACCH1_Config.DAC_Trigger=DAC_TRIGGER_NONE;             //不使用触发功能
    DACCH1_Config.DAC_OutputBuffer=DAC_OUTPUTBUFFER_DISABLE;//DAC1输出缓冲关闭
    HAL_DAC_ConfigChannel(&DAC1_Handler,&DACCH1_Config,DAC_CHANNEL_1);//DAC通道1配置
 //通道2
	  DACCH2_Config.DAC_Trigger=DAC_TRIGGER_NONE;             //不使用触发功能
    DACCH2_Config.DAC_OutputBuffer=DAC_OUTPUTBUFFER_DISABLE;//DAC1输出缓冲关闭
    HAL_DAC_ConfigChannel(&DAC1_Handler,&DACCH2_Config,DAC_CHANNEL_2);//DAC通道2配置
    
    HAL_DAC_Start(&DAC1_Handler,DAC_CHANNEL_1);  //开启DAC通道1
	  HAL_DAC_Start(&DAC1_Handler,DAC_CHANNEL_2);  //开启DAC通道2
}

//DAC底层驱动，时钟配置，引脚 配置
//此函数会被HAL_DAC_Init()调用
//hdac:DAC句柄
void HAL_DAC_MspInit(DAC_HandleTypeDef* hdac)
{      
    GPIO_InitTypeDef GPIO_Initure;
	
    __HAL_RCC_DAC12_CLK_ENABLE();           //使能DAC时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();			//开启GPIOA时钟
	
    GPIO_Initure.Pin=GPIO_PIN_4|GPIO_PIN_5;            //PA4\5--通道1和2
    GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //模拟
    GPIO_Initure.Pull=GPIO_NOPULL;          //不带上下拉
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
}

//设置通道1输出电压
//vol:0~6,代表0~6V
void DAC1CH1_Set_Vol(float vol)
{
	double temp1=vol;
	//temp1/=1000;
	temp1=temp1*4096/6;
	
	HAL_DAC_SetValue(&DAC1_Handler,DAC_CHANNEL_1,DAC_ALIGN_12B_R,temp1);//12位右对齐数据格式设置DAC值
}
void DAC1CH1_Set_Cur(float cur)
{
	double temp1=cur;
	//temp1/=1000;
	temp1=temp1*2048;
	
	HAL_DAC_SetValue(&DAC1_Handler,DAC_CHANNEL_1,DAC_ALIGN_12B_R,temp1);//12位右对齐数据格式设置DAC值
}
//设置通道2输出电压
//vol:0~6,代表0~6V
void DAC1CH2_Set_Vol(float vol)
{
	double temp2=vol;
	//temp2/=1000;
	temp2=temp2*4096/6;
	
	HAL_DAC_SetValue(&DAC1_Handler,DAC_CHANNEL_2,DAC_ALIGN_12B_R,temp2);//12位右对齐数据格式设置DAC值
}
