#pragma once

#include "stdafx.h"

// DlgColorDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgScore dialog

//WTL CFileDialog �ļ�ѡ��Ի����������
//��ǩ�� nullclass
//2008-11-30 10:54 2609���Ķ� ����(1) �ղ� �ٱ�
//��Ȩ����������Ϊ����ԭ�����£�δ������������ת�ء�
//
//CFileDialog ������� CFileDialogImpl ������ʾ�ļ��򿪺��ļ�����Ի���
//ͨ������WTLԴ���뷢��CFileDialog �Ĺ������ĺܲ������ڵ����ļ��򿪺��ļ�����Ի����ʱ��û�о��У�
//Ϊ��������Ҫʵ�־��У��������Լ�����һ��������ʵ�֣�

//class CColorDialog : public CColorDialogImpl<CColorDialog>
//{
//public:
//	CColorDialog(COLORREF clrInit = 0, DWORD dwFlags = 0, HWND hWndParent = NULL)
//		: CColorDialogImpl<CColorDialog>(clrInit, dwFlags, hWndParent)
//	{ }
//
//	// override base class map and references to handlers
//	DECLARE_EMPTY_MSG_MAP()
//};

// ͨ�����໯��� WTL CColorDialog ��ɫѡ��Ի����������
class CDlgColorDialog : public CColorDialogImpl<CDlgColorDialog>
{
public:
	//CDlgColorDialog(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
	//                         LPCTSTR lpszDefExt = NULL,
	//                         LPCTSTR lpszFileName = NULL,
	//                         DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
	//                         LPCTSTR lpszFilter = NULL,
	//                         HWND hWndParent = NULL)
	//                         : CFileDialogImpl<CDlgColorDialog>(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, hWndParent){ }

	CDlgColorDialog(COLORREF clrInit = 0, DWORD dwFlags = 0, HWND hWndParent = NULL)
		: CColorDialogImpl<CDlgColorDialog>(clrInit, dwFlags, hWndParent), m_bMsgHandled(FALSE)
	{ }

	// Maps
	BEGIN_MSG_MAP(CDlgColorDialog)
		//CHAIN_MSG_MAP(CFileDialogImpl<CDlgColorDialog>)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		CHAIN_MSG_MAP(CColorDialogImpl<CDlgColorDialog>)
	END_MSG_MAP()

	// override base class map and references to handlers
	//DECLARE_EMPTY_MSG_MAP()

	//���ش˺�����ʵ�־���
	//void OnInitDone( LPOFNOTIFY lpon )
	//{
	//	//GetFileDialogWindow().CenterWindow(lpon->lpOFN->hwndOwner);

	//}

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		//CDialog::OnInitDialog();
		CenterWindow(GetParent());

		bHandled = false;	// �û����������
		return false;
	}
};
