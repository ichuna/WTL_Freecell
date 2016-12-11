// CardSize.cpp: implementation of the CCardSize class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CardSize.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

int CCardSize::m_nWinWidth = 1210;

CCardSize::CCardSize()
{
	SetCardSize();
}

CCardSize::~CCardSize()
{

}

VOID CCardSize::SetCardSize(int nWinWide)
{
	dRate = (double)nWinWide / (double)nWindWidth0;

	ptOrg.x = int(1 * dRate), ptOrg.y = int(17 * dRate);	//牌局左上角
	CARD_HEI = int(94 * dRate);		// 每张牌的高度height
	CARD_WID = int(69 * dRate);		// 每张牌的宽度width
	CARD_INT = int(10 * dRate);		// 代表1-8列中每两堆牌之间的间隔(interval)。(第9,16两列分别与窗口两边的间距也是这个值)
	PILE_VINT = int(5 * dRate);		// 牌列与缓存列间垂直方向上的间隔
	PILE_HINT = 9 * CARD_INT;		// 缓存列与回收列之间的水平间隔。该变量不要 * dRate，否则第一行右面会越界
	CARD_UNCOVER = int(19 * dRate);	// 当被一张牌压着的时候，此露出的部分的高度
	
	//usage: clr[BIG/SML][HT/HX/MH/FK][UP/DN][X/Y]...
	BIG = 0, SML = 1, big = 16, sml = 8;

	//clr[2][4][2][2]
	clr[0][0][0][0] = 0 * big, clr[0][0][0][1] = 1 * big;	//黑桃上
	clr[0][0][1][0] = 1 * big, clr[0][0][1][1] = 1 * big;	//黑桃下
	clr[0][1][0][0] = 0 * big, clr[0][1][0][1] = 2 * big;	//红桃上
	clr[0][1][1][0] = 1 * big, clr[0][1][1][1] = 2 * big;	//红桃下
	clr[0][2][0][0] = 0 * big, clr[0][2][0][1] = 3 * big;	//梅花上
	clr[0][2][1][0] = 1 * big, clr[0][2][1][1] = 3 * big;	//梅花下
	clr[0][3][0][0] = 0 * big, clr[0][3][0][1] = 4 * big;	//方块上
	clr[0][3][1][0] = 1 * big, clr[0][3][1][1] = 4 * big;	//方块下

	clr[1][0][0][0] = 0 * sml, clr[1][0][0][1] = 0 * sml;
	clr[1][0][1][0] = 0 * sml, clr[1][0][1][1] = 1 * sml;
	clr[1][1][0][0] = 1 * sml, clr[1][1][0][1] = 0 * sml;
	clr[1][1][1][0] = 1 * sml, clr[1][1][1][1] = 1 * sml;
	clr[1][2][0][0] = 2 * sml, clr[1][2][0][1] = 0 * sml;
	clr[1][2][1][0] = 2 * sml, clr[1][2][1][1] = 1 * sml;
	clr[1][3][0][0] = 3 * sml, clr[1][3][0][1] = 0 * sml;
	clr[1][3][1][0] = 3 * sml, clr[1][3][1][1] = 1 * sml;

	//牌A－－牌10的图像数据
	x12   = CARD_WID      /  2;
	x13   = CARD_WID *  9 / 30;
	x23   = CARD_WID * 21 / 30;
	y12   = CARD_HEI      /  2;
	y15   = CARD_HEI      /  5;
	y25   = CARD_HEI *  2 /  5;
	y35   = CARD_HEI *  3 /  5;
	y45   = CARD_HEI *  4 /  5;
	y310  = CARD_HEI *  3 / 10;
	y710  = CARD_HEI *  7 / 10;
	y720  = CARD_HEI *  7 / 20;
	y1320 = CARD_HEI * 13 / 20;
	
	/*
	  cA[] = { x12, y12, 1, };
	  c2[] = { x12, y15, 1, x12, y45, 0, };
	  c3[] = { x12, y15, 1, x12, y45, 0, x12, y12, 1, };
	  c4[] = { x13, y15, 1, x23, y15, 1, x13, y45, 0, x23, y45, 0, };
	  c5[] = { x13, y15, 1, x23, y15, 1, x13, y45, 0, x23, y45, 0, x12, y12, 1, };
	  c6[] = { x13, y15, 1, x23, y15, 1, x13, y45, 0, x23, y45, 0, x13, y12, 1, x23, y12, 1, };
	  c7[] = { x13, y15, 1, x23, y15, 1, x13, y45, 0, x23, y45, 0, x13, y12, 1, x23, y12, 1, x12, y720, 1, };
	  c8[] = { x13, y15, 1, x23, y15, 1, x13, y25, 1, x23, y25, 1, x13, y35, 0, x23, y35, 0, x13,  y45, 0, x23,  y45, 0 };
	  c9[] = { x13, y15, 1, x23, y15, 1, x13, y25, 1, x23, y25, 1, x13, y35, 0, x23, y35, 0, x13,  y45, 0, x23,  y45, 0, x12,  y12, 1, };
	 c10[] = { x13, y15, 1, x23, y15, 1, x13, y25, 1, x23, y25, 1, x13, y35, 0, x23, y35, 0, x13,  y45, 0, x23,  y45, 0, x12, y310, 1, x12, y710, 0, };
	c8FK[] = { x13, y15, 1, x23, y15, 1, x13, y45, 0, x23, y45, 0, x13, y12, 1, x23, y12, 1, x12, y720, 1, x12, y1320, 0, };	
	*/

	/*
	  cA[] = { x12, y12, 1};
	  c2[] = { x12, y15, 1, x12, y45, 0};
	  c3[] = { x12, y15, 1, x12, y45, 0, x12, y12, 1};
	  c4[] = { x13, y15, 1, x23, y15, 1, x13, y45, 0, x23, y45, 0};
	  c5[] = { x13, y15, 1, x23, y15, 1, x13, y45, 0, x23, y45, 0, x12, y12, 1};
	  c6[] = { x13, y15, 1, x23, y15, 1, x13, y45, 0, x23, y45, 0, x13, y12, 1, x23, y12, 1};
	  c7[] = { x13, y15, 1, x23, y15, 1, x13, y45, 0, x23, y45, 0, x13, y12, 1, x23, y12, 1, x12, y720, 1};
	  c8[] = { x13, y15, 1, x23, y15, 1, x13, y25, 1, x23, y25, 1, x13, y35, 0, x23, y35, 0, x13,  y45, 0, x23,  y45,  0};
	  c9[] = { x13, y15, 1, x23, y15, 1, x13, y25, 1, x23, y25, 1, x13, y35, 0, x23, y35, 0, x13,  y45, 0, x23,  y45,  0, x12,  y12, 1};
	 c10[] = { x13, y15, 1, x23, y15, 1, x13, y25, 1, x23, y25, 1, x13, y35, 0, x23, y35, 0, x13,  y45, 0, x23,  y45,  0, x12, y310, 1, x12, y710, 0};
	c8FK[] = { x13, y15, 1, x23, y15, 1, x13, y45, 0, x23, y45, 0, x13, y12, 1, x23, y12, 1, x12, y720, 1, x12, y1320, 0};	
	*/

	  cA[0] = x12,   cA[1] = y12,   cA[2] = 1;
	  c2[0] = x12,   c2[1] = y15,   c2[2] = 1,   c2[3] = x12,   c2[4] = y45,   c2[5] = 0;
	  c3[0] = x12,   c3[1] = y15,   c3[2] = 1,   c3[3] = x12,   c3[4] = y45,   c3[5] = 0,   c3[6] = x12,   c3[7] = y12,   c3[8] = 1;
	  c4[0] = x13,   c4[1] = y15,   c4[2] = 1,   c4[3] = x23,   c4[4] = y15,   c4[5] = 1,   c4[6] = x13,   c4[7] = y45,   c4[8] = 0,   c4[9] = x23,   c4[10] = y45,   c4[11] = 0;
	  c5[0] = x13,   c5[1] = y15,   c5[2] = 1,   c5[3] = x23,   c5[4] = y15,   c5[5] = 1,   c5[6] = x13,   c5[7] = y45,   c5[8] = 0,   c5[9] = x23,   c5[10] = y45,   c5[11] = 0,   c5[12] = x12,   c5[13] = y12,   c5[14] = 1;
	  c6[0] = x13,   c6[1] = y15,   c6[2] = 1,   c6[3] = x23,   c6[4] = y15,   c6[5] = 1,   c6[6] = x13,   c6[7] = y45,   c6[8] = 0,   c6[9] = x23,   c6[10] = y45,   c6[11] = 0,   c6[12] = x13,   c6[13] = y12,   c6[14] = 1,   c6[15] = x23,   c6[16] = y12,   c6[17] = 1;
	  c7[0] = x13,   c7[1] = y15,   c7[2] = 1,   c7[3] = x23,   c7[4] = y15,   c7[5] = 1,   c7[6] = x13,   c7[7] = y45,   c7[8] = 0,   c7[9] = x23,   c7[10] = y45,   c7[11] = 0,   c7[12] = x13,   c7[13] = y12,   c7[14] = 1,   c7[15] = x23,   c7[16] = y12,   c7[17] = 1,   c7[18] = x12,   c7[19] = y720,   c7[20] = 1;
	  c8[0] = x13,   c8[1] = y15,   c8[2] = 1,   c8[3] = x23,   c8[4] = y15,   c8[5] = 1,   c8[6] = x13,   c8[7] = y25,   c8[8] = 1,   c8[9] = x23,   c8[10] = y25,   c8[11] = 1,   c8[12] = x13,   c8[13] = y35,   c8[14] = 0,   c8[15] = x23,   c8[16] = y35,   c8[17] = 0,   c8[18] = x13,   c8[19] = y45,    c8[20] = 0,   c8[21] = x23,   c8[22] = y45,     c8[23] = 0;
	  c9[0] = x13,   c9[1] = y15,   c9[2] = 1,   c9[3] = x23,   c9[4] = y15,   c9[5] = 1,   c9[6] = x13,   c9[7] = y25,   c9[8] = 1,   c9[9] = x23,   c9[10] = y25,   c9[11] = 1,   c9[12] = x13,   c9[13] = y35,   c9[14] = 0,   c9[15] = x23,   c9[16] = y35,   c9[17] = 0,   c9[18] = x13,   c9[19] = y45,    c9[20] = 0,   c9[21] = x23,   c9[22] = y45,     c9[23] = 0,   c9[24] = x12,   c9[25] = y12,    c9[26] = 1;
	 c10[0] = x13,  c10[1] = y15,  c10[2] = 1,  c10[3] = x23,  c10[4] = y15,  c10[5] = 1,  c10[6] = x13,  c10[7] = y25,  c10[8] = 1,  c10[9] = x23,  c10[10] = y25,  c10[11] = 1,  c10[12] = x13,  c10[13] = y35,  c10[14] = 0,  c10[15] = x23,  c10[16] = y35,  c10[17] = 0,  c10[18] = x13,  c10[19] = y45,   c10[20] = 0,  c10[21] = x23,  c10[22] = y45,    c10[23] = 0,  c10[24] = x12,  c10[25] = y310,  c10[26] = 1,  c10[27] = x12,  c10[28] = y710,  c10[29] = 0;
	c8FK[0] = x13, c8FK[1] = y15, c8FK[2] = 1, c8FK[3] = x23, c8FK[4] = y15, c8FK[5] = 1, c8FK[6] = x13, c8FK[7] = y45, c8FK[8] = 0, c8FK[9] = x23, c8FK[10] = y45, c8FK[11] = 0, c8FK[12] = x13, c8FK[13] = y12, c8FK[14] = 1, c8FK[15] = x23, c8FK[16] = y12, c8FK[17] = 1, c8FK[18] = x12, c8FK[19] = y720, c8FK[20] = 1, c8FK[21] = x12, c8FK[22] = y1320, c8FK[23] = 0;	

	//步骤计数的字体高度
	stepFont = int(15 * dRate);
}
