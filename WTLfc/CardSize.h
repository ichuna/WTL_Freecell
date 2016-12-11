// CardSize.h: interface for the CCardSize class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CARDSIZE_H__5F9E903B_D211_40D4_873F_8B2D2DEAEDC4__INCLUDED_)
#define AFX_CARDSIZE_H__5F9E903B_D211_40D4_873F_8B2D2DEAEDC4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//651 499
//660 496
//669 496
//#define nWindWidth0 597
//#define nWindWidth0 623
//#define nWindWidth0 631
#define nWindWidth0 641

class CCardSize
{
public:
	CCardSize();
	virtual ~CCardSize();
	VOID SetCardSize(int nWinWidth = m_nWinWidth);
	//VOID SetCardSize(int nWinWidth = nWindWidth0);

public:
	static int m_nWinWidth;	// 1210
	double dRate;

	CPoint ptOrg;		//(1,17);		// �ƾ����Ͻ�
	
	int CARD_HEI;		// = 94;		// ÿ���Ƶĸ߶�height
	int CARD_WID;		// = 69;		// ÿ���ƵĿ��width
	int CARD_INT;		// = 8;			// ����1-8����ÿ������֮��ļ��(interval)��(��9,16���зֱ��봰�����ߵļ��Ҳ�����ֵ)
	int PILE_VINT;		// = 8;			// �����뻺���м䴹ֱ�����ϵļ��
	int PILE_HINT;		// = 9*CARD_INT;// �������������֮���ˮƽ���
	int CARD_UNCOVER;	// = 18;		// ����һ����ѹ�ŵ�ʱ�򣬴�¶���Ĳ��ֵĸ߶�
	
	//usage: clr[BIG/SML][HT/HX/MH/FK][UP/DN][X/Y]...
	//UCHAR BIG=0, SML=1, big=16, sml=8;
	UCHAR BIG, SML, big, sml;
	UCHAR clr[2][4][2][2];
	/* = {
		{	{	{0 *big,1 *big},//������
				{1 *big,1 *big},//������
			},{	{0 *big,2 *big},//������
				{1 *big,2 *big},//������
			},{	{0 *big,3 *big},//÷����
				{1 *big,3 *big},//÷����
			},{	{0 *big,4 *big},//������
				{1 *big,4 *big},//������
		}
		},{	{	{0 *sml,0 *sml},
				{0 *sml,1 *sml}
			},{	{1 *sml,0 *sml},
				{1 *sml,1 *sml}
			},{	{2 *sml,0 *sml},
				{2 *sml,1 *sml}
			},{	{3 *sml,0 *sml},
				{3 *sml,1 *sml}
		}
		}
	};*/

	//��A������10��ͼ������
	UCHAR x12      ;	// = CARD_WID      /  2;
	UCHAR x13      ;	// = CARD_WID *  9 / 30;
	UCHAR x23      ;	// = CARD_WID * 21 / 30;
	UCHAR y12      ;	// = CARD_HEI      /  2;
	UCHAR y15      ;	// = CARD_HEI      /  5;
	UCHAR y25      ;	// = CARD_HEI *  2 /  5;
	UCHAR y35      ;	// = CARD_HEI *  3 /  5;
	UCHAR y45      ;	// = CARD_HEI *  4 /  5;
	UCHAR y310     ;	// = CARD_HEI *  3 / 10;
	UCHAR y710     ;	// = CARD_HEI *  7 / 10;
	UCHAR y720     ;	// = CARD_HEI *  7 / 20;
	UCHAR y1320    ;	// = CARD_HEI * 13 / 20;
	
	UCHAR cA[3]    ;	// = { x12 , y12 , 1, };
	UCHAR c2[6]    ;	// = { x12 , y15 , 1, x12 , y45 , 0, };
	UCHAR c3[9]    ;	// = { x12 , y15 , 1, x12 , y45 , 0, x12 , y12 , 1, };
	UCHAR c4[12]   ;	// = { x13 , y15 , 1, x23 , y15 , 1, x13 , y45 , 0, x23 , y45 , 0, };
	UCHAR c5[15]   ;	// = { x13 , y15 , 1, x23 , y15 , 1, x13 , y45 , 0, x23 , y45 , 0, x12 , y12 , 1, };
	UCHAR c6[18]   ;	// = { x13 , y15 , 1, x23 , y15 , 1, x13 , y45 , 0, x23 , y45 , 0, x13 , y12 , 1, x23 , y12 , 1, };
	UCHAR c7[21]   ;	// = { x13 , y15 , 1, x23 , y15 , 1, x13 , y45 , 0, x23 , y45 , 0, x13 , y12 , 1, x23 , y12 , 1, x12 , y720 , 1 , };
	UCHAR c8[24]   ;	// = { x13 , y15 , 1, x23 , y15 , 1, x13 , y25 , 1, x23 , y25 , 1, x13 , y35 , 0, x23 , y35 , 0, x13 , y45  , 0 , x23 , y45   , 0 };
	UCHAR c9[27]   ;	// = { x13 , y15 , 1, x23 , y15 , 1, x13 , y25 , 1, x23 , y25 , 1, x13 , y35 , 0, x23 , y35 , 0, x13 , y45  , 0 , x23 , y45   , 0 , x12 , y12  , 1 , };
	UCHAR c10[30]  ;	// = { x13 , y15 , 1, x23 , y15 , 1, x13 , y25 , 1, x23 , y25 , 1, x13 , y35 , 0, x23 , y35 , 0, x13 , y45  , 0 , x23 , y45   , 0 , x12 , y310 , 1 , x12 , y710 , 0 , };
	UCHAR c8FK[24] ;	// = { x13 , y15 , 1, x23 , y15 , 1, x13 , y45 , 0, x23 , y45 , 0, x13 , y12 , 1, x23 , y12 , 1, x12 , y720 , 1 , x12 , y1320 , 0 , };

	//�������������߶�
	UINT stepFont;		// = 22;
};

#endif // !defined(AFX_CARDSIZE_H__5F9E903B_D211_40D4_873F_8B2D2DEAEDC4__INCLUDED_)
