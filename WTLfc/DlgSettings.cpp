// DlgSettings.cpp : implementation file
//

#include "stdafx.h"
//#include "JL.h"
#include "DlgSettings.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSettings dialog


//CDlgSettings::CDlgSettings(CWnd* pParent /*=NULL*/)
//	: CDialog(CDlgSettings::IDD, pParent)
//{
//	//{{AFX_DATA_INIT(CDlgSettings)
//	//}}AFX_DATA_INIT
//}


//void CDlgSettings::DoDataExchange(CDataExchange* pDX)
//{
//	CDialog::DoDataExchange(pDX);
//	//{{AFX_DATA_MAP(CDlgSettings)
//	DDX_Control(pDX, IDSLI_DDASPEED, m_sliDDASpeed);
//	DDX_Check(pDX, IDCHK_CHECKALERT, m_bEnableAlert);
//	DDX_Check(pDX, IDCHK_CHECKQUICKLY, m_bQuickMove);
//	DDX_Check(pDX, IDCHK_ENABLELBDCLICK, m_bEnableDBClick);
//	DDX_Check(pDX, IDCHK_MOVEMAX, m_bMaxMove);
//	DDX_Slider(pDX, IDSLI_DDASPEED, m_nDDASpeed);
//	//}}AFX_DATA_MAP
//}


//BEGIN_MESSAGE_MAP(CDlgSettings, CDialog)
//	//{{AFX_MSG_MAP(CDlgSettings)
//	//}}AFX_MSG_MAP
//END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSettings message handlers

LRESULT CDlgSettings::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CenterWindow(GetParent());
	
	//// WTL ��֧�� CTrackBarCtrl DDX������ֻ���ֹ��󶨺�����
	m_sliDDASpeed = (CTrackBarCtrl)GetDlgItem(IDSLI_DDASPEED);

	//m_sliDDASpeed.ModifyStyle(WS_BORDER, NULL, SWP_DRAWFRAME);
	//m_sliDDASpeed.ModifyStyle(NULL, WS_BORDER, SWP_DRAWFRAME);
	m_sliDDASpeed.SetRange(1, 99);
	m_sliDDASpeed.SetPageSize(9);
	m_sliDDASpeed.SetTicFreq(9);
	m_sliDDASpeed.SetPos(m_nDDASpeed);
	//// WTL ��֧�� CTrackBarCtrl DDX������ֻ���ֹ��󶨺�����

	// First DDX call, hooks up variables to controls.
	DoDataExchange(false);

	//------------------------------------------------------
	//���� m_strRecNofullkbcontrol ��Ϣ
	//m_chkRecNofullkbcontrol.SetCheck(FALSE);
	//m_strRecNofullkbcontrol.MakeLower();

	//if (L"nofullkbcontrol" == m_strRecNofullkbcontrol)	//�����ִ�Сд
	//	m_chkRecNofullkbcontrol.SetCheck(TRUE);

	//------------------------------------------------------


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CDlgSettings::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}

LRESULT CDlgSettings::OnOKCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	//// WTL ��֧�� CTrackBarCtrl DDX������ֻ���ֹ��󶨺�����
	m_nDDASpeed = m_sliDDASpeed.GetPos();
	//// WTL ��֧�� CTrackBarCtrl DDX������ֻ���ֹ��󶨺�����

	// First DDX call, hooks up controls to variables.
	DoDataExchange(true);

	EndDialog(wID);
	return 0;
}
