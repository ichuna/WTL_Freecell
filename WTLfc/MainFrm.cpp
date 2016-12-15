// MainFrm.cpp : implmentation of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"
#include "WTLfcView.h"
#include "MainFrm.h"
#include "WndSizeDlg.h"

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	if(CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg))
		return TRUE;

	return m_view.PreTranslateMessage(pMsg);
}

BOOL CMainFrame::OnIdle()
{
	if (g_bMaxOnly)	// ����Ļ�ߴ�С��ȱʡ���ڴ�С������ô��ڻ�ԭ�˵������ַ�ʽ�����Ч
	{
		::EnableMenuItem(g_pMain->GetMenu(), ID_WND_RESTORE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		::EnableMenuItem(g_pMain->GetMenu(), ID_WND_RESTORE_TOP, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		::EnableMenuItem(g_pMain->GetMenu(), ID_WND_SIZE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		::EnableMenuItem(g_pMain->GetMenu(), ID_WND_SIZE_TOP, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	}

	return FALSE;
}

LRESULT CMainFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{

	m_hWndClient = m_view.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	//CenterWindow();

	return 0;
}

LRESULT CMainFrame::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	bHandled = FALSE;
	return 1;
}

LRESULT CMainFrame::OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	PostMessage(WM_CLOSE);
	return 0;
}

LRESULT CMainFrame::OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: add code to initialize document

	return 0;
}

LRESULT CMainFrame::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

void CMainFrame::OnWndRestore(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	//VOID CCardSize::SetCardSize(int nWinWidth = 1190);

	//CRect wr;
	//GetWindowRect(wr);
	//if (abs(wr.Width() - g_fcCcs.m_nWinWidth) > 10)
	//{
	//	g_fcCcs.SetCardSize();
	//}

	//g_pView->AdjustFrameToFixedSize();

	if (g_bMaxOnly) return;		// ����Ļ�ߴ�С��ȱʡ���ڴ�С���㲻������

	// false == m_bWndRestored		��ǰ���ڴ��ڷ�ȱʡ��С���˵���ʾ�����ڻ�ԭ��
	// true == m_bWndRestored		��ǰ���ڴ���ȱʡ��С���˵���ʾ��������󻯡�
	if (false == g_pView->m_bWndRestored)
	{
		g_pView->m_bWndRestored = true;
		//g_pMain->GetMenu()->ModifyMenu(ID_WND_RESTORE, MF_BYCOMMAND, ID_WND_RESTORE, _T("�������(F8)"));
		//g_pMain->DrawMenuBar();
		g_fcCcs.SetCardSize();
		g_pView->AdjustFrameToFixedSize();
	}
	else
	{
		g_pView->m_bWndRestored = false;
		//g_pMain->GetMenu()->ModifyMenu(ID_WND_RESTORE, MF_BYCOMMAND, ID_WND_RESTORE, _T("���ڻ�ԭ(F8)"));
		//g_pMain->DrawMenuBar();

		g_pMain->ShowWindow(SW_SHOWMAXIMIZED);
	}
}

void CMainFrame::OnWndSize(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	//VOID CCardSize::SetCardSize(int nWinWidth = 1190);
	
	CWndSizeDlg dlg;
	dlg.DoModal();

}

//LRESULT CMainFrame::OnSize(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
//{
//	// TODO: Add your message handler code here and/or call default
//
//	static bool bIsZoomed = false;
//	bHandled = FALSE;
//
//	if (bIsZoomed)
//	{
//		g_pView->AdjustFrameToFixedSize();
//		bIsZoomed = !bIsZoomed;
//	}
//
//	if (wParam == SIZE_MAXIMIZED)	//����¼�Ϊ���
//	{
//		bIsZoomed = !bIsZoomed;
//	}
//
//	return 0;
//}


LRESULT CMainFrame::OnMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// TODO: Add your message handler code here and/or call default

	CRect wr, cr;
	g_pMain->GetWindowRect(wr);
	g_pMain->GetClientRect(cr);
	int ww = wr.Width();
	int cw = cr.Width();

	//bool b = g_pMain->IsZoomed();
	//�����ǰΪ��󻯡�������void CJLView::OnWndRestore()����
	if (g_pView->m_nFirst && wr != g_pView->m_rectOrg)	// g_pView->m_nFirst���ų���ʼ���ĵ�һ��
	{
		g_pView->m_bWndRestored = false;
		//g_pMain->GetMenu()->ModifyMenu(ID_WND_RESTORE, MF_BYCOMMAND, ID_WND_RESTORE, _T("���ڻ�ԭ(F8)"));
		ModifyMenuW(g_pMain->GetMenu(), ID_WND_RESTORE, MF_BYCOMMAND, ID_WND_RESTORE, _T("���ڻ�ԭ(F8)"));
		ModifyMenuW(g_pMain->GetMenu(), ID_WND_RESTORE_TOP, MF_BYCOMMAND, ID_WND_RESTORE_TOP, _T("���ڻ�ԭ(F8)"));
		g_pMain->DrawMenuBar();
	}
	else
	{
		g_pView->m_bWndRestored = true;
		//g_pMain->GetMenu()->ModifyMenu(ID_WND_RESTORE, MF_BYCOMMAND, ID_WND_RESTORE, _T("�������(F8)"));
		ModifyMenuW(g_pMain->GetMenu(), ID_WND_RESTORE, MF_BYCOMMAND, ID_WND_RESTORE, _T("�������(F8)"));
		ModifyMenuW(g_pMain->GetMenu(), ID_WND_RESTORE_TOP, MF_BYCOMMAND, ID_WND_RESTORE_TOP, _T("�������(F8)"));
		g_pMain->DrawMenuBar();
	}

	return 0;
}
