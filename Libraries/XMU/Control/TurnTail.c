#include "GlobalVar.h"
#include "headfile.h"
#include "TurnTail.h"
#define _ANGLE imu_data.yaw
#define GroundAngle 9.73
#define BalanceAngle -19
int StayDistance = 3000, TurntailDistance = 9000;
int TurnTailFlag = 0, TurnTailGoFlag = 0;
int TurnNoTailFlag = 0, TurnNoTailGoFlag = 0;
float flipgyro = 0, acc_Xpeed = 0;
void TurnTail()
{
	//  flipgyro+= sensor.Gyro_deg.z * 0.002;
	static float lastangle, acc_Speed = 0, Broken_Speed, lastspeed, first, initangleset, number = 0, initmode;
	static int TurnTail = 0, count = 0, initcameraopen, initindopen;
	    // if(2==Img_BrokenFlag&&g_mode==3&&g_drive_flag&&!number)  //ce shi yong
	    // {           TurnTailFlag=1;
	    //             number=1;
	    //        initangleset = g_angle_set;
	    //     }

	if (!number)
	{
		number = 1;
		initangleset = g_angle_set;
	}

	if (TurnTailFlag)
	{

		switch (TurnTail)
		{
		case 0: //冲锋陷阵  防止没过完断路
		{
			acc_Speed += curSpeed;
			gpio_init(D2, GPO, 0);
			AD_DirectionControl();
			DirectionControlOutput();
			if (acc_Speed > 1000)
			{

				acc_Xpeed = acc_Speed;
				initmode = g_mode;
				initcameraopen = g_camera_open;
				initindopen = g_ind_open;
				g_camera_open = 0;
				g_ind_open = 0;
				lastspeed = curSpeed;
				lastangle = _ANGLE;

				TurnTail = 1;
				acc_Speed = 0;
				flipgyro = 0;
			}
			break;
		}
		case 1: //拍地板
		{
			count++;
			if (count < 100)
			{
				g_mode = 5;
				g_angle_set = GroundAngle;
				AngleControl();
			}
			else if (count >= 100 && count < 500)
			{
				g_drive_flag = 0;
			}
			else if (count >= 500)
			{
				g_drive_flag = 1;

				TurnTail = 2;
				g_angle_set = BalanceAngle;
				count = 0;
				flipgyro = 0;
			}
			break;
		}

		case 2: //起身甩头
		{
			acc_Speed += curSpeed;
			if (flipgyro <= 100)
			{
                              g_drive_flag=1;
				g_mode = 2;
				g_angle_set = BalanceAngle;
				AngleControl();
				flipgyro += sensor.Gyro_deg.z * 0.002;
				g_fDirectionControlOut = BrokenTurnTailPWM;
				lastangle = _ANGLE;
			}
			else if (flipgyro > 100)
			{
                          g_drive_flag=1;
				acc_Speed = 0;
				g_mode = 1;
				TurnTail = 3;
				g_fDirectionControlOut = 0;
				lastangle = _ANGLE;

				count = 0;
				flipgyro = 0;
			}
			break;
		}
		case 3: //再拍地板
		{
			count++;
			if (count < 50)
			{
                          g_drive_flag=1;
				g_mode = 5;
				g_angle_set = GroundAngle;
				AngleControl();
			}
			else if (count >= 50)
			{
				g_drive_flag = 0;
				TurnTail = 4;
				count = 0;
				g_StateMaster = StateTwo;
		
			}
			break;
		}
		case 4:
		{
			if (TurnTailGoFlag || count < 300)
			{
				count++;
			}
			if (count <= 300)
			{
				g_drive_flag = 0;
			}
			else if (count > 400+staycount+StayTime)
			{
                          g_drive_flag=1;
				acc_Speed += curSpeed;
				if (acc_Speed < OutMeetingDistance1)
				{
					gpio_init(D2, GPO, 1);
					g_angle_set = initangleset;
					g_mode = 3;
					g_drive_flag = 1;
					g_fDirectionControlOut = 0;
					AngleControl();
					AD_DirectionControl();
					DirectionControlOutput();
					SpeedControl();
					SpeedControlOutput();
					Img_BrokenFlag = 0;
					Img_BlockFlag = 0;
					Img_RampFlag = 0;
					g_handle_open = 1;
				}
				if (acc_Speed >= OutMeetingDistance1)
				{
					g_GetMeetingFlag = 0;
					TurnTailGoFlag = 0;
					TurnTail = 5;
					TurnTailFlag = 0;
					Img_BrokenFlag = 0;
					Img_BlockFlag = 0;
					Img_RampFlag = 0;
					CircleFlag = 0;
					CircleState = 0;
          staycount=0;
					g_StateMaster = WaitingStop;
					count = 0;
				}
				else if (!Img_BrokenFlag)
				{
					Broken_Speed += curSpeed;
					if (Broken_Speed > 1000)
					{
						g_GetMeetingFlag = 0;
						TurnTailGoFlag = 0;
						TurnTail = 5;
						TurnTailFlag = 0;
						Img_BrokenFlag = 0;
						Img_BlockFlag = 0;
						Img_RampFlag = 0;
						CircleFlag = 0;
						CircleState = 0;
						g_StateMaster = WaitingStop;
						count = 0;
					}
				}
			}
			if (count >= 50000)
			{
				g_GetMeetingFlag = 0;
				TurnTailGoFlag = 0;
				TurnTail = 5;
				TurnTailFlag = 0;
				Img_BrokenFlag = 0;
				Img_BlockFlag = 0;
				Img_RampFlag = 0;
				CircleFlag = 0;
				CircleState = 0;
                                g_drive_flag = 0;
				g_StateMaster = WaitingStop;
				count = 0;
			}
			break;
		}
		default:
			break;
		}
	}
}

void TurnNoTail()
{
	static float lastangle, acc_Speed = 0, Broken_Speed = 0, lastspeed, first, initangleset, number = 0, initmode;
	static int TurnNoTail = 0, count = 0, initcameraopen, initindopen;
	//   if(2==Img_BrokenFlag&&g_mode==3&&g_drive_flag&&!number)  //ce shi yong
	//    {           TurnTailFlag=1;
	//                number=1;
	//           initangleset = g_angle_set;
	//        }
	if (!number)
	{
		number = 1;
		initangleset = g_angle_set;
	}
	if (TurnNoTailFlag)
	{
		switch (TurnNoTail)
		{
		case 0: //冲锋陷阵  防止没过完断路
		{
			acc_Speed += curSpeed;

			AD_DirectionControl();
			DirectionControlOutput();
			if (acc_Speed > 300)
			{
				gpio_init(D2, GPO, 0);
				acc_Xpeed = acc_Speed;
				initmode = g_mode;
				initcameraopen = g_camera_open;
				initindopen = g_ind_open;
				g_camera_open = 0;
				g_ind_open = 0;
				lastspeed = curSpeed;
				lastangle = _ANGLE;

				TurnNoTail = 1;
				acc_Speed = 0;
				flipgyro = 0;
			}
			break;
		}
		case 1: //拍地板
		{
			count++;
			acc_Speed += curSpeed;
			if (g_StateSlave >= StateTwo)
			{
				AD_DirectionControl();
				DirectionControlOutput();
				if (acc_Speed > 1200)
				{
					TurnNoTail = 2;
					acc_Speed = 0;
				}
			}
			else
			{
				if (count < 50)
				{
					g_mode = 5;
					g_angle_set = GroundAngle;
					AngleControl();
				}
				else if (count >= 50 && count < 1000)
				{
					g_drive_flag = 0;
				}
				else if (count >= 2000)
				{
					TurnNoTail = 2;
					g_angle_set = BalanceAngle;
					count = 0;
					flipgyro = 0;
					g_StateMaster = StateTwo;
					acc_Speed = 0;
				}
				break;
			}
		}
		case 2:
		{
			if (TurnNoTailGoFlag || count < 300)
			{
				count++;
				acc_Speed = 0;
			}
			if (count <= 300 && g_StateSlave < StateTwo)
			{
				g_drive_flag = 0;
				acc_Speed = 0;
			}
			else if (count > 400 || g_StateSlave >= StateTwo)
			{
				acc_Speed += curSpeed;
				count++;
				if (acc_Speed < OutMeetingDistance2)
				{
					g_angle_set = initangleset;
					g_mode = 3;
					g_drive_flag = 1;
					AngleControl();
					AD_DirectionControl();
					DirectionControlOutput();
					ExpectSpeedGet();
					SpeedControl();
					SpeedControlOutput();
				}
				if (acc_Speed > OutMeetingDistance2)
				{
					TurnNoTailGoFlag = 1;
					g_GetMeetingFlag = 0;
					TurnNoTail = 5;
					TurnNoTailFlag = 0;
					Img_BrokenFlag = 0;
					Img_BlockFlag = 0;
					Img_RampFlag = 0;
					g_StateMaster = WaitingStop;
				}
				else if (!Img_BrokenFlag)
				{
					Broken_Speed += curSpeed;
					if (Broken_Speed > 4000)
					{
						TurnNoTailGoFlag = 0;
						g_GetMeetingFlag = 0;
						TurnNoTail = 5;
						TurnNoTailFlag = 0;
						Img_BrokenFlag = 0;
						Img_BlockFlag = 0;
						Img_RampFlag = 0;
						g_StateMaster = WaitingStop;
					}
				}
				if (count > 10000)
				{
					TurnNoTailGoFlag = 0;
					g_GetMeetingFlag = 0;
					TurnNoTail = 5;
					TurnNoTailFlag = 0;
					Img_BrokenFlag = 0;
					Img_BlockFlag = 0;
					Img_RampFlag = 0;

					g_StateMaster = WaitingStop;
				}
			}

			break;
		}
		default:
			break;
		}
	}
}
