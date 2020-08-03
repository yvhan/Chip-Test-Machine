#include "iwdg.h"
#include "sys.h"
	

IWDG_HandleTypeDef IWDG_Handler; //�������Ź����

//��ʼ���������Ź�
//prer:��Ƶ��:0~7(ֻ�е�3λ��Ч!)
//rlr:�Զ���װ��ֵ,0~0XFFF.
//��Ƶ����=4*2^prer.�����ֵֻ����256!
//rlr:��װ�ؼĴ���ֵ:��11λ��Ч.
//ʱ�����(���):Tout=((4*2^prer)*rlr)/32 (ms).
void IWDG_Init(u8 prer,u16 rlr)
{
    IWDG_Handler.Instance=IWDG1;
    IWDG_Handler.Init.Prescaler=prer;    //����IWDG��Ƶϵ��
    IWDG_Handler.Init.Reload=rlr;        //��װ��
    IWDG_Handler.Init.Window=IWDG_WINDOW_DISABLE;//�رմ��ڹ���
    HAL_IWDG_Init(&IWDG_Handler);    
}
    
//ι�������Ź�
void IWDG_Feed(void)
{   
    HAL_IWDG_Refresh(&IWDG_Handler); //��װ��
}
