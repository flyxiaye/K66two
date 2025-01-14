#include "GlobalVar.h"
#include "BasicFun.h"
#include "FindLine.h"
#include "FillSpecialLine.h"
#include "MainProcess.h"
#include "CircleIsland.h"
#include "FirstLineProcess.h"
#include "canny.h"
#include "SpecialElem.h"

//================================================================//
//  @brief  :		首行搜线采用函数
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void SelectFirstLine(void)
{
	FirstLineV4();
}
//================================================================//
//  @brief  :		普通补图主程序
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void MainFill(void)
{
	VarInit();
	SelectFirstLine();
	g_RoadType = FirstRowProcess();

	if (0 == g_RoadType)
	{
		FindLineNormal(1);
		ImgJudgeStopLine();		//识别停车
		ImgJudgeObstacle();     //识别坡道路障直道断路					
		ImgJudgeCurveBroken();	//弯道断路
		if (Img_GrayJumpOpen)
		{
			if (!Img_SpecialElemFlag && ImgJudgeOutBroken())
			{
				Img_BrokenFlag = 2;
				Img_SpecialElemFlag = 1;
			}
			//ImgJudgeOutBroken();			//灰度跳变检测断路
		}

#if CIRCLE == 2
		//CircleFlag = ImgJudgeCircle(0);
		CircleFlag = Img_JudgeCircleIsland(0);
		if (CL == CircleFlag)
		{
			int tmp_row = LeftPnt.ErrRow;
			GetPointA();
			GetPointB();
			GetPointC();
			GetPointD();
			FillLineAB();
			FillLineCD();
			FillAllEage();
			CircleFlag = CN;
			if (Ind_CircleOpen)
				CircleFlag = CN;
			else
			{
				if (tmp_row > DOWN_EAGE - 10)
					;
				else CircleFlag = CN;
			}
		}
		else if (CR == CircleFlag)
		{
			int tmp_row = RightPnt.ErrRow;
			GetPointA();
			GetPointB();
			GetPointC();
			GetPointD();
			FillLineAB();
			FillLineCD();
			FillAllEage();
			CircleFlag = CN;
			if (Ind_CircleOpen)
				CircleFlag = CN;
			else
			{
				if (tmp_row > DOWN_EAGE - 10)
					;
				else CircleFlag = CN;
			}
		}
		else
#endif // CIRCLE
			if (LeftPnt.Type == 2 && RightPnt.Type == 2)		//十字补图
			{
				if (LeftPnt.ErrRow - RightPnt.ErrRow > 10 || RightPnt.ErrRow - LeftPnt.ErrRow > 10)
					FillBevelCross();
				else
					FillLevelCross();
				FindLineNormal(0);

			}
	}
	if (1 == g_RoadType)
	{
		FindLineLost();
#if CIRCLE == 2
		//CircleFlag = ImgJudgeCircle(1);
		CircleFlag = Img_JudgeCircleIsland(1);
		if (CL == CircleFlag)
		{
			GetPointA();
			GetPointB();
			GetPointC();
			GetPointD();
			FillLineAB();
			FillLineCD();
			FillAllEage();
			if (Ind_CircleOpen)
				CircleFlag = CN;
		}
		else if (CR == CircleFlag)
		{
			GetPointA();
			GetPointB();
			GetPointC();
			GetPointD();
			FillLineAB();
			FillLineCD();
			FillAllEage();
			if (Ind_CircleOpen)
				CircleFlag = CN;
		}
		else
#endif
			if (1 == g_RoadType && 2 == LeftPnt.Type && 2 == RightPnt.Type)
			{
				FillBevelCross();
				FindLineNormal(0);
			}
	}
	if (2 == g_RoadType)
	{
		FillFourCross();
		FindLineNormal(0);
	}
	FillMiddleLine();
}


//================================================================//
//  @brief  :		补图主程序
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void GetML(void)
{
	CannyEage();
#if CIRCLE
	if (CircleFlag)		//is CircleIsland 
	{
		CircleFill();
	}
	else
#endif // CIRCLE
	{
		if (Img_SpecialElemFlag)
			SpecialElemFill();
		if (!Img_SpecialElemFlag)
			MainFill();
	}

	//中线校验
	if (RL[DOWN_EAGE] - LL[DOWN_EAGE] <= 40 || ML_Count > DOWN_EAGE - 20		//下边界过小，有效行数过低
		|| RightPnt.ErrCol - LeftPnt.ErrCol > 100)									//上边界不收敛
	{
		ErrorFlag = 4;
	}
	if (!ErrorFlag)
		SpeedRow = GetSpeedRow(ControlMid, LeftPnt.ErrRow, RightPnt.ErrRow);
	//	if (!ErrorFlag && UP_EAGE + 1 >= SpeedRow)
	//		Ind_LongRoadFlag = 1;
	//	else Ind_LongRoadFlag = 0;


}
