// DGView.cpp : implementation file
//

#include "stdafx.h"
//#include "JL.h"
//#include "JLDoc.h"
//#include "JLView.h"
#include "DGView.h"
#include "DlgDefGame.h"
#include "CardSize.h"
//����֧��ϴ�Ƶĺ���
#include <algorithm>
#include <functional>

//#ifdef _DEBUG
//#define new DEBUG_NEW
//#undef THIS_FILE
//static char THIS_FILE[] = __FILE__;
//#endif

/////////////////////////////////////////////////////////////////////////////
// CDGWnd
//extern CJLDoc * AfxGetDocument();
//extern CJLView * AfxGetView();
//extern CCardSize g_fcCcs;

//IMPLEMENT_DYNCREATE(CDGWnd, CWnd)
//IMPLEMENT_DYNCREATE(CDGWnd, CWindowImpl)

//BOOL CDGWnd::PreTranslateMessage(MSG* pMsg)
//{
//	if (CFrameWindowImpl<CDGWnd>::PreTranslateMessage(pMsg))
//		return TRUE;
//
//	return FALSE;
//}
//
//BOOL CDGWnd::OnIdle()
//{
//	return FALSE;
//}

CDGWnd::CDGWnd()
{
	Init();
}

CDGWnd::~CDGWnd()
{
}


//BEGIN_MESSAGE_MAP(CDGWnd, CWnd)
//	//{{AFX_MSG_MAP(CDGWnd)
//	ON_WM_LBUTTONDOWN()
//	ON_WM_LBUTTONDBLCLK()
//	ON_WM_ERASEBKGND()
//	ON_WM_PAINT()
//	//}}AFX_MSG_MAP
//END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDGWnd diagnostics

//#ifdef _DEBUG
//void CDGWnd::AssertValid() const
//{
//	CWnd::AssertValid();
//}
//
//void CDGWnd::Dump(CDumpContext& dc) const
//{
//	CWnd::Dump(dc);
//}
//#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDGWnd drawing

LRESULT CDGWnd::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//void CDGWnd::OnPaint()
{
	//CPaintDC dc(this);
	//CJLView *pView = AfxGetView();
	CPaintDC dc(m_hWnd);

	//�����ڴ�DC��λͼ
	CDC dcMem;
	dcMem.CreateCompatibleDC(dc);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(dc, g_fcCcs.CARD_WID, g_fcCcs.CARD_HEI);
	//�����ƺ���
	//CBitmap *pOldBmp = dcMem.SelectObject(&bmp);
	HBITMAP hBmpOld = dcMem.SelectBitmap(bmp);
	//����˾��ѱ༭���������ƺ��ӵ���������ʾ��ʾ
	if (GameIsValid()) {
		CFont font;
		font.CreatePointFont(20 * 10, TEXT("Arial"), dc);
		HFONT prevFont = dc.SelectFont(font);
		int prevMode = dc.SetBkMode(TRANSPARENT);
		WTL::CString str(TEXT("�˾ֱ༭��ϣ���浵��"));
		dc.DrawText(str, str.GetLength(), RectOfBox(), DT_CENTER);
		dc.SetBkMode(prevMode);
		dc.SelectFont(prevFont);
	}
	for (UINT i = 52; i >= 1; --i) {
		CRect r = RectOf(i);
		UINT card = Get(i);
		if (!card) continue;
		//���ڴ�DC�л��ƴ��Ʋ����������д��Ƶľ���λ����
		g_pView->DrawCard(CPoint(0, 0), card, &dcMem);
		dc.BitBlt(r.left, r.top, r.Width(), r.Height(), dcMem, 0, 0, SRCCOPY);
		if (m_iSrcCol == i) {
			dc.InvertRect(r);        //�����ѡ��״̬��ɫ
		}
	}
	dcMem.SelectBitmap(hBmpOld);
	//��������
	//�������ƣ�ǰ����ÿ�����ţ�������ÿ�����ţ�����52��
	for (int i = 1; i <= 8; i++) {
		UINT m = (i <= 4 ? 7 : 6);
		for (UINT j = 1; j <= m; j++) {
			int idx = (i << 8) + j, card = Get(idx);
			CRect r = RectOf(idx);
			//������ƾͻ��տ�
			if (!card) {
				if (j < m) r.bottom += 5;
				CBrush OldBrush = dc.SelectBrush(g_pView->m_brushBkgnd);		// ��ˢ�ı任�������������
				dc.RoundRect(r, CPoint(5, 5));
				dc.SelectBrush(OldBrush);	// ��ˢ�ı任�������������
			}
			//������ƴ��� 
			else {
				g_pView->DrawCard(r.TopLeft(), card, &dc);	 // �任��ˢ���¸þ����ʼ�ձ���������ǰ����л�ˢ�任
				if (m_iSrcCol == idx) dc.InvertRect(r);
			}
		}
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CDGWnd message handlers
//˫�������ϵ��ƽ���Ѹ��ƻ��յ��ƺ�����
//void CDGWnd::OnLButtonDblClk(UINT nFlags, CPoint point) 
void CDGWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	//����˫����������
	UINT hit = HitTest(point);
	//���˫���������ϵ���
	if (hit && !IS_CARD(hit) && Get(hit)) {
		if (m_iSrcCol) { //ѡ��ĳ��
			CRect rBack = RectOf(m_iSrcCol);//��ǰѡ�е��еľ��εı���
			m_iSrcCol = 0;//ȡ��ѡ��
			InvalidateRect(rBack);
		}
		UINT card = Get(hit);
		Set(hit, 0);
		InvalidateRect(RectOf(hit));
		Set(card, card);//���մ���
		InvalidateRect(RectOf(card));
		m_iSrcCol = 0;
		m_bModified = true;
		SetTitle();
	}

	CheckGame();

	//CWnd::OnLButtonDblClk(nFlags, point);
}

//void CDGWnd::OnLButtonDown(UINT nFlags, CPoint point) 
void CDGWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	UINT hit = HitTest(point);
	if (!hit) {
		;
	}
	//��Դ��
	else if (!m_iSrcCol) {
		if (Get(hit)) { //������
			m_iSrcCol = hit;//ѡ�д���
			InvalidateRect(RectOf(hit));
		}
	}
	//��Դ��
	else if (!Get(hit)) {
		//���п��� 
		if (!IS_CARD(hit)) {
			//�������
			UINT card = Get(m_iSrcCol);
			Set(m_iSrcCol, 0);
			InvalidateRect(RectOf(m_iSrcCol));
			Set(hit, card);
			InvalidateRect(RectOf(hit));

			m_iSrcCol = 0;
			m_bModified = true;

			SetTitle();
		}
		//���пպ��ӣ���Դ��������
		else if (!IS_CARD(m_iSrcCol)) {
			UINT card = Get(m_iSrcCol);
			Set(m_iSrcCol, 0);
			InvalidateRect(RectOf(m_iSrcCol));
			Set(card, card);//���մ���
			InvalidateRect(RectOf(card));

			m_iSrcCol = 0;
			m_bModified = true;

			SetTitle();
		}
	}
	//������
	else {
		CRect rBack = RectOf(m_iSrcCol);//��ǰѡ�е��еľ��εı���
		//����Դ��
		if (hit == m_iSrcCol) {
			m_iSrcCol = 0;//ȡ��ѡ��
		}
		else {
			m_iSrcCol = hit;//ѡ�л��е���
			InvalidateRect(RectOf(hit));
		}
		InvalidateRect(rBack);
	}

	CheckGame();

	//CWnd::OnLButtonDown(nFlags, point);
}
//���в��ԣ����������������
UINT CDGWnd::HitTest(const CPoint &pt)
{
	//�����Ƿ����ƺ��ӵ�ĳ���ϵ��
	for (UINT i = 1; i <= 52; i++) {
		CRect r = RectOf(i);
		if (r.PtInRect(pt)) return i;
	}
	//�����Ƿ��������Ϸ��������
	for (int i = 1; i <= 8; i++) {
		UINT x = i <= 4 ? 7 : 6;//��������
		for (UINT j = 1; j <= x; j++) {
			UINT idx = (i << 8) + j;
			CRect r = RectOf(idx);
			if (r.PtInRect(pt)) return idx;
		}
	}
	return 0;
}
//����ָ������ռ�ݵľ���λ��
CRect CDGWnd::RectOf(UINT pos)
{
	CRect cr;
	GetClientRect(cr);
	int wid = cr.Width() / 13;
	int hei = (cr.Height() - g_fcCcs.CARD_HEI) / (6 + 4 + 1);
	//�ƺ����е���ÿ�����ڵľ���
	CRect r(0, 0, 0, 0);
	if (IS_CARD(pos)) {
		r.left = (13 - NUM(pos))*wid;
		r.right = r.left + wid;
		r.top = TYPE(pos)*hei;
		r.bottom = r.top + hei;
	}
	//������ÿ������ռ�ݵľ���
	else {
		UINT col = pos >> 8;
		UINT idx = pos & 0x000000FF;
		ATLASSERT(col <= 8 && col >= 1 && idx >= 1);
		ATLASSERT(col <= 4 && idx <= 7 || col >= 4 && idx <= 6);

		CPoint p(0, hei * 5);
		int inter = (cr.Width() - 8 * g_fcCcs.CARD_WID) / 9;
		int x = (col - 1) * (inter + g_fcCcs.CARD_WID) + inter;
		int y = (4 + idx) * hei;
		r.SetRect(
			x,
			y,
			x + g_fcCcs.CARD_WID,
			y + ((col <= 4 && idx == 7 || col > 4 && idx == 6) ? g_fcCcs.CARD_HEI : hei)
			);
	}
	return r;
}
//��ָ����λ�ô�Ÿ�������
void CDGWnd::Set(UINT pos, UINT card)
{
	//���λ�������ƺ�����
	if (IS_CARD(pos)) {
		m_iBox[pos - 1] = card;
	}
	else {
		UINT col = pos >> 8;
		UINT idx = pos & 0x000000FF;
		ATLASSERT(col <= 8 && col >= 1 && idx >= 1);
		ATLASSERT(col <= 4 && idx <= 7 || col >= 4 && idx <= 6);

		if (col <= 4) {
			m_iTbl[(col - 1) * 7 + idx - 1] = card;
		}
		else {
			m_iTbl[28 + (col - 5) * 6 + idx - 1] = card;
		}
	}
}

//���ݸ���������λ��ȡ���˴�����ֵ
UINT CDGWnd::Get(UINT pos)
{
	//������ƺ����е���
	if (IS_CARD(pos)) {
		return m_iBox[pos - 1];
	}
	//������������ϵ���
	else {
		UINT col = pos >> 8;
		UINT idx = pos & 0x000000FF;
		ATLASSERT(col <= 8 && col >= 1 && idx >= 1);
		ATLASSERT(col <= 4 && idx <= 7 || col >= 4 && idx <= 6);

		if (col <= 4) {
			return m_iTbl[(col - 1) * 7 + idx - 1];
		}
		else {
			return m_iTbl[28 + (col - 5) * 6 + idx - 1];
		}
	}
}

//BOOL CDGWnd::OnEraseBkgnd(CDC* pDC) 
LRESULT CDGWnd::OnEraseBkgnd(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	HDC hDc = (HDC)wParam;
	CRect cr;
	GetClientRect(cr);
	FillRect(hDc, cr, g_pView->m_brushBkgnd);
	return 1;
}


void CDGWnd::NewDefGame()
{
	if (!GiveUp()) return;

	Init();
	InvalidateRect(NULL);
	SetTitle();
}

void CDGWnd::SaveDefGameAs()
{
	WTL::CString name = m_strFile;
	bool bModi = m_bModified;

	m_strFile.Empty();
	m_bModified = true;

	SaveDefGame();

	if (m_strFile.IsEmpty()) {
		m_strFile = name;
		m_bModified = bModi;
	}
}

void CDGWnd::SaveDefGame()
{
	struct SIZE_INF { UINT size, *pAddr; } cols[3] = {
		{ sizeof(m_iCards) / sizeof(UINT), &m_iCards[0][0] },
		{ sizeof(m_iBuffer) / sizeof(UINT), &m_iBuffer[0] },
		{ sizeof(m_iRecycle) / sizeof(UINT), &m_iRecycle[0][0] }
	};
	if (!m_bModified) return;

	if (m_strFile.IsEmpty()) {
		//CFileDialog dlg(FALSE, TEXT("rep"), TEXT("�Զ����ƾ�"), dwFlags, filter);
		//if (dlg.DoModal() == IDCANCEL) return;
		//m_strFile = dlg.GetPathName();
		CFileDialog fd(FALSE, L"txt", TEXT("�Զ����ƾ�"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
			L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0\0", m_hWnd);
		if (fd.DoModal() == IDCANCEL) return;
		fd.m_ofn.lpstrInitialDir = L".";
		m_strFile = fd.m_szFileName;

		SetTitle();
	}
	//CFile file(m_strFile, modeCrWr);
	//CArchive ar(&file, CArchive::store);

	// Storing
	CXFile file1;
	file1.Open(wstring(m_strFile),						// file name
		GENERIC_WRITE | GENERIC_READ,			// access mode 
		FILE_SHARE_READ | FILE_SHARE_WRITE,		// share mode 
		NULL,									// no security 
		CREATE_ALWAYS,							// create a new file, overwrite if it exists
		FILE_ATTRIBUTE_NORMAL,					// file attributes
		NULL);									// no template file

	CXArchive ar1(&file1, CXArchive::store);
	g_fcData.Serialize(ar1);

	CTList<COperations> ops;
	CDlgScore dlgScore;

	if (GameIsValid()) {
		////////////////////////////////////////////////////////////
		//�������Զ����ƾ֣����ƾִ���Ϊ0��
		//����ʱ��Ϊһ���������ƾִ浵�ļ�
		////////////////////////////////////////////////////////////
		Shuffle();//ϴ��
		m_nCurGameNumber = 0;
		ar1 << m_nCurGameNumber;			//1�����汾�ִ���
		//CObList ops;
		//m_OpsList.Serialize(ar);//���沽���¼
		ops.Serialize(ar1);				//2������յĲ����¼
		for (UINT k = 0; k < 3; ++k)		//3�������ƾ�
			for (UINT i = 0; i < cols[k].size; i++)
				ar1 << cols[k].pAddr[i];

		//m_dlgScore.m_score.Serialize(ar);//����ս����¼
		dlgScore.m_score.Serialize(ar1);//4������յ�ս����¼
	}
	else {
		////////////////////////////////////////////////////////////
		//���������Զ����ƾ֣����ƾִ���Ϊ-1��
		//����ʱֻ���浱ǰ�༭ʱ���õ���Ϣ�������´��ٱ༭
		////////////////////////////////////////////////////////////
		m_nCurGameNumber = -1;
		ar1 << m_nCurGameNumber;			//1�����汾�ִ���
		for (UINT i = 0; i < 52; i++) { ar1 << m_iTbl[i]; }	//2����������
		for (int i = 0; i < 52; i++) { ar1 << m_iBox[i]; }	//3�������ƺ���
		dlgScore.m_score.Serialize(ar1);//4������յ�ս����¼
	}
	//ar.Close();
	//file.Close();

	ar1.Close();
	// CXFile file1 �� ~CXFile �ر�

	m_bModified = false;

	SetTitle();
}
//�����������ƾִ��ž�����ζ�����
//����ƾִ���Ϊ0��˵�����������Զ����ƾ֣�
//	��ʱ���ݲ����¼�жϴ˾��Ƿ��߹����ɲ������������ʾ����ȳ���������״̬���浵��༭��
//	�����˵���ƾ�����������û���߹�����ô�Ͷ����ƺ��Ӻ����棻
//����ƾִ��Ŵ���0��˵���Ǳ�׼�ƾֵĴ浵�ļ���
//	��ʱ����������һ��ԭʼ�ƾ֣�
//����ƾִ���Ϊ-1��˵����δ�༭�������Զ����ƾ֣�
//	��ʱ�Ͷ����ƺ��Ӻ����档
void CDGWnd::LoadDefGame()
{
	struct SIZE_INF { UINT size, *pAddr; } cols[3] = {
		{ sizeof(m_iCards) / sizeof(UINT), &m_iCards[0][0] },
		{ sizeof(m_iBuffer) / sizeof(UINT), &m_iBuffer[0] },
		{ sizeof(m_iRecycle) / sizeof(UINT), &m_iRecycle[0][0] }
	};

	if (!GiveUp()) return;

	//CFileDialog dlg(TRUE, TEXT("rep"), NULL, dwFlags, filter);
	//if (dlg.DoModal() == IDCANCEL) return;
	CFileDialog fd(TRUE, L"txt", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0\0", m_hWnd);
	fd.m_ofn.lpstrTitle = L"ReadFileTest";
	fd.m_ofn.lpstrInitialDir = L".";

	if (fd.DoModal() == IDCANCEL) return;

	Init();

	//CFile file(dlg.GetPathName(), modeRead);
	//CArchive ar(&file, CArchive::load);

	// Loading
	CXFile file2;
	file2.Open(fd.m_szFileName,					// file name
		GENERIC_WRITE | GENERIC_READ,			// access mode 
		FILE_SHARE_READ | FILE_SHARE_WRITE,		// share mode 
		NULL,									// no security 
		//CREATE_ALWAYS,							// create a new file, overwrite if it exists
		OPEN_EXISTING,							// open the file, if it exists
		FILE_ATTRIBUTE_NORMAL,					// file attributes
		NULL);									// no template file

	CXArchive ar2(&file2, CXArchive::load);

	CTList<COperations> ops;
	CDlgScore dlgScore;

	//��ȡ���ִ���
	ar2 >> m_nCurGameNumber;					//1����ȡ�ƾִ���

	//�����δ�༭��ɵ��ƾ�
	if (m_nCurGameNumber == -1)
	{
		for (UINT i = 0; i < 52; i++) { ar2 >> m_iTbl[i]; }	//2����ȡ����
		for (int i = 0; i < 52; i++) { ar2 >> m_iBox[i]; }	//3����ȡ�ƺ���
		dlgScore.m_score.Serialize(ar2);		//4����ȡս����¼
	}
	//����Ǳ�׼�ƾֵĴ浵
	else if (m_nCurGameNumber > 0)
	{
		//׼��һ�����ƣ���ϴ��
		int cards[52];
		for (int i = 1; i <= 52; ++i) cards[i - 1] = i;

		srand(m_nCurGameNumber >> 16);
		random_shuffle(cards, cards + 52);
		srand(m_nCurGameNumber & 0xFFFF);
		random_shuffle(cards, cards + 52);

		for (int i = 0; i < 52; i++) { m_iTbl[i] = cards[i]; }	//���Ƶ�����
		for (int i = 0; i < 52; i++) { m_iBox[i] = 0; }			//׼���յ��ƺ���
	}
	//�Ѿ���ɵ��Զ����ƾ�
	else
	{
		ops.Serialize(ar2);						//2����ȡ�����¼
		UINT nSteps = ops.m_tList.size();
		//ClrOpsRecords(&ops);		//��������¼
		ops.m_tList.clear();			//��������¼
		if (nSteps == 0) {
			for (UINT k = 0; k < 3; ++k)			//3����ȡ�ƾ�
				for (UINT i = 0; i < cols[k].size; i++)
					ar2 >> cols[k].pAddr[i];

			for (int col = 0; col < 4; col++)//׼���ƾ�ǰ4��
				for (int idx = 0; idx < 7; idx++)
					m_iTbl[col * 7 + idx] = m_iCards[col][idx];
			for (int col = 4; col < 8; col++)//׼���ƾֺ�4��
				for (int idx = 0; idx < 6; idx++)
					m_iTbl[28 + (col - 4) * 6 + idx] = m_iCards[col][idx];

			for (UINT i = 0; i < 52; i++) { m_iBox[i] = 0; }//׼���յ��ƺ���

			dlgScore.m_score.Serialize(ar2);		//4����ȡս����¼
			dlgScore.m_score.m_tList.clear();	//���ս����¼

		}
		else
		{
			ar2.Close();//�ر��ļ�
			//file.Close();
			WTL::CString errStr;
			errStr.LoadString(IDS_RESON_UNEDITABLE);
			MessageBox(errStr);
			return;
		}
	}
	//ar.Close();//�ر��ļ�
	//file.Close();

	ar2.Close();
	// CXFile file2 �� ~CXFile �ر�

	m_strFile = fd.m_szFileName;
	m_bGameIsValid = GameIsValid();

	InvalidateRect(NULL);
	SetTitle();
}
//ϴ�Ʋ����Ƶ�����
void CDGWnd::Shuffle()
{
	//��ջ����С������к�����
	struct SIZE_INF { UINT size, *pAddr; };
	const SIZE_INF cols[3] = {
		{ sizeof(m_iCards) / sizeof(UINT), &m_iCards[0][0] },
		{ sizeof(m_iBuffer) / sizeof(UINT), &m_iBuffer[0] },
		{ sizeof(m_iRecycle) / sizeof(UINT), &m_iRecycle[0][0] },
	};
	for (UINT k = 0; k < 3; ++k)
		for (UINT i = 0; i < cols[k].size; i++)
			cols[k].pAddr[i] = 0;
	//����
	for (int col = 0; col <= 3; col++) {
		UINT *pTop;

		pTop = &m_iCards[col][0];
		for (UINT i = 0; i < 7; i++) {
			UINT *pDes = &pTop[pTop[19]];//ָ�����֮��
			pDes[i] = m_iTbl[col * 7 + i];
		}
		pTop[19] = 7;

		pTop = &m_iCards[col + 4][0];
		for (int i = 0; i < 6; i++) {
			UINT *pDes = &pTop[pTop[19]];//ָ�����֮��
			pDes[i] = m_iTbl[28 + col * 6 + i];
		}
		pTop[19] = 6;
	}
}

//////////////////////////////////////////////////////////////////////
//�����ƾ��Ƿ�༭��ȫ
//�ƺ����е���ȫ���õ��������ˣ���˵���༭��ɣ��ƾ�����������
//////////////////////////////////////////////////////////////////////
bool CDGWnd::GameIsValid()
{
	for (UINT i = 0; i < 52; i++) {
		if (m_iBox[i]) return false;
	}
	return true;
}

//�����ƾ��Ƿ��޸Ĺ������Ƿ���ʾ�浵
BOOL CDGWnd::GiveUp()
{
	return !m_bModified || IDYES ==
		MessageBox(L"�ƾ��Ѿ����ģ���δ�浵��Ҫ��������", L"�Զ����ƾ�", MB_YESNO);
}
//��������¼
//void CDGWnd::ClrOpsRecords(CObList *pList)
//{
//	POSITION p = pList->GetHeadPosition();
//	while(p) {
//		COperations *pOps = (COperations *)pList->GetNext(p);
//		pOps->ClrOps();
//		delete pOps;
//	}
//	pList->RemoveAll();
//}

void CDGWnd::Init()
{
	m_bGameIsValid = false;
	m_strFile.Empty();
	m_iSrcCol = 0;
	for (UINT i = 0; i < 52; i++) {
		m_iTbl[i] = 0;//����������
		m_iBox[i] = i + 1;//׼��һ�����Ʒ����ƺ�����
	}
	m_bModified = false;
}
//���ݳ���ǰ״̬�����ڱ���������ʾʲô
//���û�д浵���ѸĶ������ļ��������*��
void CDGWnd::SetTitle()
{
	WTL::CString title = L"�Զ����ƾ�";
	const WTL::CString strModi = "*";

	if (!m_strFile.IsEmpty()) {
		int idx = m_strFile.ReverseFind('\\');
		title = m_strFile.Right(m_strFile.GetLength() - idx - 1);
	}
	if (m_bModified) {
		title += strModi;
	}
	//CDlgDefGame *pDlg = (CDlgDefGame *)GetParent();
	//pDlg->SetWindowText(title);
	SetWindowText(title);
}

void CDGWnd::CheckGame()
{
	//���ݵ�ǰ�����ж��Զ����ƾ��Ƿ�༭����
	bool isValidNow = GameIsValid();
	//�����ǰ״̬�͵��ǰ��״̬��ͬ��ˢ���ƺ���
	//Ҳ����˵�����Ƿ���ʾ�ƾ��Ƿ�༭��������ʾ��Ϣ
	if (m_bGameIsValid ^ isValidNow) {
		m_bGameIsValid = isValidNow;
		InvalidateRect(RectOfBox());
	}
}
//���������ƺ��ӵľ���
CRect CDGWnd::RectOfBox()
{
	CRect r;
	r.UnionRect(RectOf(1), RectOf(52));
	return r;
}

LRESULT CDGWnd::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	//EndDialog(wID);
	return 0;
}

LRESULT CDGWnd::OnOKCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// First DDX call, hooks up controls to variables.
	//DoDataExchange(true);

	//EndDialog(wID);
	return 0;
}

LRESULT CDGWnd::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	//CenterWindow(GetParent());

	//m_strGameNumber.Format(L"%d", m_nGameNumber);

	//// First DDX call, hooks up variables to controls.
	//DoDataExchange(false);

	return 0;
}
