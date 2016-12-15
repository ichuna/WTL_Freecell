// aboutdlg.cpp : implementation of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"

CAboutDlg::CAboutDlg(COLORREF clrText)
{
	//Ĭ����ɫ
	//m_brushBkgnd.CreateSolidBrush(::GetSysColor(COLOR_BTNFACE));
	//m_brushBkgnd.CreateSolidBrush(RGB(218, 218, 218));
	m_brushBkgnd.CreateSolidBrush(::GetSysColor(COLOR_BTNFACE));
	m_clrText = clrText;
}

LRESULT CAboutDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CenterWindow(GetParent());

	//����Ȩ��Ϣ�����б����
	wchar_t *msg[] =
	{
		TEXT("--------------------------------------------"),
		TEXT("WTLfc�յ�����4.00��"),
		TEXT("--------------------------------------------"),
		TEXT("����      ���򺣷�"),
		TEXT("E-mail    ��xianghaifei@sohu.com"),
		TEXT("blog      ��http://blog.csdn.net/xianghaifei/"),
		TEXT("Improveed ��ybmj@vip.163.com"),
		TEXT("--------------------------------------------"),
	};

	//CListBox *plb = (CListBox *)GetDlgItem(IDLB_ABOUT);
	CListBox lb(::GetDlgItem(m_hWnd, IDLB_ABOUT));
	int cnt = 0;
	while (cnt < sizeof(msg) / sizeof(char*))
		lb.AddString(msg[cnt++]);

	return TRUE;		// return TRUE unless you set the focus to a control
					// EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CAboutDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}

// �ı�Ի��򱳾�ɫ
LRESULT CAboutDlg::OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
	//CBrush brushBkgnd;    //����ɫ,
	//brushBkgnd.CreateSolidBrush(RGB(192,192,192));

	//T* pThis = static_cast<T*>(this);
	HDC hDc = (HDC)wParam;
	RECT rcRect;
	//pThis->GetClientRect(&rcRect);
	GetClientRect(&rcRect);
	FillRect(hDc, &rcRect, m_brushBkgnd);
	return 1;
}

LRESULT CAboutDlg::OnCtlColorDlg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	HDC hDc = (HDC)wParam;
	switch (uMsg)
	{
	case WM_CTLCOLORSTATIC:
	case WM_CTLCOLORBTN:
	case WM_CTLCOLORMSGBOX:
	case WM_CTLCOLORLISTBOX:
	{
		::SetBkMode(hDc, TRANSPARENT);
		::SetTextColor(hDc, m_clrText);
		bHandled = TRUE;
		return (LRESULT)m_brushBkgnd.m_hBrush;
	}
	break;
	case WM_CTLCOLORDLG:
	case WM_CTLCOLOREDIT:
	default:
		break;
	}
	return 0;
}
