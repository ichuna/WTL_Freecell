// WTLfcView.cpp : implementation of the CWTLfcView class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "WTLfcView.h"
#include "MainFrm.h"
#include "DlgAICal.h"
#include "DlgSelGame.h"

BOOL CWTLfcView::PreTranslateMessage(MSG* pMsg)
{
	pMsg;
	return FALSE;
}

CWTLfcView::CWTLfcView()
{
	g_pView = this;
	//m_bDisableOnSize = false;

	//HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	//SetIcon(hIcon, TRUE);
	m_hDown = AtlLoadCursor(IDCSR_DOWN_ARROW);
	m_hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	m_brushBk.CreateSolidBrush(RGB(255, 255, 255));	//card bk
	m_brushBkgnd.CreateSolidBrush(RGB(0, 128, 0));	//client bk

	//װ��λͼ��Դ
	//HANDLE bmp = ::LoadImage(pApp->m_hInstance, name, IMAGE_BITMAP, 596, 86, LR_LOADFROMFILE);
	//HANDLE bmp_Fixed = ::LoadImage(pApp->m_hInstance, name, IMAGE_BITMAP, 596, 86, LR_LOADFROMFILE);
	//m_AllBmps.Attach(bmp);
	//m_AllBmps_Fixed.Attach(bmp_Fixed);
	m_AllBmps.LoadBitmapW(IDB_JL_SUB);
	m_AllBmps_Fixed.LoadBitmapW(IDB_JL_SUB);

	// ��һ�ַ�ʽ������ȫ�ֺ�������λͼ
	//m_AllBmps_Fixed = LoadBitmap(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDB_JL_SUB));

	m_nCardLabelHit = 0;
	m_nFirst = 0;
	m_bWndRestored = true;
}

//CJLView::CJLView()
//{
//	// TODO: add construction code here
//	CWinApp *pApp = AfxGetApp();
//	m_hDown = pApp->LoadCursor(IDCSR_DOWN_ARROW);
//	m_hIcon = pApp->LoadIcon(IDR_MAINFRAME);
//	m_brushBk.CreateSolidBrush(RGB(255, 255, 255));//card bk
//	m_brushBkgnd.CreateSolidBrush(RGB(0, 128, 0));//client bk
//	//װ��λͼ��Դ
//	{
//		//ѹ�����λͼ�ļ�����Ϊ�Զ�����Դ��������װ����Դ
//		HRSRC handle = FindResource(NULL, TEXT("IDR_BIN_ZIP_BMP"), TEXT("BIN"));
//		HGLOBAL hGlobal = LoadResource(NULL, handle);
//		BYTE * pByte = (BYTE *)LockResource(hGlobal);
//		DWORD size = SizeofResource(NULL, handle);
//
//		CMemFile srcFile;
//		srcFile.Attach(pByte, size, 0);
//
//		//׼����ѹ����λͼ���뵽��ʱ�ļ�
//		wchar_t path[MAX_PATH], name[MAX_PATH];
//		DWORD len = GetTempPath(MAX_PATH, path);
//		if (len == 0 || len >= MAX_PATH || GetTempFileName(path, TEXT("bmp"), 0, name) == 0) {
//			MessageBox(TEXT("�޷�������ʱ�ļ�!"), NULL, MB_ICONERROR | MB_OK);
//		}
//
//		CFile desFile(name, modeCrWr);
//
//		//����
//		Decoding(desFile, srcFile);
//		srcFile.Detach();
//
//		//װ��λͼ��Դ֮ǰҪ��֤λͼ�ļ��Ѿ��ر�
//		desFile.Flush();//���������Ǳز����ٵĲ��裡
//		desFile.Close();
//		HANDLE bmp = ::LoadImage(pApp->m_hInstance, name, IMAGE_BITMAP, 596, 86, LR_LOADFROMFILE);
//		HANDLE bmp_Fixed = ::LoadImage(pApp->m_hInstance, name, IMAGE_BITMAP, 596, 86, LR_LOADFROMFILE);
//		m_AllBmps.Attach(bmp);
//		m_AllBmps_Fixed.Attach(bmp_Fixed);
//
//		//ɾ����ʱλͼ�ļ�
//		CFile::Remove(name);
//		m_nFirst = 0;
//	}
//
//	m_nCardLabelHit = 0;
//}


//�������ڱ���
LRESULT CWTLfcView::OnEraseBkgnd(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
	// TODO: Add your message handler code here and/or call default

	CRect r;
	GetClientRect(r);
	//pDC->FillRect(r, &m_brushBkgnd);

	CDCHandle dc((HDC)wParam);
	dc.FillRect(r, m_brushBkgnd);

	bHandled = TRUE;

	return TRUE;
}

LRESULT CWTLfcView::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CPaintDC dc(m_hWnd);

	//TODO: Add your drawing code here

	////// ��ӵĴ��� ���ڻ�ԭ ���ܵ���ش���
	//static int bFisrt = 0;

	//if (bFisrt < 1 && m_bWndRestored)
	if (1 == m_nFirst && m_bWndRestored)	// �Ż����롣ʡ�Ա���bFisrt
	{
		//bFisrt++;
		m_nFirst++;
		g_pMain->GetWindowRect(m_rectOrg);
		CRect vcr; GetClientRect(vcr);
		g_fcCcs.m_nWinWidth = vcr.Width();
	}

	CRect wr, cr;
	g_pMain->GetWindowRect(wr);
	g_pMain->GetClientRect(cr);
	int ww = wr.Width();
	int cw = cr.Width();

	//g_fcCcs.SetCardSize(wr.Width());	// �� OnSize��Ϣ��Ӧ�����У�g_pView->GetClientRect(wr)��ֵ��׼ȷ
	g_fcCcs.SetCardSize(cr.Width());		// �ŵ����g_pView->GetClientRect(wr)��ֵ��׼ȷ��


	//bool b = g_pMain->IsZoomed();
	//�����ǰΪ��󻯡�������void CJLView::OnWndRestore()����
	if (g_pMain->IsZoomed() || wr != m_rectOrg)
		//abs(wr.Width() - g_rectOrg.Width()) > 5 || abs(wr.Height() - g_rectOrg.Height()) > 5)
	{
		m_bWndRestored = false;
		//g_pMain->GetMenu()->ModifyMenu(ID_WND_RESTORE, MF_BYCOMMAND, ID_WND_RESTORE, _T("���ڻ�ԭ(F8)"));
		ModifyMenuW(g_pMain->GetMenu(), ID_WND_RESTORE, MF_BYCOMMAND, ID_WND_RESTORE, _T("���ڻ�ԭ(F8)"));
		ModifyMenuW(g_pMain->GetMenu(), ID_WND_RESTORE_TOP, MF_BYCOMMAND, ID_WND_RESTORE_TOP, _T("���ڻ�ԭ(F8)"));
		g_pMain->DrawMenuBar();
	}
	else
	{
		m_bWndRestored = true;
		//g_pMain->GetMenu()->ModifyMenu(ID_WND_RESTORE, MF_BYCOMMAND, ID_WND_RESTORE, _T("�������(F8)"));
		ModifyMenuW(g_pMain->GetMenu(), ID_WND_RESTORE, MF_BYCOMMAND, ID_WND_RESTORE, _T("�������(F8)"));
		ModifyMenuW(g_pMain->GetMenu(), ID_WND_RESTORE_TOP, MF_BYCOMMAND, ID_WND_RESTORE_TOP, _T("�������(F8)"));
		g_pMain->DrawMenuBar();
	}
	////// ��ӵĴ��� ���ڻ�ԭ ���ܵ���ش���

	//BITMAP bmp;
	//::GetObject(m_AllBmps_Fixed, sizeof(BITMAP), &bmp);
	//SIZE size = { bmp.bmWidth, bmp.bmHeight };

	//CDC dcMem;
	//dcMem.CreateCompatibleDC(dc);
	//HBITMAP hBmpOld = dcMem.SelectBitmap(m_AllBmps_Fixed);
	//dc.BitBlt(0, 0, size.cx, size.cy, dcMem, 0, 0, SRCCOPY);
	//dcMem.SelectBitmap(hBmpOld);

	// 1����ʾ������Ϣ
	CString strSteps;
	CRect r = g_fcData.RectOfStep();
	// ...
	if (!g_fcData.m_pOps->empty()) {
		strSteps.Format(TEXT("%d"), g_fcData.m_pOps->size());

		CFont font;
		//font.CreatePointFont(ccs.stepFont * 10, "Arial", pDC);
		font.CreatePointFont(g_fcCcs.stepFont * 6, TEXT("Arial"), dc);
		HFONT prevFont = dc.SelectFont(font);
		int prevMode = dc.SetBkMode(TRANSPARENT);

		dc.SetTextColor(RGB(0, 255, 0));
		r.left -= 3;
		//	int DrawText(LPTSTR lpstrText, int cchText, LPRECT lpRect, UINT uFormat)
		dc.DrawText(strSteps, strSteps.GetLength(), r, DT_CENTER);

		dc.SetBkMode(prevMode);
		dc.SelectFont(prevFont);
	}

	// 2���ڲ�����Ϣ������ʾͼ��
	CPoint p = r.CenterPoint();
	//p.x -= 16;
	p.x -= 23;
	//p.y += 16;
	//p.y += 30;
	p.y += int(30 * g_fcCcs.dRate);
	dc.DrawIcon(p, m_hIcon);

	// 3������������ʾ��ť(8*12)   A-K
	CDC dcCard;
	dcCard.CreateCompatibleDC(dc);
	//CBitmap *poldbmp = memDC.SelectObject(&m_AllBmps);
	HBITMAP hBmpOld = dcCard.SelectBitmap(m_AllBmps_Fixed);

	//CBrush brush(RGB(0,0,0));
	//CBrush brush = CreateSolidBrush(RGB(0, 255, 0));
	CBrush brush;
	brush.CreateSolidBrush(RGB(0, 255, 0));

	for (UINT c = 1; c <= 13; c++) {
		CRect r = g_fcData.RectOf(c);
		dc.FrameRect(r, brush);
		CPoint p = r.CenterPoint();
		//p.x -= 5; p.y -= 5;
		p.x -= int(2.2 * g_fcCcs.dRate); p.y -= int(4 * g_fcCcs.dRate);
		//pDC->BitBlt(p.x,p.y,8,12,&memDC,(c-1)*8+32,0,SRCAND);
		//pDC->BitBlt(p.x,p.y,8,12,&memDC,(c-1)*8+32,0,NOTSRCCOPY | SRCAND);
		dc.StretchBlt(p.x, p.y, int(5 * g_fcCcs.dRate), int(8 * g_fcCcs.dRate), dcCard, (c - 1) * 8 + 32, 0, 8, 12, NOTSRCCOPY | SRCAND);
	}
	//memDC.SelectObject(poldbmp);
	dcCard.SelectBitmap(hBmpOld);

	// 4�������ƾ�
	p.x = p.y = 0;
	for (UINT i = 1; i <= 16; i++)
	{
		UINT nCards = g_fcData.CntCardsIn(i);
		if (!nCards && i > 8)	//����û������ֻ���Ʒ���ֻ���ƿյ��кͻ����з���
		{
			//HPEN penBlack = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
			CPen penBlack;
			penBlack.CreatePen(PS_SOLID, 1, RGB(0, 255, 0));

			HPEN penOld = dc.SelectPen(penBlack);

			//pDC->FrameRect(pDoc->RectOf(i,1,1),&brush);
			//CBrush *pPrevBrush = pDC->SelectObject(&m_brushBkgnd);
			CBrush brushOld = dc.SelectBrush(m_brushBkgnd);
			//pDC->RoundRect(pDoc->RectOf(i,1,1),CPoint(0,0));
			dc.Rectangle(g_fcData.RectOf(i, 1, 1));
			dc.SelectBrush(brushOld);
			//pDC->SelectObject(&brush);

			//HPEN penBlack2 = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
			CPen penBlack2;
			penBlack2.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

			dc.SelectPen(penBlack2);

			CPoint p;
			CPoint pTopLeft = g_fcData.RectOf(i, 1, 1).TopLeft();
			CPoint BottomRight = g_fcData.RectOf(i, 1, 1).BottomRight();
			p.x = pTopLeft.x;
			p.y = BottomRight.y;

			dc.MoveTo(p);
			dc.LineTo(pTopLeft);

			p.x = BottomRight.x;
			p.y = pTopLeft.y;

			dc.LineTo(p);

			dc.SelectPen(penOld);
		}
		else if (i <= 8)
		{
			for (UINT j = 1; j <= nCards; j++)
			{
				UINT card = g_fcData.GetCard(i, j);
				if (card == 0) continue;		//���ﱾ���ó����Ƶ���Ϊ 0 ����������Ƕ��߳��ǵĻ�
				r = g_fcData.RectOf(i, j, 1);
				DrawCard(r.TopLeft(), card, &dc);
				if (g_fcData.m_nSel == i && j == nCards)		 //��ѡ���еĵ�����Ҫ��ɫ
				{
					dc.InvertRect(r);
				}
			}
		}
		else		//���ƿյ��кͻ�����
		{
			r = g_fcData.RectOf(i, 1, 1);
			DrawCard(r.TopLeft(), g_fcData.BottCard(i), &dc);
			if (g_fcData.m_nSel == i)	 //��������Ǳ�ѡ��״̬��������Ʊ���ɫ
			{
				dc.InvertRect(r);
			}
		}
	}

	return 0;
}

//void CJLView::OnDraw(CDC* pDC)
//{
//	CJLDoc* pDoc = GetDocument();
//	ASSERT_VALID(pDoc);
//
//	//��ʾ������Ϣ
//	CString strSteps;
//	CRect r = pDoc->RectOfStep();
//	if (!pDoc->m_pOps->IsEmpty()) {
//		strSteps.Format(TEXT("%d"), pDoc->m_pOps->GetCount());
//
//		CFont font;
//		//font.CreatePointFont(ccs.stepFont * 10, "Arial", pDC);
//		font.CreatePointFont(ccs.stepFont * 6, TEXT("Arial"), pDC);
//		CFont * prevFont = pDC->SelectObject(&font);
//		int     prevMode = pDC->SetBkMode(TRANSPARENT);
//
//		pDC->SetTextColor(RGB(0, 255, 0));
//		r.left -= 3;
//		pDC->DrawText(strSteps, r, DT_CENTER);
//
//		pDC->SetBkMode(prevMode);
//		pDC->SelectObject(prevFont);
//	}
//	//�ڲ�����Ϣ������ʾͼ��
//	CPoint p = r.CenterPoint();
//	//p.x -= 16;
//	p.x -= 23;
//	//p.y += 16;
//	//p.y += 30;
//	p.y += int(30 * ccs.dRate);
//	pDC->DrawIcon(p, m_hIcon);
//
//	//����������ʾ��ť(8*12)   A-K
//	CDC memDC;
//	memDC.CreateCompatibleDC(pDC);
//	//CBitmap *poldbmp = memDC.SelectObject(&m_AllBmps);
//	CBitmap *poldbmp = memDC.SelectObject(&m_AllBmps_Fixed);
//	//CBrush brush(RGB(0,0,0));
//	CBrush brush(RGB(0, 255, 0));
//	for (UINT c = 1; c <= 13; c++) {
//		CRect r = pDoc->RectOf(c);
//		pDC->FrameRect(r, &brush);
//		CPoint p = r.CenterPoint();
//		//p.x -= 5; p.y -= 5;
//		p.x -= int(2.2 * ccs.dRate); p.y -= int(4 * ccs.dRate);
//		//pDC->BitBlt(p.x,p.y,8,12,&memDC,(c-1)*8+32,0,SRCAND);
//		//pDC->BitBlt(p.x,p.y,8,12,&memDC,(c-1)*8+32,0,NOTSRCCOPY | SRCAND);
//		pDC->StretchBlt(p.x, p.y, int(5 * ccs.dRate), int(8 * ccs.dRate), &memDC, (c - 1) * 8 + 32, 0, 8, 12, NOTSRCCOPY | SRCAND);
//	}
//	memDC.SelectObject(poldbmp);
//
//	//�����ƾ�
//	p.x = p.y = 0;
//	for (UINT i = 1; i <= 16; i++) {
//		UINT nCards = pDoc->CntCardsIn(i);
//		if (!nCards && i > 8) { //����û������ֻ���Ʒ���ֻ���ƿյ��кͻ����з���
//			CPen penBlack(PS_SOLID, 1, RGB(0, 255, 0));
//			CPen *oldPen = pDC->SelectObject(&penBlack);
//
//			//pDC->FrameRect(pDoc->RectOf(i,1,1),&brush);
//			CBrush *pPrevBrush = pDC->SelectObject(&m_brushBkgnd);
//			//pDC->RoundRect(pDoc->RectOf(i,1,1),CPoint(0,0));
//			pDC->Rectangle(pDoc->RectOf(i, 1, 1));
//			pDC->SelectObject(&pPrevBrush);
//			//pDC->SelectObject(&brush);
//
//			CPen penBlack2(PS_SOLID, 1, RGB(0, 0, 0));
//			pDC->SelectObject(&penBlack2);
//
//			CPoint p;
//			CPoint pTopLeft = pDoc->RectOf(i, 1, 1).TopLeft();
//			CPoint BottomRight = pDoc->RectOf(i, 1, 1).BottomRight();
//			p.x = pTopLeft.x;
//			p.y = BottomRight.y;
//
//			pDC->MoveTo(p);
//			pDC->LineTo(pTopLeft);
//
//			p.x = BottomRight.x;
//			p.y = pTopLeft.y;
//
//			pDC->LineTo(p);
//
//			pDC->SelectObject(oldPen);
//		}
//		else if (i <= 8) {
//			for (UINT j = 1; j <= nCards; j++) {
//				UINT card = pDoc->GetCard(i, j);
//				if (card == 0) continue; //���ﱾ���ó����Ƶ���Ϊ0 ����������Ƕ��߳��ǵĻ�
//				r = pDoc->RectOf(i, j, 1);
//				DrawCard(r.TopLeft(), card, pDC);
//				if (pDoc->m_nSel == i && j == nCards) {
//					pDC->InvertRect(r); //��ѡ���еĵ�����Ҫ��ɫ
//				}
//			}
//		}
//		else { //���ƿյ��кͻ�����
//			r = pDoc->RectOf(i, 1, 1);
//			DrawCard(r.TopLeft(), pDoc->BottCard(i), pDC);
//			if (pDoc->m_nSel == i) { //��������Ǳ�ѡ��״̬��������Ʊ���ɫ
//				pDC->InvertRect(r);
//			}
//		}
//	}
//}

//�������ڵ����ʵĴ�С
void CWTLfcView::AdjustFrameToFixedSize()
{
	// �����ȱʡ��С������󻯺󴰿ڻ�ԭ������
	// ������������pWnd->MoveWindow(wr);֮ǰ����Ȼ�������⣺
	//   �����ڲ���ȱʡ��Сλ��->���->���ڻ�ԭ���ͻ���ϵͳ�Ļָ����ڴ�С�������ܽ� ���ڻ�ԭ ��ȱʡ��Сλ��
	//if (g_pMain->IsZoomed())
	//	g_pMain->SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);

	//Ҫ�󲻹���ʲôʱ�򴰿�Ҫ��������ʾ���е���
	CRect wr, cr;

	//if (m_bWndRestored && m_rectOrg.Width() > 0)
	if (m_bWndRestored && m_nFirst > 0)		// ����ɳ�ʼ������ȱʡ���ڴ�С�Ѿ�����
	{
		wr = m_rectOrg;
	}
	else
	{
		GetClientRect(cr);
		g_pMain->GetWindowRect(wr);

		// �������������ߵĿ�Ⱥ����±߿�ĸ߶�
		int xEdge = (wr.Width() - cr.Width()) / 2 + 1;
		int yEdge = (wr.Height() - cr.Height()) / 2;

		CRect r = g_fcData.RectOf(8, 1, 1);//�ڰ��е�һ��
		ClientToScreen(r);

		//GetSystemMetrics(SM_CXFRAME)
		//GetSystemMetrics(SM_CYFRAME)
		//wr.left = wr.top = 0;
		wr.right = r.right + g_fcCcs.CARD_INT + g_fcCcs.ptOrg.x + xEdge;
		wr.bottom = r.top + g_fcCcs.CARD_UNCOVER * 11 + g_fcCcs.CARD_HEI + xEdge;

		//pWnd->MoveWindow(wr);
		//pWnd->CenterWindow();
	}

	MyCenterWindow(wr);		// ���ⴰ����˸

	if (0 == m_nFirst)
		m_nFirst = 1;
}

BOOL CWTLfcView::MyCenterWindow(CRect rcDlg)
{
	CRect rcArea;
	CRect rcScreen;

	rcScreen.left = rcScreen.top = 0;
	rcScreen.right = GetSystemMetrics(SM_CXSCREEN);
	rcScreen.bottom = GetSystemMetrics(SM_CYSCREEN);

	::SystemParametersInfo(SPI_GETWORKAREA, NULL, (PVOID)&rcArea, 0);	//��ȡ���湤������С

	int xLeft = (rcScreen.left + rcScreen.right) / 2 - rcDlg.Width() / 2;
	int yTop = (rcScreen.top + rcScreen.bottom) / 2 - rcDlg.Height() / 2;

	// if the dialog is outside the screen, move it inside
	if (xLeft < rcArea.left)		xLeft = rcArea.left;
	if (yTop < rcArea.top)	yTop = rcArea.top;

	CRect rcRet(xLeft, yTop, xLeft + rcDlg.Width(), yTop + rcDlg.Height());

	if (rcRet.Width() > rcArea.Width())
		rcRet.right = rcArea.right;
	if (rcRet.Height() > rcArea.Height())
		rcRet.bottom = rcArea.bottom;

	// �����ȱʡ��С������󻯺󴰿ڻ�ԭ������
	// ������������pWnd->MoveWindow(wr);��SetWindowPos��֮ǰ����Ȼ�������⣺
	//   �����ڲ���ȱʡ��Сλ��->���->���ڻ�ԭ���ͻ���ϵͳ�Ļָ����ڴ�С�������ܽ� ���ڻ�ԭ ��ȱʡ��Сλ��
	if (g_pMain->IsZoomed())
	{
		//g_pMain->SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
		// �������Խ�һ�����ٴ��ڴ�С�ı�ʱ����˸
		long lWnd = ::GetWindowLong(g_pMain->m_hWnd, GWL_STYLE);
		//lWnd = lWnd & ~WS_MAXIMIZEBOX;		//lWnd = lWnd & ~WS_MINIMIZEBOX;
		lWnd = lWnd & ~(WS_MAXIMIZE | WS_MINIMIZE);
		::SetWindowLong(g_pMain->m_hWnd, GWL_STYLE, lWnd);
	}

	return g_pMain->SetWindowPos(NULL, xLeft, yTop,
		rcRet.Width(), rcRet.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
}

//�������ڵ����ʵĴ�С
//void CWTLfcView::AdjustFrameToFixedSize()
//{
//	//CenterWindow(GetParent());		// WTL ��������Ч
//	//GetParent().CenterWindow();		// WTL ��������Ч
//
//	//Ҫ�󲻹���ʲôʱ�򴰿�Ҫ��������ʾ���е���
//	//HWND hwnd = GetParent();
//	CRect wr, cr;
//	//g_pView->GetClientRect(cr);		// �õ�View���ڿͻ�������
//	//g_pView->GetWindowRect(wr);		// �õ�View���ڵľ���
//	//::GetClientRect(m_hWnd, cr);		// �õ�View���ڿͻ������Ρ����ַ�ʽ���������ʱ����������
//	//::GetWindowRect(m_hWnd, wr);		// �õ�View���ڵľ��Ρ����ַ�ʽ���������ʱ����������
//
//	//g_pMain->GetWindowRect(wr);		// �õ�CMainFrame���ڵľ���
//	//g_pMain->GetClientRect(cr);		// �õ�CMainFrame���ڿͻ�������
//
//	// �������������ߵĿ�Ⱥ����±߿�ĸ߶�
//	//int edge = (wr.Width() - cr.Width()) / 2 + 1;
//
//	CRect r = g_fcData.RectOf(8, 1, 1);//�õ��ڰ��е�һ�ŵĿͻ������Ρ��ڰ���Ҳ�����������һ��
//	//ClientToScreen(r);		// ���ַ�ʽ���������ʱ����������
//	//g_pMain->ClientToScreen(r);	// ���ַ�ʽ�Ǵ����
//	//g_pView->ClientToScreen(r);		// ���ַ�ʽ������ȷ��
//
//	// g_fcCcs.CARD_INT ��������֮��ļ����g_fcCcs.ptOrg.x ����������������ͻ����߿�ľ���
//	//wr.right = r.right + g_fcCcs.CARD_INT + g_fcCcs.ptOrg.x + edge;
//	//wr.bottom = r.top + g_fcCcs.CARD_UNCOVER*12 + g_fcCcs.CARD_HEI + edge;
//	//wr.bottom = r.top + g_fcCcs.CARD_UNCOVER * 11 + g_fcCcs.CARD_HEI + edge;
//	wr.top = wr.left = 0;
//	wr.right = r.right + g_fcCcs.CARD_INT + g_fcCcs.ptOrg.x + 2 * (GetSystemMetrics(SM_CXFRAME)) + 1;
//	wr.bottom = r.top + g_fcCcs.CARD_UNCOVER * 13 + g_fcCcs.CARD_HEI + 2 * (GetSystemMetrics(SM_CYFRAME)) + 1;
//
//	//CRect rcArea;
//	//rcArea.left = rcArea.top = 0;
//	//rcArea.right = GetSystemMetrics(SM_CXSCREEN);
//	//rcArea.bottom = GetSystemMetrics(SM_CYSCREEN);
//
//	//int DlgWidth = wr.right - wr.left;
//	//int DlgHeight = wr.bottom - wr.top;
//
//	//// find dialog's upper left based on rcCenter
//	//int xLeft = (rcArea.left + rcArea.right) / 2 - DlgWidth / 2;
//	//int yTop = (rcArea.top + rcArea.bottom) / 2 - DlgHeight / 2;
//
//	//wr.Height();
//	//::MoveWindow(hwnd, wr.left, wr.top, wr.Width(), wr.Height(), TRUE);
//	//::MoveWindow(hwnd, xLeft, yTop, wr.Width(), wr.Height(), TRUE);
//	//GetParent().CenterWindow();		// WTL ��������Ч
//	//GetParent().UpdateWindow();		// WTL ��������Ч
//	MyCenterWindow(wr);
//	//g_pMain->SetWindowPos(NULL, wr.left, wr.top, wr.Width(), wr.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
//	//if (g_pMain->IsZoomed())
//	//	g_pMain->PostMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
//	//g_pMain->CenterWindow();
//
//	//if (0 == m_nFirst)
//	//	m_nFirst = 1;
//}

////�������ڵ����ʵĴ�С
//void CJLView::AdjustFrameToFixedSize()
//{
//	CJLDoc * pDoc = GetDocument();
//
//	//Ҫ�󲻹���ʲôʱ�򴰿�Ҫ��������ʾ���е���
//	CRect wr, cr;
//	GetClientRect(cr);
//	CWnd *pWnd = GetParent();
//	pWnd->GetWindowRect(wr);
//
//	// �������������ߵĿ�Ⱥ����±߿�ĸ߶�
//	int edge = (wr.Width() - cr.Width()) / 2 + 1;
//
//	CRect r = pDoc->RectOf(8, 1, 1);//�ڰ��е�һ��
//	ClientToScreen(r);
//
//	wr.right = r.right + ccs.CARD_INT + ccs.ptOrg.x + edge;
//	//wr.bottom = r.top + ccs.CARD_UNCOVER*12 + ccs.CARD_HEI + edge;
//	wr.bottom = r.top + ccs.CARD_UNCOVER * 11 + ccs.CARD_HEI + edge;
//
//	//wr.Height();
//	pWnd->MoveWindow(wr);
//	pWnd->CenterWindow();
//
//	if (0 == m_nFirst)
//		m_nFirst = 1;
//}


LRESULT CWTLfcView::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// TODO: Add your message handler code here and/or call default

	//���û���ĵ��򿪾��Զ������ƾ�
	//if (g_fcData.GetPathName().IsEmpty())
	{ g_fcData.OnRand(); }

	//AdjustFrameToFixedSize();	//�������ڵ����ʵĴ�С

	//OK
	CClientDC dc(m_hWnd);
	//CClientDC pDC(this);
	//CDC *pDC = GetDC();
	//replaceColor(&m_AllBmps_Fixed, RGB(0,0,0), RGB(0,255,0), &pDC);
	replaceColor(&m_AllBmps_Fixed, RGB(0, 0, 0), RGB(255, 0, 255), &dc);

	return 0;
}


//��ɫ�滻����
//����:���滻��ͼ��ָ��,�滻ǰ��ɫ,�滻�����ɫ,CDC����
//���:�滻���ͼ��ָ��
CBitmap* CWTLfcView::replaceColor(CBitmap *pBmpBefore, COLORREF res, COLORREF aim, CClientDC *dc)
{
	CDC dcMask, dcBefore;
	CBitmap bmpMask;
	BITMAP bitmap;
	pBmpBefore->GetBitmap(&bitmap);
	CRect rect(0, 0, bitmap.bmWidth, bitmap.bmHeight);
	//������������ֲ�ͼ��,�豸,��װ��
	bmpMask.CreateBitmap(bitmap.bmWidth, bitmap.bmHeight, 1, 1, NULL);
	dcMask.CreateCompatibleDC(*dc);
	dcBefore.CreateCompatibleDC(*dc);
	dcMask.SelectBitmap(bmpMask);
	dcBefore.SelectBitmap(pBmpBefore->m_hBitmap);
	//���滻ǰ��ɫ����Ϊ��ɫ
	dcBefore.SetBkColor(res);
	dcMask.BitBlt(0, 0, bitmap.bmWidth, bitmap.bmHeight, dcBefore, 0, 0, SRCCOPY);
	dcBefore.SetBkColor(RGB(0, 0, 0));
	dcBefore.SetTextColor(RGB(255, 255, 255));
	dcBefore.BitBlt(0, 0, bitmap.bmWidth, bitmap.bmHeight, dcMask, 0, 0, SRCAND);
	//���滻ǰ��ɫ�滻ΪĿ����ɫ
	dcBefore.SetBkColor(aim);
	dcBefore.SetTextColor(RGB(0, 0, 0));
	dcBefore.BitBlt(0, 0, bitmap.bmWidth, bitmap.bmHeight, dcMask, 0, 0, SRCPAINT);
	return	pBmpBefore;
}

//��ɫ�滻����
//����:���滻��ͼ��ָ��,�滻ǰ��ɫ,�滻�����ɫ,CDC����
//���:�滻���ͼ��ָ��
//CBitmap* CJLView::replaceColor(CBitmap *b_bmp, COLORREF res, COLORREF aim, CDC *cdc)
//{
//	CDC m_cdc, b_cdc;
//	CBitmap m_bmp;
//	BITMAP bitmap;
//	b_bmp->GetBitmap(&bitmap);
//	CRect rect(0, 0, bitmap.bmWidth, bitmap.bmHeight);
//	//������������ֲ�ͼ��,�豸,��װ��
//	m_bmp.CreateBitmap(bitmap.bmWidth, bitmap.bmHeight, 1, 1, NULL);
//	m_cdc.CreateCompatibleDC(cdc);
//	b_cdc.CreateCompatibleDC(cdc);
//	m_cdc.SelectObject(&m_bmp);
//	b_cdc.SelectObject(b_bmp);
//	//���滻ǰ��ɫ����Ϊ��ɫ
//	b_cdc.SetBkColor(res);
//	m_cdc.BitBlt(0, 0, bitmap.bmWidth, bitmap.bmHeight, &b_cdc, 0, 0, SRCCOPY);
//	b_cdc.SetBkColor(RGB(0, 0, 0));
//	b_cdc.SetTextColor(RGB(255, 255, 255));
//	b_cdc.BitBlt(0, 0, bitmap.bmWidth, bitmap.bmHeight, &m_cdc, 0, 0, SRCAND);
//	//���滻ǰ��ɫ�滻ΪĿ����ɫ
//	b_cdc.SetBkColor(aim);
//	b_cdc.SetTextColor(RGB(0, 0, 0));
//	b_cdc.BitBlt(0, 0, bitmap.bmWidth, bitmap.bmHeight, &m_cdc, 0, 0, SRCPAINT);
//	return	b_bmp;
//}


//draw all things in memory dc!!!!!!!!!!!!!
//���еĻ��ƶ������붼���ڴ��н��з�����������
void CWTLfcView::DrawCard(CPoint point, UINT card, CDC *pDC)
{
	//CJLDoc* pDoc = GetDocument();
	ATLASSERT(g_fcData.IsCard(card));

	//HPEN penBlack = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen penBlack;
	penBlack.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

	HPEN penOld = pDC->SelectPen(penBlack);
	CBrush brushOld = pDC->SelectBrush(m_brushBk);
	CRect r(point, CSize(g_fcCcs.CARD_WID, g_fcCcs.CARD_HEI));
	pDC->RoundRect(r, CPoint(5, 5));//���Ƶı�����߿�
	pDC->SelectPen(penOld);
	pDC->SelectBrush(brushOld);

	CDC memDC;
	memDC.CreateCompatibleDC(*pDC);
	HBITMAP bmpOld = memDC.SelectBitmap(m_AllBmps);
	//�����Ͻ� & ���½�����(8*12)��������/�����ͼ��(8*8)
	int type = TYPE(card), num = NUM(card);
	int idxBS = type % 2 * 26, idxUL = idxBS + num - 1, idxBR = idxBS + 26 - num;
	/*
	pDC->BitBlt(r.left+3    , r.top+3       , 8 , 12 , &memDC , idxUL*8+32 , 0 , SRCAND);
	pDC->BitBlt(r.right-3-8 , r.bottom-3-12 , 8 , 12 , &memDC , idxBR*8+32 , 0 , SRCAND);
	pDC->BitBlt(r.left+3    , r.top+3+12+1,       8, 8, &memDC , ccs.clr[ccs.SML][type][0][0], ccs.clr[ccs.SML][type][0][1] , SRCAND);
	pDC->BitBlt(r.right-3-8 , r.bottom-3-12-1-8 , 8, 8, &memDC , ccs.clr[ccs.SML][type][1][0], ccs.clr[ccs.SML][type][1][1] , SRCAND);
	*/

	//����˼·��ģʽ���ǽ�����StretchBlt����ʱ��ǰ4������ĩβ������ * ccs.dRate��һ��ÿ������ֻ��ĩ���ֳ�1��
	//��������ǿ��תint�����⾯�档warning C4244: 'argument' : conversion from 'float' to 'int', possible loss of data
	int nEdg = int(4.5*g_fcCcs.dRate);
	pDC->StretchBlt(r.left + nEdg, r.top + nEdg, int(8.8*g_fcCcs.dRate), int(13.2*g_fcCcs.dRate), memDC, idxUL * 8 + 32, 0, 8, 12, SRCAND);
	pDC->StretchBlt(int(r.right - nEdg - 8.5*g_fcCcs.dRate), int(r.bottom - nEdg - 13 * g_fcCcs.dRate), int(8.8*g_fcCcs.dRate), int(13.2*g_fcCcs.dRate), memDC, idxBR * 8 + 32, 0, 8, 12, SRCAND);
	//���������������⴦���ǻ����Ͻ� & ���½�ͼ���(8*8)��
	pDC->StretchBlt(int(r.left + nEdg + 1.5*g_fcCcs.dRate), int(r.top + nEdg + 15 * g_fcCcs.dRate + 1 * g_fcCcs.dRate), int(8 * g_fcCcs.dRate), int(8 * g_fcCcs.dRate), memDC, g_fcCcs.clr[g_fcCcs.SML][type][0][0], g_fcCcs.clr[g_fcCcs.SML][type][0][1], 8, 8, SRCAND);
	pDC->StretchBlt(int(r.right - nEdg - 8.5*g_fcCcs.dRate), int(r.bottom - nEdg - 12 * g_fcCcs.dRate - 1 * g_fcCcs.dRate - 10.5*g_fcCcs.dRate), int(8 * g_fcCcs.dRate), int(8 * g_fcCcs.dRate), memDC, g_fcCcs.clr[g_fcCcs.SML][type][1][0], g_fcCcs.clr[g_fcCcs.SML][type][1][1], 8, 8, SRCAND);

	//���Ƶ���Ҫ���ֻ���ɫ
	const UCHAR *p, *data[] = { g_fcCcs.cA, g_fcCcs.c2, g_fcCcs.c3, g_fcCcs.c4, g_fcCcs.c5, g_fcCcs.c6, g_fcCcs.c7, g_fcCcs.c8, g_fcCcs.c9, g_fcCcs.c10 };
	if (num <= 10) {
		p = (card == CARD(8, 3) ? g_fcCcs.c8FK : data[num - 1]);
		UINT n = num * 3;
		for (UINT j = 0; j < n; j += 3) {
			//pDC->BitBlt(point.x + p[j] - 8, point.y + p[j+1] - 8, 16, 16, &memDC, clr[BIG][type][!p[j+2]][0], clr[BIG][type][!p[j+2]][1], SRCAND);
			//ԭ��ʱԭʼ��С������Ƭ���������ţ�������BitBlt������Ϊ8����ע�������õ���MFC��CDC��Ա����BitBlt������ƽ̨SDK API��BitBlt(����Ϊ9�������һ��HDC)
			//������Ҫ�������ţ�������StretchBlt������Ϊ10����ע�������õ���MFC��CDC��Ա����StretchBlt������ƽ̨SDK API��StretchBlt(����Ϊ11�������һ��HDC)
			pDC->StretchBlt(int(point.x + p[j] - 5.5*g_fcCcs.dRate), int(point.y + p[j + 1] - 7 * g_fcCcs.dRate), int(12 * g_fcCcs.dRate), int(16 * g_fcCcs.dRate), memDC,
				g_fcCcs.clr[g_fcCcs.BIG][type][!p[j + 2]][0], g_fcCcs.clr[g_fcCcs.BIG][type][!p[j + 2]][1], 16, 16, SRCAND);

			//pDC->StretchBlt(int(point.x + p[j] - 12*g_fcCcs.fRate), int(point.y + p[j+1] - 16*g_fcCcs.fRate), int(26*g_fcCcs.fRate), int(32*g_fcCcs.fRate), &memDC, 
			//	ccs.clr[ccs.BIG][type][!p[j+2]][0], ccs.clr[ccs.BIG][type][!p[j+2]][1], 16, 16, SRCAND);


		}
	}
	else { //��J Q K
		CPoint p = r.CenterPoint();
		//pDC->BitBlt(p.x-23, p.y-37, 47, 74, &memDC, ( (num-11)*4 + type )*47 + 32, 12, SRCAND);
		pDC->StretchBlt(int(p.x - 20 * g_fcCcs.dRate), int(p.y - 33 * g_fcCcs.dRate), int(41 * g_fcCcs.dRate), int(67 * g_fcCcs.dRate), memDC, ((num - 11) * 4 + type) * 47 + 32, 12, 47, 74, SRCAND);

	}
	memDC.SelectBitmap(bmpOld);
}

//draw all things in memory dc!!!!!!!!!!!!!
//���еĻ��ƶ������붼���ڴ��н��з�����������
//void CJLView::DrawCard(CPoint point, UINT card, CDC *pDC)
//{
//	CJLDoc* pDoc = GetDocument();
//	ASSERT(pDoc->IsCard(card));
//
//	CPen penBlack(PS_SOLID, 1, RGB(0, 0, 0));
//	CPen *oldPen = pDC->SelectObject(&penBlack);
//	CBrush *oldBrush = pDC->SelectObject(&m_brushBk);
//	CRect r(point, CSize(ccs.CARD_WID, ccs.CARD_HEI));
//	pDC->RoundRect(r, CPoint(5, 5));//���Ƶı�����߿�
//	pDC->SelectObject(oldPen);
//	pDC->SelectObject(oldBrush);
//	CDC memDC;
//	memDC.CreateCompatibleDC(pDC);
//	CBitmap *poldbmp = memDC.SelectObject(&m_AllBmps);
//	//�����Ͻ� & ���½�����(8*12)��������/�����ͼ��(8*8)
//	int type = TYPE(card), num = NUM(card);
//	int idxBS = type % 2 * 26, idxUL = idxBS + num - 1, idxBR = idxBS + 26 - num;
//	/*
//	pDC->BitBlt(r.left+3    , r.top+3       , 8 , 12 , &memDC , idxUL*8+32 , 0 , SRCAND);
//	pDC->BitBlt(r.right-3-8 , r.bottom-3-12 , 8 , 12 , &memDC , idxBR*8+32 , 0 , SRCAND);
//	pDC->BitBlt(r.left+3    , r.top+3+12+1,       8, 8, &memDC , ccs.clr[ccs.SML][type][0][0], ccs.clr[ccs.SML][type][0][1] , SRCAND);
//	pDC->BitBlt(r.right-3-8 , r.bottom-3-12-1-8 , 8, 8, &memDC , ccs.clr[ccs.SML][type][1][0], ccs.clr[ccs.SML][type][1][1] , SRCAND);
//	*/
//
//	//����˼·��ģʽ���ǽ�����StretchBlt����ʱ��ǰ4������ĩβ������ * ccs.dRate��һ��ÿ������ֻ��ĩ���ֳ�1��
//	//��������ǿ��תint�����⾯�档warning C4244: 'argument' : conversion from 'float' to 'int', possible loss of data
//	int nEdg = int(4.5*ccs.dRate);
//	pDC->StretchBlt(r.left + nEdg, r.top + nEdg, int(8.8*ccs.dRate), int(13.2*ccs.dRate), &memDC, idxUL * 8 + 32, 0, 8, 12, SRCAND);
//	pDC->StretchBlt(int(r.right - nEdg - 8.5*ccs.dRate), int(r.bottom - nEdg - 13 * ccs.dRate), int(8.8*ccs.dRate), int(13.2*ccs.dRate), &memDC, idxBR * 8 + 32, 0, 8, 12, SRCAND);
//	//���������������⴦���ǻ����Ͻ� & ���½�ͼ���(8*8)��
//	pDC->StretchBlt(int(r.left + nEdg + 1.5*ccs.dRate), int(r.top + nEdg + 15 * ccs.dRate + 1 * ccs.dRate), int(8 * ccs.dRate), int(8 * ccs.dRate), &memDC, ccs.clr[ccs.SML][type][0][0], ccs.clr[ccs.SML][type][0][1], 8, 8, SRCAND);
//	pDC->StretchBlt(int(r.right - nEdg - 8.5*ccs.dRate), int(r.bottom - nEdg - 12 * ccs.dRate - 1 * ccs.dRate - 10.5*ccs.dRate), int(8 * ccs.dRate), int(8 * ccs.dRate), &memDC, ccs.clr[ccs.SML][type][1][0], ccs.clr[ccs.SML][type][1][1], 8, 8, SRCAND);
//
//	//���Ƶ���Ҫ���ֻ���ɫ
//	const UCHAR *p, *data[] = { ccs.cA, ccs.c2, ccs.c3, ccs.c4, ccs.c5, ccs.c6, ccs.c7, ccs.c8, ccs.c9, ccs.c10 };
//	if (num <= 10) {
//		p = (card == CARD(8, 3) ? ccs.c8FK : data[num - 1]);
//		UINT n = num * 3;
//		for (UINT j = 0; j < n; j += 3) {
//			//pDC->BitBlt(point.x + p[j] - 8, point.y + p[j+1] - 8, 16, 16, &memDC, clr[BIG][type][!p[j+2]][0], clr[BIG][type][!p[j+2]][1], SRCAND);
//			//ԭ��ʱԭʼ��С������Ƭ���������ţ�������BitBlt������Ϊ8����ע�������õ���MFC��CDC��Ա����BitBlt������ƽ̨SDK API��BitBlt(����Ϊ9�������һ��HDC)
//			//������Ҫ�������ţ�������StretchBlt������Ϊ10����ע�������õ���MFC��CDC��Ա����StretchBlt������ƽ̨SDK API��StretchBlt(����Ϊ11�������һ��HDC)
//			pDC->StretchBlt(int(point.x + p[j] - 5.5*ccs.dRate), int(point.y + p[j + 1] - 7 * ccs.dRate), int(12 * ccs.dRate), int(16 * ccs.dRate), &memDC,
//				ccs.clr[ccs.BIG][type][!p[j + 2]][0], ccs.clr[ccs.BIG][type][!p[j + 2]][1], 16, 16, SRCAND);
//
//			//pDC->StretchBlt(int(point.x + p[j] - 12*ccs.fRate), int(point.y + p[j+1] - 16*ccs.fRate), int(26*ccs.fRate), int(32*ccs.fRate), &memDC, 
//			//	ccs.clr[ccs.BIG][type][!p[j+2]][0], ccs.clr[ccs.BIG][type][!p[j+2]][1], 16, 16, SRCAND);
//
//
//		}
//	}
//	else { //��J Q K
//		CPoint p = r.CenterPoint();
//		//pDC->BitBlt(p.x-23, p.y-37, 47, 74, &memDC, ( (num-11)*4 + type )*47 + 32, 12, SRCAND);
//		pDC->StretchBlt(int(p.x - 20 * ccs.dRate), int(p.y - 33 * ccs.dRate), int(41 * ccs.dRate), int(67 * ccs.dRate), &memDC, ((num - 11) * 4 + type) * 47 + 32, 12, 47, 74, SRCAND);
//
//	}
//	memDC.SelectObject(poldbmp);
//}


//LRESULT CWTLfcView::OnSize(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
//{
//	// TODO: Add your message handler code here and/or call default
//
//	bHandled = FALSE;
//	//if (0 == m_nFirst)
//	//	return 0;
//
//	////if (m_bDisableOnSize)
//	////	return 0;
//
//	//CRect wr;
//	////GetClientRect(wr);	// �õ�View���ڿͻ�������
//	////GetWindowRect(wr);	// �õ�View���ھ���
//
//	//g_pView->GetClientRect(wr);		// �õ�View���ڿͻ������Ρ����������ڱ������ڵ�ʱ������ִ�λ����
//	////g_pMain->GetWindowRect(wr);		// �õ�CMainFrame���ڿͻ������Ρ����������ڱ������ڵ�ʱ��������ִ�λ����
//
//	////g_pView->GetWindowRect(wr);		// �õ�View���ڵľ���
//	////::GetClientRect(m_hWnd, wr);		// �õ�View���ڿͻ������Ρ����ַ�ʽ���������ʱ����������
//	////::GetWindowRect(m_hWnd, wr);		// �õ�View���ڵľ��Ρ����ַ�ʽ���������ʱ����������
//
//	//if (wParam != SIZE_MAXIMIZED)	//����¼�Ϊ���
//	//{
//	//	//ccs.fRate = (double)cr.Width() / (double)g_nWindowWdith;
//	//	//ShowWindow(SW_HIDE);//���������� 
//	//	//Shell_NotifyIcon(NIM_ADD,&m_nid);//�����������ͼ��
//	//}
//
//	////if (wr.Width() > 1195 || wr.Width() < 1185)		// Ϊ�������������ȡֵ��Χ
//	//g_fcCcs.SetCardSize(wr.Width());	// �� OnSize��Ϣ��Ӧ�����У�g_pView->GetClientRect(wr)��ֵ��׼ȷ
//	////AdjustFrameToFixedSize();
//
//	return 0;
//}

//BOOL CWTLfcView::MyCenterWindow(CRect rcDlg)
//{
//	//int with = GetSystemMetrics(SM_CXFULLSCREEN);
//	//int heigh = GetSystemMetrics(SM_CYFULLSCREEN);
//	//ͨ���ϱ�����������ȡ������ʾ��Ļ�Ĵ�С����������������������
//
//	//int   cx = GetSystemMetrics(SM_CXSCREEN);
//	//int   cy = GetSystemMetrics(SM_CYSCREEN);
//	//������������ȡ����������Ļ�Ĵ�С��
//
//	CRect rcArea;
//	CRect rcScreen;
//
//	rcScreen.left = rcScreen.top = 0;
//	rcScreen.right = GetSystemMetrics(SM_CXSCREEN);
//	rcScreen.bottom = GetSystemMetrics(SM_CYSCREEN);
//
//	::SystemParametersInfo(SPI_GETWORKAREA, NULL, (PVOID)&rcArea, 0);	//��ȡ���湤������С
//
//	//int DlgWidth = rcDlg.right - rcDlg.left;
//	//int DlgHeight = rcDlg.bottom - rcDlg.top;
//
//	// find dialog's upper left based on rcCenter
//	int xLeft = (rcScreen.left + rcScreen.right) / 2 - rcDlg.Width() / 2;
//	int yTop = (rcScreen.top + rcScreen.bottom) / 2 - rcDlg.Height() / 2;
//
//	// if the dialog is outside the screen, move it inside
//	if (xLeft < rcArea.left)		xLeft = rcArea.left;
//	if (yTop < rcArea.top)	yTop = rcArea.top;
//
//	//CRect rcRet(xLeft, yTop, rcDlg.right, rcDlg.bottom);
//	CRect rcRet(xLeft, yTop, xLeft + rcDlg.Width(), yTop + rcDlg.Height());
//
//	if (rcRet.Width() > rcArea.Width())
//		rcRet.right = rcArea.right;
//	if (rcRet.Height() > rcArea.Height())
//		rcRet.bottom = rcArea.bottom;
//
//	// map screen coordinates to child coordinates
//	//return ::SetWindowPos(GetParent(), NULL, xLeft, yTop, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
//	if (g_pMain->IsZoomed())
//		g_pMain->SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
//	//g_pMain->PostMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
//
//	return g_pMain->SetWindowPos(NULL, xLeft, yTop,
//		rcRet.Width(), rcRet.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
//}

// atlwin.h of WTL91_5321_Final
//BOOL CenterWindow(_Inout_opt_ HWND hWndCenter = NULL) throw()
//{
//	ATLASSERT(::IsWindow(m_hWnd));
//
//	// determine owner window to center against
//	DWORD dwStyle = GetStyle();
//	if (hWndCenter == NULL)
//	{
//		if (dwStyle & WS_CHILD)
//			hWndCenter = ::GetParent(m_hWnd);
//		else
//			hWndCenter = ::GetWindow(m_hWnd, GW_OWNER);
//	}
//
//	// get coordinates of the window relative to its parent
//	RECT rcDlg;
//	::GetWindowRect(m_hWnd, &rcDlg);
//	RECT rcArea;
//	RECT rcCenter;
//	HWND hWndParent;
//	if (!(dwStyle & WS_CHILD))
//	{
//		// don't center against invisible or minimized windows
//		if (hWndCenter != NULL)
//		{
//			DWORD dwStyleCenter = ::GetWindowLong(hWndCenter, GWL_STYLE);
//			if (!(dwStyleCenter & WS_VISIBLE) || (dwStyleCenter & WS_MINIMIZE))
//				hWndCenter = NULL;
//		}
//
//		// center within screen coordinates
//		HMONITOR hMonitor = NULL;
//		if (hWndCenter != NULL)
//		{
//			hMonitor = ::MonitorFromWindow(hWndCenter, MONITOR_DEFAULTTONEAREST);
//		}
//		else
//		{
//			hMonitor = ::MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTONEAREST);
//		}
//		ATLENSURE_RETURN_VAL(hMonitor != NULL, FALSE);
//
//		MONITORINFO minfo;
//		minfo.cbSize = sizeof(MONITORINFO);
//		BOOL bResult = ::GetMonitorInfo(hMonitor, &minfo);
//		ATLENSURE_RETURN_VAL(bResult, FALSE);
//
//		rcArea = minfo.rcWork;
//
//		if (hWndCenter == NULL)
//			rcCenter = rcArea;
//		else
//			::GetWindowRect(hWndCenter, &rcCenter);
//	}
//	else
//	{
//		// center within parent client coordinates
//		hWndParent = ::GetParent(m_hWnd);
//		ATLASSERT(::IsWindow(hWndParent));
//
//		::GetClientRect(hWndParent, &rcArea);
//		ATLASSERT(::IsWindow(hWndCenter));
//		::GetClientRect(hWndCenter, &rcCenter);
//		::MapWindowPoints(hWndCenter, hWndParent, (POINT*)&rcCenter, 2);
//	}
//
//	int DlgWidth = rcDlg.right - rcDlg.left;
//	int DlgHeight = rcDlg.bottom - rcDlg.top;
//
//	// find dialog's upper left based on rcCenter
//	int xLeft = (rcCenter.left + rcCenter.right) / 2 - DlgWidth / 2;
//	int yTop = (rcCenter.top + rcCenter.bottom) / 2 - DlgHeight / 2;
//
//	// if the dialog is outside the screen, move it inside
//	if (xLeft + DlgWidth > rcArea.right)
//		xLeft = rcArea.right - DlgWidth;
//	if (xLeft < rcArea.left)
//		xLeft = rcArea.left;
//
//	if (yTop + DlgHeight > rcArea.bottom)
//		yTop = rcArea.bottom - DlgHeight;
//	if (yTop < rcArea.top)
//		yTop = rcArea.top;
//
//	// map screen coordinates to child coordinates
//	return ::SetWindowPos(m_hWnd, NULL, xLeft, yTop, -1, -1,
//		SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
//}

//LRESULT CWTLfcView::OnLButtonDblClk(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
void CWTLfcView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//bHandled = false;

	//long nXPos = static_cast<short>(LOWORD(lParam));
	//long nYPos = static_cast<short>(HIWORD(lParam));
	//long nColumn = -1;
	//POINT ptMouse = { nXPos, nYPos };

	//����˫���������пյ���˫����Ч
	if (g_fcData.m_bEnableDbClick)
	{
		//ȡ��ѡ����
		g_fcData.UnselectCardCol();
		//���Ի�������
		UINT curHit = CardColHitTest(point), emptyCol;
		if (
			g_fcData.ColInBuf(curHit)
			&&
			g_fcData.ColInCard(emptyCol = g_fcData.FindEmptyCardCol())

			||

			g_fcData.ColInCard(curHit)
			&&
			(
			g_fcData.ColInBuf(emptyCol = g_fcData.FindEmptyBuf())
			||
			g_fcData.ColInCard(emptyCol = g_fcData.FindEmptyCardCol())
			)
			)
		{
			HitAt(point);//���跢������������������һ���������е�ǰ˫���ĵط�
			HitAt(g_fcData.RectOf(emptyCol, 1, 1).CenterPoint());//�ڶ���������������ĳ�յ�����
		}
	}

	//return 0;
}

//���Ի�������һ��
UINT CWTLfcView::CardColHitTest(const CPoint &point)
{
	for (UINT i = 1; i <= 16; i++)
		if (g_fcData.RectOf(i, 1, 19).PtInRect(point))
			return i;
	return 0;
}

//���Ե�ǰ�����㲢���ݻ��е��е������ȡ��Ӧ����
//��������Ĵ��뱾����Ӧ����OnLButtonDown�еģ���
//���ǵ�˫�����������õ�����ģ�⣬��ֱ����OnLBtttonDblClk�е���
//OnLButtonDown��̫���ʣ�������ȡ������Ϊһ������
//��OnLButtonDown��OnLBtttonDblClk���ߵ���
void CWTLfcView::HitAt(CPoint point)
{
	UINT nMv = 0, hit;
	//���Ե�ǰ��������һ����
	if (!(hit = CardColHitTest(point)))
		goto ret;//û�л����κ�һ��

	//���ε���֮ǰδѡ���κ���
	if (!g_fcData.m_nSel) {
		if (hit <= 12 && g_fcData.CntCardsIn(hit))//�������л򻺳����Ҹ��зǿ�
			g_fcData.SelectCardCol(hit);//ѡ�д���
		goto ret;
	}
	//�Ѿ�ѡ��ĳ�У����л����л�����
	if (hit <= 12) {
		if (hit == g_fcData.m_nSel) { //�����˱�ѡ������ȡ����ǰѡ��
			g_fcData.UnselectCardCol();
			goto ret;
		}
		//�������в���ѡ���У������ƶ����ţ�
		if (!(nMv = g_fcData.CntMaxMv(hit, g_fcData.m_nSel))) {
			if (!g_fcData.m_bEnableAlert) {
				//ѡ�е�ǰ�����е��У����бض��ǿգ�
				g_fcData.SelectCardCol(hit);
			}
			else {
				MessageBox(TEXT("���ܿյ������򲻺Ϲ���"),
					TEXT("���������ƶ�"), MB_ICONWARNING | MB_OK);
			}
			goto ret;
		}
		//=================================-============================
		//�����ƶ������ŵ�Ŀ���У�Ŀ���п����ǿ���Ҳ���Բ��ǣ�Դ����ȷ��
		//��Դ����һ�����ƣ���һ����SelectCardCol��CntMaxMv��֤
		//=================================-============================
		if (g_fcData.ColInCard(g_fcData.m_nSel) && //ѡ������
			g_fcData.ColInCard(hit) &&
			g_fcData.IsEmptyCol(hit) && //���п�����
			nMv > 1) { //���ƶ�����
			//���û�����á��������ƶ�����ơ�ѡ���ѯ��
			if (!g_fcData.m_bMaxMove && IDCANCEL == MessageBox(
				TEXT("�����ƶ��Ƶ��˴���"), TEXT("�ƶ�"), MB_OKCANCEL))
				nMv = 1;
		}
		//һ��Ҫ����g_fcData.m_nSel����ΪMoveCards�������ǰѡ�б�ǣ�
		//����g_fcData.m_nSelΪ0������Record�оͲ�����ȷ��¼��ǰѡ����
	Move:		UINT colSel = g_fcData.m_nSel;
		g_fcData.MoveCards(hit, colSel, nMv);
		g_fcData.Record(new COperations(hit, colSel, nMv));
	}
	//���ε���֮ǰ�Ѿ�ѡ��ĳ�У����ڻ��л�����
	else if ((nMv = g_fcData.CntMaxMv(hit, g_fcData.m_nSel)) == 1) {
		goto Move;
	}
ret:	g_fcData.AutoThrow();//�Զ�����
	g_fcData.CheckGame();//��Ϸ�Ƿ������
	//Invalidate();		// ��Ч������˸
	//UpdateWindow();	// ��Ч
}


//LRESULT CWTLfcView::OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
void CWTLfcView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	//������ʾ�ƶ�Ӧ�İ�ť���ڴ˴���
	bool bInverted = false;
	UINT hit = CardLabelHitTest(point);
	if (hit) {
		//CJLDoc *pDoc = GetDocument();
		CARD_POS pos[5], *p = pos, *pEnd = g_fcData.FindCardForLabel(hit, pos);
		if (p < pEnd) { //������ƶ�Ӧ�Ʊ�
			SetCapture();//�����������
			m_nCardLabelHit = hit;//��¼���Ʊ�

			CClientDC cdc(m_hWnd);
			cdc.InvertRect(g_fcData.RectOf(hit));//�Ʊ귴ɫ

			//�÷�ɫ�Ʊ�ͼ���е�������Ŀ
			CDC memDC;
			memDC.CreateCompatibleDC(cdc);
			//CBitmap *poldbmp = memDC.SelectObject(&m_AllBmps);
			HBITMAP hBmpOld = memDC.SelectBitmap(m_AllBmps);
			//CBrush brush(RGB(0, 255, 0));
			//CBrush brush = CreateSolidBrush(RGB(0, 255, 0));
			CBrush brush;
			brush.CreateSolidBrush(RGB(0, 255, 0));

			CRect r = g_fcData.RectOf(hit);
			cdc.FrameRect(r, brush);
			CPoint pt = r.CenterPoint();
			//pt.x -= 5; pt.y -= 7;
			//cdc.StretchBlt(pt.x,pt.y,9,15,&memDC,(hit-1)*8+32,0,8,12,NOTSRCCOPY | SRCAND);

			pt.x -= int(2.2 * g_fcCcs.dRate); pt.y -= int(4 * g_fcCcs.dRate);
			//pDC->BitBlt(p.x,p.y,8,12,&memDC,(c-1)*8+32,0,SRCAND);
			//pDC->BitBlt(p.x,p.y,8,12,&memDC,(c-1)*8+32,0,NOTSRCCOPY | SRCAND);
			//pDC->StretchBlt(p.x,p.y,int(5 * ccs.dRate),int(10 * ccs.dRate),&memDC,(c-1)*8+32,0,8,12,NOTSRCCOPY | SRCAND);
			cdc.StretchBlt(pt.x, pt.y, int(5 * g_fcCcs.dRate), int(8 * g_fcCcs.dRate), memDC, (hit - 1) * 8 + 32, 0, 8, 12, NOTSRCCOPY | SRCAND);

			memDC.SelectBitmap(hBmpOld);

			//CBrush brRed(RGB(255, 0, 0));
			//CBrush brRed = CreateSolidBrush(RGB(255, 0, 0));
			CBrush brRed;
			brRed.CreateSolidBrush(RGB(255, 0, 0));

			//CBrush brBlk(RGB(0,0,0));
			//CBrush brBlk(RGB(0, 0, 255));
			//CBrush brBlk = CreateSolidBrush(RGB(0, 0, 255));
			CBrush brBlk;
			brBlk.CreateSolidBrush(RGB(0, 0, 255));

			//WTL�б����ڴ˶���
			CBrush hBr;
			while (p < pEnd) { //������Ʊ��Ӧ������ʾ��������ɫ
				CRect r = g_fcData.RectOf(p->col, p->idx, 1);
				UINT card = g_fcData.GetCard(p->col, p->idx);
				DrawCard(r.TopLeft(), card, &cdc);

				//WTL���������壬ֻ���ʼ��һ�Σ����¸�ֵ�ᵼ��hBrʧЧ
				//CBrush hBr = TYPE(card) % 2 == 0 ? brBlk : brRed;
				hBr = TYPE(card) % 2 == 0 ? brBlk : brRed;

				cdc.FrameRect(r, hBr);
				r.InflateRect(-1, -1);
				cdc.FrameRect(r, hBr);
				r.InflateRect(-1, -1);
				cdc.FrameRect(r, hBr);

				++p;
			}
		}
	}
	else {
		HitAt(point);//���������������ﴦ��
	}

	//return 0;
}


//LRESULT CWTLfcView::OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
void CWTLfcView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	UINT hit = m_nCardLabelHit;
	m_nCardLabelHit = 0;
	if (hit) {
		ReleaseCapture();
		//CJLDoc *pDoc = GetDocument();
		InvalidateRect(g_fcData.RectOf(hit));//�Ʊ귴ɫ
		CARD_POS pos[5], *p = pos, *pEnd = g_fcData.FindCardForLabel(hit, pos);
		while (p < pEnd) { //������Ʊ��Ӧ����������ʾ����
			CRect r = g_fcData.RectOf(p->col, p->idx, 1);
			InvalidateRect(r);
			++p;
		}
	}

	//return 0;
}


//LRESULT CWTLfcView::OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
void CWTLfcView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	UINT curHit = CardColHitTest(point);
	if
		(	//��ѡ����
		g_fcData.IsCol(g_fcData.m_nSel)
		//���������ƶ�
		&& g_fcData.IsCol(curHit)
		//��ѡ�е����в���Ŀǰ������ڵ���
		&& g_fcData.m_nSel != curHit
		//�յ��㹻�����ù������ʾ�����ƶ������ŵ�Ŀ����
		&& g_fcData.CntMaxMv(curHit, g_fcData.m_nSel)
		)
	{
		SetCursor(m_hDown);
	}

	//return 0;
}

//���Ի�������һ����ǩ
UINT CWTLfcView::CardLabelHitTest(const CPoint &point)
{
	for (UINT i = 1; i <= 13; i++)
		if (g_fcData.RectOf(i).PtInRect(point))
			return i;
	return 0;
}


//LRESULT CWTLfcView::OnRButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
void CWTLfcView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	//���Ե���������
	UINT hit = CardColHitTest(point);
	//��������
	if (g_fcData.ColInCard(hit)) {
		for (UINT idx = 1; idx < g_fcData.CntCardsIn(hit); idx++) {
			//���б�ѹס����
			if (g_fcData.RectOf(hit, idx, 1).PtInRect(point) &&
				!g_fcData.RectOf(hit, idx + 1, 1).PtInRect(point)) {
				m_rectRBDown = g_fcData.RectOf(hit, idx, 1);
				CClientDC cdc(m_hWnd);
				DrawCard(m_rectRBDown.TopLeft(), g_fcData.GetCard(hit, idx), &cdc);
				SetCapture();
				break;
			}
		}
	}

	//return 0;
}


//LRESULT CWTLfcView::OnRButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
void CWTLfcView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	//ˢ�±��Ҽ��������ռ�ݵľ�������
	if (!m_rectRBDown.IsRectEmpty()) {
		InvalidateRect(m_rectRBDown);
		m_rectRBDown.SetRectEmpty();
		ReleaseCapture();
	}

	//return 0;
}

LRESULT CWTLfcView::OnUndo(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your message handler code here and/or call default

	g_fcData.Undo();
	g_fcData.GetHints();

	return 0;
}

LRESULT CWTLfcView::OnRand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your command handler code here

	if (!g_fcData.GiveUp()) return 0;
	int nUniqueGame = g_fcData.Random();
	//while (m_dlgScore.IsOldGameNumber(nUniqueGame = Random()));
	g_fcData.StartGame(nUniqueGame);

	return 0;
}

//�Զ����
LRESULT CWTLfcView::OnAi(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your command handler code here
	g_fcData.UnselectCardCol();//ȡ��ѡ��״̬
	g_fcData.m_Hints.ClrHints();//�����ʾ�ļ�¼

	CDlgAICal dlgAICal;//�Զ����
	dlgAICal.DoModal();
	//UpdateAllViews(NULL);//����ʹ���˿��ٽ������Ҫˢ�½���
	g_pView->RedrawWindow();

	if (!dlgAICal.m_bSuccess) {
		//AfxMessageBox(TEXT("��Ǹ���Զ����δ�ܳɹ�!"));
		::MessageBox(g_pView->m_hWnd, TEXT("��Ǹ���Զ����δ�ܳɹ�!"), TEXT("WTLfc"), MB_OK);
	}
	g_fcData.CheckGame();//���Ѿ��ҵ�����ͷ��ʼ��ʾ

	return 0;
}

//�Զ����
//void WTLfcData::OnAi()
//{
//	// TODO: Add your command handler code here
//	UnselectCardCol();//ȡ��ѡ��״̬
//	m_Hints.ClrHints();//�����ʾ�ļ�¼
//
//	CDlgAICal dlgAICal;//�Զ����
//	dlgAICal.DoModal();
//	UpdateAllViews(NULL);//����ʹ���˿��ٽ������Ҫˢ�½���
//
//	if (!dlgAICal.m_bSuccess) {
//		AfxMessageBox(TEXT("��Ǹ���Զ����δ�ܳɹ�!"));
//	}
//	CheckGame();//���Ѿ��ҵ�����ͷ��ʼ��ʾ
//}

//��һ��
//void CWTLfcView::OnPrevGame()
LRESULT CWTLfcView::OnPrevGame(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your command handler code here
	if (!g_fcData.GiveUp()) return 0;
	g_fcData.StartGame(max(g_fcData.m_nCurGameNumber - 1, MIN_GAME_NUMBER));
	return 0;
}

//��һ��
//void CWTLfcView::OnNextGame()
LRESULT CWTLfcView::OnNextGame(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (!g_fcData.GiveUp()) return 0;
	g_fcData.StartGame(min(g_fcData.m_nCurGameNumber + 1, MAX_GAME_NUMBER));
	return 0;
}

//����
//void CWTLfcView::OnAgain()
LRESULT CWTLfcView::OnAgain(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your command handler code here
	if (!g_fcData.GiveUp()) return 0;
	if (g_fcData.m_nCurGameNumber > 0) {
		g_fcData.StartGame(g_fcData.m_nCurGameNumber);
	}
	else {
		while (!g_fcData.m_pOps->empty()) {
			g_fcData.Undo();//��������ͷ
		}
	}
	return 0;
}

//ս��
//void WTLfcData::OnScore()
LRESULT CWTLfcView::OnScore(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	g_fcData.m_dlgScore.DoModal();
	return 0;
}

LRESULT CWTLfcView::OnEdit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	return 0;
}

//ѡ��
LRESULT CWTLfcView::OnSelectgamenumber(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (!g_fcData.GiveUp()) return 0;

	CDlgSelGame dlg;
	dlg.m_nGameNumber = g_fcData.m_nCurGameNumber;
	if (dlg.DoModal() != IDOK) return 0;

	g_fcData.StartGame(dlg.m_nGameNumber);

	return 0;
}

//��ʾ��һ��
//void CWTLfcView::OnHelpNextstep()
LRESULT CWTLfcView::OnHelpNextstep(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your command handler code here
	if (g_fcData.m_Hints.IsEmpty()) return 0;

	//��ʾǰȡ��ѡ��״̬
	g_fcData.UnselectCardCol();

	//ȡ����һ�������ļ�¼����ʾ���
	const COperation *pOp = g_fcData.m_Hints.NextHint();

	UINT cntSrc = g_fcData.CntCardsIn(pOp->src);
	UINT cntDes = g_fcData.CntCardsIn(pOp->des);
	CRect sR = g_fcData.RectOf(pOp->src, cntSrc - pOp->cnt + 1, pOp->cnt);
	CRect dR = g_fcData.RectOf(pOp->des, max(cntDes, 1), 1);

	//CJLView *pView = GetView();
	CClientDC cdc(g_pView->m_hWnd);
	//��ʾ���̾�����˸Դ�к�Ŀ�е���
	cdc.InvertRect(sR); cdc.InvertRect(dR);//��ɫ
	SendMessage(g_pView->m_hWnd, WM_PAINT, 0, 0);
	::Sleep(200);
	cdc.InvertRect(sR); cdc.InvertRect(dR);//��ԭ
	SendMessage(g_pView->m_hWnd, WM_PAINT, 0, 0);
	::Sleep(200);
	cdc.InvertRect(sR); cdc.InvertRect(dR);//��ɫ
	SendMessage(g_pView->m_hWnd, WM_PAINT, 0, 0);
	::Sleep(200);
	cdc.InvertRect(sR); cdc.InvertRect(dR);//��ԭ

	return 0;
}
