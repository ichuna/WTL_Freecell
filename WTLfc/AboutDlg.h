// aboutdlg.h : interface of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "CEditImpl.h"

class CAboutDlg : public CDialogImpl<CAboutDlg>
{
public:
	enum { IDD = IDD_ABOUTBOX };

	CBrush      m_brushBkgnd;	//����ɫ,
	COLORREF    m_clrText;		//������ɫ
	CHyperLink	m_wndLink_1;		//CHyperLink�ؼ�
	CHyperLink	m_wndLink_2;		//CHyperLink�ؼ�
	CHyperLink	m_wndLink_3;		//CHyperLink�ؼ�
	CEditImpl	m_edit1;

	CAboutDlg(COLORREF clrText = RGB(218, 218, 218));

	BEGIN_MSG_MAP(CAboutDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)

		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		//MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorDlg)
		//MESSAGE_HANDLER(WM_CTLCOLORDLG, OnCtlColorDlg)
		//MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorDlg)
		//MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorDlg)
		//MESSAGE_HANDLER(WM_CTLCOLORMSGBOX, OnCtlColorDlg)
		//MESSAGE_HANDLER(WM_CTLCOLORLISTBOX, OnCtlColorDlg)
		MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorDlg)		//�������þ䣬���ܸı�༭����ɫ
		MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorDlg)	//�������þ䣬���ܸı侲̬�ı�����ɫ
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	LRESULT OnCtlColorDlg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};
