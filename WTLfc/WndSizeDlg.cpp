// WndSizeDlg.cpp : implementation of the CWndSizeDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "WndSizeDlg.h"

LRESULT CWndSizeDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CenterWindow(GetParent());

	CRect wr;
	//::GetWindowRect(GetParent(), wr);	// �õ�CMainFrame���ھ���
	g_pView->GetClientRect(wr);	// �õ�View���ڿͻ�������
	m_nWidth = wr.Width();
	m_nHeight = wr.Height();

	// First DDX call, hooks up variables to controls.
	DoDataExchange(false);

	// Set the focus, and we're done.
	HWND hEdit = GetDlgItem(IDC_EDIT_WIDTH);	// ����ؼ����hEdit����������ֵ
	CEdit editWidth(hEdit);		// 1��ʹ�ù��캯���������ڱ��� editWidth �����ؼ���� hEdit
	editWidth.SetFocus();
	editWidth.SendMessage(WM_KEYDOWN, VK_END, 0);

	return FALSE; // ���ܷ���TRUE
}

LRESULT CWndSizeDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}

LRESULT CWndSizeDlg::OnOKCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	int nWidth = m_nWidth;
	int nHight = m_nHeight;
	DoDataExchange(true);

	//if (nWidth != m_nWidth || nHight != m_nHeight)
	if (abs(nWidth - m_nWidth) > 5 || abs(nHight - m_nHeight) > 5)		// Ϊ�������������ȡֵ��Χ
	{
		g_fcCcs.SetCardSize(m_nWidth);
		g_pView->m_bWndRestored = false;		// ����AdjustFrameToFixedSize()�Ὣ��������Ϊȱʡ��С
		g_pView->AdjustFrameToFixedSize();
	}
	
	EndDialog(wID);
	return 0;
}
