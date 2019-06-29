#include "headfile.h"
#include "MK60_uart.h"
#include "DFlash.h"
#include "Bluetooth.h"
#include  "isr.h"
#include  "Init.h"
#include  "Basic.h"
#include "DFLASH.h"
#include <GlobalVar.h>
#include  "keyboard.h"


int main(void)
{
static int lala=0;
	get_clk();
	MyFlash_Read(0);
	system_Init();
	systick_delay_ms(1000);
	//    dialSwitchFlg3 = gpio_get(DIALSWITCH_PIN3);
	//    if (dialSwitchFlg3) g_StartSlave = 1;
	while (1)
	{
		dialSwitchFlg1 = gpio_get(DIALSWITCH_PIN1);
		dialSwitchFlg2 = gpio_get(DIALSWITCH_PIN2);
                	dialSwitchFlg3 = gpio_get(DIALSWITCH_PIN3);
		dialSwitchFlg4 = gpio_get(DIALSWITCH_PIN4);
		//    ftm_pwm_duty(ftm0,RIGHT_GO,0);
		//    ftm_pwm_duty(ftm0,RIGHT_BACK,8000);
		//    ftm_pwm_duty(ftm0,LEFT_GO,0);
		//    ftm_pwm_duty(ftm0,LEFT_BACK,8000);
		if (mt9v032_finish_flag)//图像接收完成标志位
		{

			mt9v032_finish_flag = 0;//标志位清零 
			if (1 == g_handle_open)
			{
				GetML();
                                MeetingToImage();
			}
			else;


		}
		if (1 == dialSwitchFlg1 && g_drive_flag == 0)
		{
			displayimage032(image[0]);
			ShowEage();
		}
		else
		{
			Menu();
		}
                if(!dialSwitchFlg3&&!lala&&g_drive_flag)
                {
                lala=1;
                  g_StateMaster=2;
                }
		//Bluebooth_Push_Data();
//                UserData();
//    SendAngle();
    SendData();
                		if (Img_BrokenFlag)
			gpio_init(D0, GPO, 0);
		else gpio_init(D0, GPO, 1);
//		if (CircleFlag)
//			gpio_init(A7, GPO, 1);
//		else gpio_init(A7, GPO, 0);
         
}
}