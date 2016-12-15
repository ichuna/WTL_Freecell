// WTLfc.cpp : main source file for WTLfc.exe
//

#include "stdafx.h"

#include "resource.h"

#include "WTLfcView.h"
#include "aboutdlg.h"
#include "MainFrm.h"

CAppModule _Module;
CCardSize g_fcCcs;
CWTLfcData g_fcData;
CWTLfcView* g_pView;
CMainFrame* g_pMain;
bool g_bMaxOnly = false;

int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	CMainFrame wndMain;

	g_pMain = &wndMain;

	if(wndMain.CreateEx() == NULL)
	{
		ATLTRACE(_T("Main window creation failed!\n"));
		return 0;
	}

	wndMain.CenterWindow();

	//if (1)
	if (GetSystemMetrics(SM_CXSCREEN) < 1213 || GetSystemMetrics(SM_CYSCREEN) < 850)
	{
		// ����Ļ�ߴ�С��ȱʡ���ڴ�С������ô��ڻ�ԭ�˵� 
		// ͨ��WTL��BOOL CMainFrame::OnIdle()��UIEnableֻ�ܴ��������Ͷ����˵�������ý��ã����ܴ������˵�
		// ������ͨ��WTL��BOOL CMainFrame::OnIdle()��ʹ��EnableMenuItem����
		g_bMaxOnly = true;
		wndMain.ShowWindow(SW_SHOWMAXIMIZED);

		// EnableMenuItem������������Ч��ֻ�ܷ���BOOL CMainFrame::OnIdle()�в���Ч
		//::EnableMenuItem(g_pMain->GetMenu(), ID_WND_RESTORE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		//::EnableMenuItem(g_pMain->GetMenu(), ID_WND_RESTORE_TOP, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		// ��VS2013�У���ʹ��CMainFrame::CMainFrame()�м���m_bAutoMenuEnable = FALSE;һ�䣬���ò˵�Ҳ���С�VC6�п���
		//g_pMain->GetMenu()->GetSubMenu(0)->EnableMenuItem(ID_WND_RESTORE, MF_BYCOMMAND | MF_GRAYED | MF_DISABLED);
		// ��VS2013�У�ʹ��CMainFrame::OnUpdateWndRestore��ʹ��pCmdUI->Enable(FALSE);�ܹ����ò˵���
	}
	else
	{
		g_bMaxOnly = false;
		wndMain.ShowWindow(nCmdShow);
		g_pView->m_bWndRestored = false;
		wndMain.OnWndRestore(0, 0, 0);	//����������͵����˵������ڻ�ԭ�������䶯
	}

	int nRet = theLoop.Run();

	_Module.RemoveMessageLoop();
	return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	HRESULT hRes = ::CoInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_BAR_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	int nRet = Run(lpstrCmdLine, nCmdShow);

	_Module.Term();
	::CoUninitialize();

	return nRet;
}
