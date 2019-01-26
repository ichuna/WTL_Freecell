// DlgScore.cpp : implementation file
//

#include "stdafx.h"
//#include "JL.h"
//#include "JLDoc.h"
#include "DlgScore.h"

//extern CJLDoc * AfxGetDocument();

static const wchar_t * statusStr[4] = { TEXT("δ��"), TEXT("ͨ��"), TEXT("����"), TEXT("����") };
static const wchar_t * fmts = TEXT("%H:%M:%S");
static const wchar_t * fmtsYMD = TEXT("%Y.%m.%d-%H:%M:%S");

/////////////////////////////////////////////////////////////////////////////
// CDlgScore dialog

//CDlgScore::CDlgScore(CWnd* pParent /*=NULL*/)
//	: CDialog(CDlgScore::IDD, pParent)
//{
//	//{{AFX_DATA_INIT(CDlgScore)
//	//}}AFX_DATA_INIT
//}

CDlgScore::CDlgScore()	// -V730
{
}

CDlgScore::~CDlgScore()
{
	//while (!m_score.empty()) {
	//	CScore *pScore = (CScore*)m_score.back();
	//	delete pScore;
	//	m_score.pop_front();
	//}

	//for (list<CMyObject*>::iterator it = m_score.end(); it != m_score.begin();)
	//{
	//	//���һ�ֻ�û�н⣬����״̬������ʾ
	//	it--;	//	�����������������б�����ֹ��COblist �� STL list�Ĳ�ͬ

	//	//CScore *p = (CScore*)m_score.GetPrev(pos);
	//	CScore *pScore = (CScore*)(*it);	// it = m_score.end()��������it--������*it����������ʱ������ֹ
	//	delete pScore;

	//	//if(pos == m_score.GetHeadPosition()) break;
	//	//if (m_score.begin() == it)
	//	//	break;
	//}
}


//void CDlgScore::DoDataExchange(CDataExchange* pDX)
//{
//	CDialog::DoDataExchange(pDX);
//	//{{AFX_DATA_MAP(CDlgScore)
//	DDX_Control(pDX, IDLC_SCORE_INFO, m_lcScoreInfo);
//	DDX_Control(pDX, IDLC_SCORE, m_lcScore);
//	//}}AFX_DATA_MAP
//}


//BEGIN_MESSAGE_MAP(CDlgScore, CDialog)
//	//{{AFX_MSG_MAP(CDlgScore)
//	//}}AFX_MSG_MAP
//END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgScore message handlers

//BOOL CDlgScore::OnInitDialog() 
LRESULT CDlgScore::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	//CDialog::OnInitDialog();
	CenterWindow(GetParent());

	// set icons
	HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME),
		IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME),
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);

	// First DDX call, hooks up variables to controls.
	DoDataExchange(false);

	//ÿ�ֵ���Ϣ
	m_lcScore.SetBkColor(RGB(0, 128, 0));
	m_lcScore.SetTextBkColor(RGB(0, 128, 0));
	m_lcScore.SetTextColor(RGB(218, 218, 218));

	int charWid = m_lcScore.GetStringWidth(TEXT("9"));
	m_lcScore.InsertColumn(0, TEXT("���"), LVCFMT_RIGHT, charWid * (13 + 2));	// ���ַ�ʽ����0�ж��벻��
	m_lcScore.InsertColumn(1, TEXT("�ƾ�"), LVCFMT_RIGHT, charWid * (13 + 2));
	m_lcScore.InsertColumn(2, TEXT("����"), LVCFMT_RIGHT, charWid * (5 + 2));
	m_lcScore.InsertColumn(3, TEXT("״̬"), LVCFMT_RIGHT, charWid * (5 + 2));
	m_lcScore.InsertColumn(4, TEXT("��ʱ"), LVCFMT_LEFT, charWid * (17 + 2));
	m_lcScore.InsertColumn(5, TEXT("ʱ��"), LVCFMT_LEFT, charWid * (43 + 2));

	// ����0�ж��룬��Ҫʹ�����ַ�ʽ
	LVCOLUMN colTest;
	colTest.mask = LVCF_FMT;
	m_lcScore.GetColumn(0, &colTest);
	colTest.fmt = LVCFMT_RIGHT;
	m_lcScore.SetColumn(0, &colTest);
	// ����0�ж��룬��Ҫʹ�����ַ�ʽ

	int nTotal = 0;
	int nPassed = 0;
	int nGiveUp = 0;
	int nDead = 0;
	int nStepsPassed = 0;

	//CString s;	//CString.Format ��֧�� %f
	ATL::CString s;		//ATL::CString.Format ֧�� %f
	ATL::CString st;
	int i = 0;
	int nSize = m_score.m_tList.size();

	// list<CMyObject*> m_score;
	//for (POSITION pos = m_score.GetTailPosition(); pos != NULL; ++i )
	//for (list<CScore>::iterator it = m_score.m_tList.end(); it != m_score.m_tList.begin(); i++)
	//ս�������һ�ֿ�ʼ��ʾ
	for (list<CScore>::iterator it = m_score.m_tList.begin(); ; i++)
	{
		// m_score.m_tList.begin()��ŵ����һ�ֻ�û�н⣬����״̬������ʾ
		++it;	//	�����������������б�����ֹ������ m_score.m_tList.begin()
		if (m_score.m_tList.end() == it)
			break;

		//it--;	//	�����������������б�����ֹ��COblist �� STL list�Ĳ�ͬ
		////if(pos == m_score.m_tList.GetHeadPosition()) break;
		//if (m_score.m_tList.begin() == it)
		//	break;

		//CScore *p = (CScore*)m_score.m_tList.GetPrev(pos);
		CScore& cs = *it;	// it = m_score.m_tList.end()��������it--������*it����������ʱ������ֹ

		s.Format(TEXT("%d"), nSize - (i + 1));	// ��ǰһ��û��ս����Ϣ������Ҫ���1
		m_lcScore.InsertItem(i, s);

		s.Format(TEXT("%d"), cs.gameNumber);
		m_lcScore.SetItemText(i, 1, s);

		s.Format(TEXT("%d"), cs.steps);
		m_lcScore.SetItemText(i, 2, s);

		m_lcScore.SetItemText(i, 3, statusStr[cs.gameStatus]);	// �þ䱨����ֹ

		CTimeSpan ts = cs.tmEnd - cs.tmStart;	// #include <atltime.h>
		//s.Format(TEXT("%2dm%2ds"), ts.GetMinutes(), ts.GetSeconds());

		// �������ַ�ʽ������ȷ��ȡ��������ATL::CString.Format������ë��
		//s.Format(TEXT("%2dD%2dh%2dm%2ds"), ts.GetDays(), ts.GetHours(), ts.GetMinutes(), ts.GetSeconds());

		// ���밴�������ķ�ʽ������ȷ��ȡ�����򲻶ԡ�������ATL::CString.Format������ë��
		s.Format(TEXT("%3dD"), (int)ts.GetDays());
		st.Format(TEXT("%2dH - "), ts.GetHours());
		s += st;
		st.Format(TEXT("%2dm%2ds"), ts.GetMinutes(), ts.GetSeconds());
		s += st;
		m_lcScore.SetItemText(i, 4, s);

		m_lcScore.SetItemText(i, 5,
			cs.tmStart.Format(fmtsYMD) + TEXT(" / ") + cs.tmEnd.Format(fmtsYMD));

		++nTotal;

		if (cs.gameStatus == CScore::gamePassed) {
			++nPassed;
			nStepsPassed += cs.steps;
		}
		else if (cs.gameStatus == CScore::gameGiveUp) {
			++nGiveUp;
		}
		else if (cs.gameStatus == CScore::gameDead) {
			++nDead;
		}
	}

	//ͳ��ս��
	m_lcScoreInfo.SetBkColor(RGB(0, 128, 0));
	m_lcScoreInfo.SetTextBkColor(RGB(0, 128, 0));
	m_lcScoreInfo.SetTextColor(RGB(218, 218, 218));

	// �ÿո����\t����������2���ش�	listbox����������������ȡ���������ݻ�����ȷ�ġ�
	m_lcScoreInfo.InsertColumn(0, TEXT("��Ŀ"), LVCFMT_LEFT, charWid * 13);
	m_lcScoreInfo.InsertColumn(1, TEXT("ͳ��"), LVCFMT_LEFT, charWid * 15);
	m_lcScoreInfo.InsertItem(0, TEXT("�ܼ�(��) ��"));
	m_lcScoreInfo.InsertItem(1, TEXT("ͨ��(��) ��"));
	m_lcScoreInfo.InsertItem(2, TEXT("ʤ��     ��"));
	m_lcScoreInfo.InsertItem(3, TEXT("��ͨһ�� ��"));
	m_lcScoreInfo.InsertItem(4, TEXT("����(��) ��"));
	m_lcScoreInfo.InsertItem(5, TEXT("����(��) ��"));

	s.Format(TEXT("%d"), nTotal);
	m_lcScoreInfo.SetItemText(0, 1, s);

	s.Format(TEXT("%d"), nPassed);
	m_lcScoreInfo.SetItemText(1, 1, s);

	if (nTotal > 0)
	{
		s.Format(TEXT("%3.1f%%"), 100.*nPassed / nTotal);
		m_lcScoreInfo.SetItemText(2, 1, s);
	}
	else
		m_lcScoreInfo.SetItemText(2, 1, L"0");

	if (nPassed > 0)
	{
		s.Format(TEXT("ƽ��%3.1f��"), float(nStepsPassed) / nPassed);
		m_lcScoreInfo.SetItemText(3, 1, s);
	}
	else
		m_lcScoreInfo.SetItemText(3, 1, L"0");

	s.Format(TEXT("%d"), nGiveUp);
	m_lcScoreInfo.SetItemText(4, 1, s);

	s.Format(TEXT("%d"), nDead);
	m_lcScoreInfo.SetItemText(5, 1, s);

	// ����ʵ��ս����������ʱ���ؼ��Զ�

	//CRect rect1, rect2, rect3;
	//GetClientRect(&rect1);		// ȡ�ͻ�����С  

	//m_lcScoreInfo.GetWindowRect(&rect2);
	//ScreenToClient(rect2);

	//m_lcScore.GetWindowRect(&rect3);
	//ScreenToClient(rect3);

	//+ rect1{ top = 0 bottom = 495 left = 0 right = 943 }	WTL::CRect
	//+ rect2{ top = 0 bottom = 495 left = 0 right = 259 }	WTL::CRect
	//+ rect3{ top = 0 bottom = 495 left = 266 right = 941 }	WTL::CRect


	CRect rect;
	m_lcScore.GetWindowRect(&rect);
	m_nInterval = rect.left;
	m_lcScoreInfo.GetWindowRect(&rect);
	m_nInterval -= rect.right;	// �õ�m_lcScoreInfo��m_lcScore �ؼ�֮��ļ�����

	m_dScale = rect.Width();		// �õ� m_lcScoreInfo �Ĵ��ڿ��
	GetClientRect(&rect);		// ȡ�ͻ�����С  
	m_dScale /= rect.Width();	// �õ� m_lcScore �ؼ����ռ���ڿͻ�����ȵı���

	// ����ʵ��ս����������ʱ���ؼ��Զ�

	m_sortstyl = 1;
	m_selectCol = -1;

	return TRUE;
}

//����ս��
void CDlgScore::UpdateScore()
{
	if (m_score.m_tList.empty()) return;

	//CScore *p = (CScore*)m_score.m_tList.GetHead();
	CScore& cs = m_score.m_tList.front();
	cs.tmEnd = CTime::GetCurrentTime();

	//CString str = cs.tmStart.Format(fmts) + TEXT("/") + cs.tmEnd.Format(fmts);
	//::MessageBox(NULL, str, L"", NULL);

	//ʮ�������ڽ⿪�ľ���Ϊ��ʹ�����Զ����İ������������¼
	if ((cs.tmEnd - cs.tmStart).GetTotalSeconds() < 15) {
		//m_score.RemoveHead();
		m_score.m_tList.pop_front();
		//delete p;
		return;
	}

	//CJLDoc *pDoc = AfxGetDocument();
	cs.gameStatus =
		g_fcData.GameOver() ? CScore::gamePassed :
		g_fcData.m_Hints.IsEmpty() ? CScore::gameDead :
		CScore::gameGiveUp;
	cs.steps = g_fcData.m_OpsList.m_tList.size();
}

void CDlgScore::InitScore()
{
	//CScore *p = new CScore;
	//CScore *p = new CScore;
	//ATLASSERT(p);
	m_score.m_tList.push_front(CScore());

	//CJLDoc *pDoc = AfxGetDocument();

	// STL list front() ���ص�һ��Ԫ�ص����ã�back() �������һԪ�ص�����
	m_score.m_tList.front().gameNumber = g_fcData.m_nCurGameNumber;
	m_score.m_tList.front().tmStart = CTime::GetCurrentTime();
}

//�����˾��Ƿ��������
bool CDlgScore::IsOldGameNumber(int gameNum)
{
	//for(POSITION pos = m_score.GetHeadPosition(); pos != NULL; )
	//	if(gameNum == ((CScore*)m_score.GetNext(pos))->gameNumber)
	//		return true;
	for (list<CScore>::iterator it = m_score.m_tList.begin(); it != m_score.m_tList.end(); ++it)
	{
		if (gameNum == (*it).gameNumber)
			return true;
	}

	return false;
}

LRESULT CDlgScore::OnOKCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}

LRESULT CDlgScore::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// �������2�д��룬�����2�δ�ս���Ի��򱨴���ֹ����
	m_lcScore.Detach();
	m_lcScoreInfo.Detach();

	EndDialog(wID);
	return 0;
}

LRESULT CDlgScore::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//CDialog::OnSizing(fwSide, pRect);

	// TODO: Add your message handler code here
	//CRect r;
	//GetClientRect(r);
	//MoveWindow(r);

	resize();

	return 0;
}

void CDlgScore::resize()
{
	CRect rectc;
	GetClientRect(&rectc);     //ȡ�ͻ�����С  

	CRect rect(0, 0, int(rectc.right * m_dScale), rectc.bottom);
	::MoveWindow(m_lcScoreInfo.m_hWnd, rect.left, rect.top, rect.Width(), rect.Height(), TRUE);	// �ƶ��ؼ�

	rect.left = rect.right + m_nInterval;
	rect.top = 0;
	rect.right = rectc.right - 2;
	rect.bottom = rectc.bottom;
	::MoveWindow(m_lcScore.m_hWnd, rect.left, rect.top, rect.Width(), rect.Height(), TRUE);		// �ƶ��ؼ�

}

//-------------------------------------------------------------------
//�����б���б�ͷ����
LRESULT CDlgScore::OnColumnClick(int/*idCtrl*/, LPNMHDR pnmh, BOOL&/*bHandled*/)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pnmh;
	//NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	//MessageBox(L"Test");

	/*
	VC CListCtrl�ؼ�����������  2009-09-24 14:26:07|  ���ࣺ VC���� |  ��ǩ�� |�ֺŴ���С ����
	http://alifens198345.blog.163.com/blog/static/11679917720098242267804/
	���Ϻܶ඼�ǵ������Լ�д�Ļص�����������ģ��������ֱ����Ӧ��������Ϣ���������򣬱���Ҳ�������ҵ�һ�δ��룡
	����ֻ�����ַ���������������ַ������֣����硰10������1����2����3����11�������ִ�С����Ͳ���ȷ�ˣ�
	��ʾΪ1��10��11��2��3��˳�򣡵���ʵ��˳��Ӧ����1��2��3��10��11�����Լ��޸���һ�£�
	�����ַ������ַ������ֶ�����ȷ������!ֻҪ������Ĵ�������OnColumnclickList()������Ϳ����ˣ�

	2010-09-03 11:37sd
	double dbStrA = atof(strA);//����ط�������(����strA��CString����)���ţ����Լ�������һ�£�
	˳��˵һ�䣬���õ���visual studio 2010��лл�ظ�
	2010-11-14 22:54�и������������ �ظ� sd
	atof((LPSTR)(LPCTSTR)strA);
	��������ͬ,����Ҳ��ͬ,�Լ�תһ�¾ͺ���.���ܷ��¿�����.�ظ�
	*/

	//SetRedraw(FALSE);

	int selectCol = pNMListView->iSubItem;
	int listRows, listColumns, i, j, n;
	//DWORD ddA, ddB;

	if (m_selectCol != selectCol)	// �������ϴε����ı�ͷ�У�����������Ϊ����
		m_sortstyl = 1;

	m_selectCol = selectCol;			// ���浥���ı�ͷ��

	//ȡ�ÿؼ�������������
	//listRows = GetItemCount() - m_nTreeRec;
	listRows = m_score.m_tList.size() - 1;	// ��ǰһ��û��ս����Ϣ������Ҫ���1
	listColumns = m_lcScore.GetHeader().GetItemCount();
	CString strA, strB;
	double dbStrA, dbStrB;
	wchar_t *wpSrc, *wpEnd;

	//m_bSortStatusOfLV = (100 + selectCol) * m_sortstyl;
	//if (g_pMainFrame->m_bSaveSortStatusOfLV)
	//{
	//	g_pMainFrame->m_bSaveSortStatusOfLV = m_bSortStatusOfLV;
	//	WritePrivateProfileStringW(L"Main", L"SaveSortStatusOfLV", itos(m_bSortStatusOfLV), g_pMainFrame->m_wzIniName);
	//}

	//���캯����m_sortstyl = 1��ÿ�θı�Ŀ¼���ʼ����������Ϊ��������m_sortstyl = 1
	//m_sortstyl == 1 �������У�m_sortstyl == -1 �������С�ÿ������󶼷�ת��������
	if (m_sortstyl == 1)
	{
		for (j = 1; j <= listRows; j++)
		{
			for (i = 0; i < listRows - j; i++)
			{
				m_lcScore.GetItemText(i, selectCol, strA.GetBuffer(255), 255); //m_List2Ϊ�ؼ��󶨵ı����������Լ����޸�
				m_lcScore.GetItemText(i + 1, selectCol, strB.GetBuffer(255), 255);
				strA.ReleaseBuffer();
				strB.ReleaseBuffer();

				wpSrc = strA.GetBuffer(strA.GetLength());	//����WTL�У�m_lcScore.GetItemTextʹ��CString��㣬�������ﻹ����strA
				dbStrA = wcstod(wpSrc, &wpEnd);
				strA.ReleaseBuffer();

				wpSrc = strB.GetBuffer(strB.GetLength());
				dbStrB = wcstod(wpSrc, &wpEnd);
				strB.ReleaseBuffer();

				//���ݷ���ֵ�ж����ַ��������ַ�������
				//if (dbStrA != dbStrB && dbStrA != 0 && dbStrB != 0)		// �����ַ�������
				if (fabs(dbStrA - dbStrB) > 0 && fabs(dbStrA) > 0 && fabs(dbStrB) > 0)		// �����ַ�������
				{
					if (dbStrA > dbStrB)
					{
						for (n = 0; n < listColumns; n++)
						{
							m_lcScore.GetItemText(i, n, strA.GetBuffer(255), 255);
							m_lcScore.GetItemText(i + 1, n, strB.GetBuffer(255), 255);
							strA.ReleaseBuffer();
							strB.ReleaseBuffer();

							m_lcScore.SetItemText(i, n, strB);
							m_lcScore.SetItemText(i + 1, n, strA);
						}

						//�����ڼ�¼���������е�nRecNOֻ�轻��1��
						//ddA = GetItemData(i);
						//ddB = GetItemData(i + 1);
						//SetItemData(i, ddB);
						//SetItemData(i + 1, ddA);

						//m_lcScore.SetItemText(m, 2, itos(ddB));		//�����ã�Show Data
						//m_lcScore.SetItemText(m + 1, 2, itos(ddA));	//�����ã�Show Data
					}
				}
				else
				{
					if (strA > strB)							//�����ַ���
					{
						for (n = 0; n < listColumns; n++)		//12Ϊ��ؼ����е������������Լ���ʵ������޸�
						{
							m_lcScore.GetItemText(i, n, strA.GetBuffer(255), 255);
							m_lcScore.GetItemText(i + 1, n, strB.GetBuffer(255), 255);
							strA.ReleaseBuffer();
							strB.ReleaseBuffer();

							m_lcScore.SetItemText(i, n, strB);
							m_lcScore.SetItemText(i + 1, n, strA);
						}

						//�����ڼ�¼���������е�iRecNOֻ�轻��1��
						//ddA = GetItemData(i);
						//ddB = GetItemData(i + 1);
						//SetItemData(i, ddB);
						//SetItemData(i + 1, ddA);

						//m_lcScore.SetItemText(m, 2, itos(ddB));		//�����ã�Show Data
						//m_lcScore.SetItemText(m + 1, 2, itos(ddA));	//�����ã�Show Data
					}
				}

			}
		}
	}
	//���캯����m_sortstyl = 1��ÿ�θı�Ŀ¼���ʼ����������Ϊ��������m_sortstyl = 1
	//m_sortstyl == 1 �������У�m_sortstyl == -1 �������С�ÿ������󶼷�ת��������
	else if (m_sortstyl == -1)
	{
		for (j = 1; j <= listRows; j++)
		{
			for (i = 0; i < listRows - j; i++)
			{
				m_lcScore.GetItemText(i, selectCol, strA.GetBuffer(255), 255);
				m_lcScore.GetItemText(i + 1, selectCol, strB.GetBuffer(255), 255);
				strA.ReleaseBuffer();
				strB.ReleaseBuffer();

				wpSrc = strA.GetBuffer(strA.GetLength());
				dbStrA = wcstod(wpSrc, &wpEnd);
				strA.ReleaseBuffer();

				wpSrc = strB.GetBuffer(strB.GetLength());
				dbStrB = wcstod(wpSrc, &wpEnd);
				strB.ReleaseBuffer();

				//if (dbStrA != dbStrB && dbStrA != 0 && dbStrB != 0)		// �����ַ�������
				if (fabs(dbStrA - dbStrB) > 0 && fabs(dbStrA) > 0 && fabs(dbStrB) > 0)		// �����ַ�������
				{
					if (dbStrB > dbStrA)
					{
						for (n = 0; n < listColumns; n++)
						{
							m_lcScore.GetItemText(i, n, strA.GetBuffer(255), 255);
							m_lcScore.GetItemText(i + 1, n, strB.GetBuffer(255), 255);
							strA.ReleaseBuffer();
							strB.ReleaseBuffer();

							m_lcScore.SetItemText(i, n, strB);
							m_lcScore.SetItemText(i + 1, n, strA);
						}

						//�����ڼ�¼���������е�iRecNOֻ�轻��1��
						//ddA = GetItemData(i);
						//ddB = GetItemData(i + 1);
						//SetItemData(i, ddB);
						//SetItemData(i + 1, ddA);

						//m_lcScore.SetItemText(m, 2, itos(ddB));		//�����ã�Show Data
						//m_lcScore.SetItemText(m + 1, 2, itos(ddA));	//�����ã�Show Data
					}
				}
				else
				{
					if (strB > strA)
					{
						for (n = 0; n < listColumns; n++)
						{
							m_lcScore.GetItemText(i, n, strA.GetBuffer(255), 255);
							m_lcScore.GetItemText(i + 1, n, strB.GetBuffer(255), 255);
							strA.ReleaseBuffer();
							strB.ReleaseBuffer();

							m_lcScore.SetItemText(i, n, strB);
							m_lcScore.SetItemText(i + 1, n, strA);
						}

						//�����ڼ�¼���������е�iRecNOֻ�轻��1��
						//ddA = GetItemData(i);
						//ddB = GetItemData(i + 1);
						//SetItemData(i, ddB);
						//SetItemData(i + 1, ddA);

						//m_lcScore.SetItemText(m, 2, itos(ddB));		//�����ã�Show Data
						//m_lcScore.SetItemText(m + 1, 2, itos(ddA));	//�����ã�Show Data
					}
				}
			}
		}
	}

	//���캯����m_sortstyl = 1��ÿ�θı�Ŀ¼���ʼ����������Ϊ��������m_sortstyl = 1
	//m_sortstyl == 1 �������У�m_sortstyl == -1 �������С�ÿ������󶼷�ת��������
	m_sortstyl = m_sortstyl * (-1);

	//ÿ��������Զ��������к�
	//for (j = 0; j < listRows; j++)
	//	m_lcScore.SetItemText(j, 1, itos(j + 1));	//Set Number

	//SetRedraw(TRUE);
	//CRect rect;
	//::GetWindowRect(m_lcScore.m_hWnd, &rect);
	//rect.top -= 8;
	//InvalidateRect(rect);
	//m_lcScore.Invalidate();
	//Invalidate();

	//*pResult = 0;


	/*
	EnsureVisible---���øú�����ȷ��ָ�����б���ͼ��Ŀ�ɼ�����ԭ��Ϊ��
	BOOL EnsureVisible��int nItem,BOOL bPartialOK��
	���� ����ֵ��Nonzero if successful; otherwise zero.
	   ���� nItem��ָ���˱���ɼ�����Ŀ������
		  ���� bPartialOK��ָ�����Ƿ������ֿɼ���
			 ���� �б���ͼ�ؼ����ڱ�Ҫʱ���й�������ȷ��ָ����Ŀ�ɼ������bPartialOK����Ϊ����ֵ��
				����Ŀ���ֿɼ�ʱ���б���ͼ�ؼ������й�����

				ע�⣺���ListViewָ��LVS_NOSCROLL,����Scroll��EnsureVisible����FALSE
				*/

				/*
				1������CListCtrlѡ����

				m_list.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);

				ע�⣺������ֻ����ָ���и�����ʾ�����ʹ�ú�����GetSelectionMark�������õ�ѡ���У���������Ǵ���ġ�
				���磬���������ѡ�е�5�У�����GetSelectionMark�����õ�����4(��5��)���ٵ���SetItemState����ѡ�е�2�У�
				Ȼ�����GetSelectionMark�����õ��Ļ���4(��5��)�����ԣ���Ҫ������ѡ���и�����ʾʱ��һ����SetSelectionMark�������ã�
				m_list.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,LVIS_FOCUSED | LVIS_SELECTED);   //ѡ����
				m_list.SetSelectionMark(nIndexs);

				*/

	return 0L;
}

//-------------------------------------------------------------------
//�����б���б�ͷ����
//LRESULT CMainFrame::OnLVItemDblclk(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
//LRESULT CMainFrame::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult)
//LRESULT CRMlistView::OnColumnClick(int/*idCtrl*/, LPNMHDR pnmh, BOOL&/*bHandled*/)
//{
//	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pnmh;
//	//NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
//
//	/*
//	VC CListCtrl�ؼ�����������  2009-09-24 14:26:07|  ���ࣺ VC���� |  ��ǩ�� |�ֺŴ���С ����
//	http://alifens198345.blog.163.com/blog/static/11679917720098242267804/
//	���Ϻܶ඼�ǵ������Լ�д�Ļص�����������ģ��������ֱ����Ӧ��������Ϣ���������򣬱���Ҳ�������ҵ�һ�δ��룡
//	����ֻ�����ַ���������������ַ������֣����硰10������1����2����3����11�������ִ�С����Ͳ���ȷ�ˣ�
//	��ʾΪ1��10��11��2��3��˳�򣡵���ʵ��˳��Ӧ����1��2��3��10��11�����Լ��޸���һ�£�
//	�����ַ������ַ������ֶ�����ȷ������!ֻҪ������Ĵ�������OnColumnclickList()������Ϳ����ˣ�
//
//	2010-09-03 11:37sd
//	double dbStrA = atof(strA);//����ط�������(����strA��CString����)���ţ����Լ�������һ�£�
//	˳��˵һ�䣬���õ���visual studio 2010��лл�ظ�
//	2010-11-14 22:54�и������������ �ظ� sd
//	atof((LPSTR)(LPCTSTR)strA);
//	��������ͬ,����Ҳ��ͬ,�Լ�תһ�¾ͺ���.���ܷ��¿�����.�ظ�
//	*/
//
//	SetRedraw(FALSE);
//
//	int selectCol = pNMListView->iSubItem;
//	int listRows, listColumns, i, j, m, n;
//	DWORD ddA, ddB;
//
//	//ȡ�ÿؼ�������������
//	listRows = GetItemCount() - m_nTreeRec;
//	listColumns = GetHeader().GetItemCount();
//	CString strA, strB;
//	double dbStrA, dbStrB;
//	wchar_t *wpSrc, *wpEnd;
//
//	m_bSortStatusOfLV = (100 + selectCol) * m_sortstyl;
//	if (g_pMainFrame->m_bSaveSortStatusOfLV)
//	{
//		g_pMainFrame->m_bSaveSortStatusOfLV = m_bSortStatusOfLV;
//		WritePrivateProfileStringW(L"Main", L"SaveSortStatusOfLV", itos(m_bSortStatusOfLV), g_pMainFrame->m_wzIniName);
//	}
//
//	//���캯����m_sortstyl = 1��ÿ�θı�Ŀ¼���ʼ����������Ϊ��������m_sortstyl = 1
//	//m_sortstyl == 1 �������У�m_sortstyl == -1 �������С�ÿ������󶼷�ת��������
//	if (m_sortstyl == 1)
//	{
//		for (j = 1; j <= listRows; j++)
//		{
//			for (i = 0; i < listRows - j; i++)
//			{
//				m = i + m_nTreeRec;
//				GetItemText(m, selectCol, strA); //m_List2Ϊ�ؼ��󶨵ı����������Լ����޸�
//				GetItemText(m + 1, selectCol, strB);
//
//				wpSrc = strA.GetBuffer(strA.GetLength());	//����WTL�У�GetItemTextʹ��CString��㣬�������ﻹ����strA
//				dbStrA = wcstod(wpSrc, &wpEnd);
//				strA.ReleaseBuffer();
//
//				wpSrc = strB.GetBuffer(strB.GetLength());
//				dbStrB = wcstod(wpSrc, &wpEnd);
//				strB.ReleaseBuffer();
//
//				//���ݷ���ֵ�ж����ַ��������ַ�������
//				if (dbStrA != dbStrB && dbStrA != 0 && dbStrB != 0)		//�����ַ�������
//				{
//					if (dbStrA > dbStrB)
//					{
//						for (n = 0; n < listColumns; n++)
//						{
//							GetItemText(m, n, strA);
//							GetItemText(m + 1, n, strB);
//							SetItemText(m, n, strB);
//							SetItemText(m + 1, n, strA);
//						}
//						//�����ڼ�¼���������е�nRecNOֻ�轻��1��
//						ddA = GetItemData(m);
//						ddB = GetItemData(m + 1);
//						SetItemData(m, ddB);
//						SetItemData(m + 1, ddA);
//
//						//SetItemText(m, 2, itos(ddB));		//�����ã�Show Data
//						//SetItemText(m + 1, 2, itos(ddA));	//�����ã�Show Data
//					}
//				}
//				else
//				{
//					if (strA > strB)							//�����ַ���
//					{
//						for (n = 0; n < listColumns; n++)		//12Ϊ��ؼ����е������������Լ���ʵ������޸�
//						{
//							GetItemText(m, n, strA);
//							GetItemText(m + 1, n, strB);
//							SetItemText(m, n, strB);
//							SetItemText(m + 1, n, strA);
//						}
//						//�����ڼ�¼���������е�iRecNOֻ�轻��1��
//						ddA = GetItemData(m);
//						ddB = GetItemData(m + 1);
//						SetItemData(m, ddB);
//						SetItemData(m + 1, ddA);
//
//						//SetItemText(m, 2, itos(ddB));		//�����ã�Show Data
//						//SetItemText(m + 1, 2, itos(ddA));	//�����ã�Show Data
//					}
//				}
//
//			}
//		}
//	}
//	//���캯����m_sortstyl = 1��ÿ�θı�Ŀ¼���ʼ����������Ϊ��������m_sortstyl = 1
//	//m_sortstyl == 1 �������У�m_sortstyl == -1 �������С�ÿ������󶼷�ת��������
//	else if (m_sortstyl == -1)
//	{
//		for (j = 1; j <= listRows; j++)
//		{
//			for (i = 0; i < listRows - j; i++)
//			{
//				m = i + m_nTreeRec;
//				GetItemText(m, selectCol, strA);
//				GetItemText(m + 1, selectCol, strB);
//
//				wpSrc = strA.GetBuffer(strA.GetLength());
//				dbStrA = wcstod(wpSrc, &wpEnd);
//				strA.ReleaseBuffer();
//
//				wpSrc = strB.GetBuffer(strB.GetLength());
//				dbStrB = wcstod(wpSrc, &wpEnd);
//				strB.ReleaseBuffer();
//
//				if (dbStrA != dbStrB && dbStrA != 0 && dbStrB != 0)
//				{
//					if (dbStrB > dbStrA)
//					{
//						for (n = 0; n < listColumns; n++)
//						{
//							GetItemText(m, n, strA);
//							GetItemText(m + 1, n, strB);
//							SetItemText(m, n, strB);
//							SetItemText(m + 1, n, strA);
//						}
//						//�����ڼ�¼���������е�iRecNOֻ�轻��1��
//						ddA = GetItemData(m);
//						ddB = GetItemData(m + 1);
//						SetItemData(m, ddB);
//						SetItemData(m + 1, ddA);
//
//						//SetItemText(m, 2, itos(ddB));		//�����ã�Show Data
//						//SetItemText(m + 1, 2, itos(ddA));	//�����ã�Show Data
//					}
//				}
//				else
//				{
//					if (strB > strA)
//					{
//						for (n = 0; n < listColumns; n++)
//						{
//							GetItemText(m, n, strA);
//							GetItemText(m + 1, n, strB);
//							SetItemText(m, n, strB);
//							SetItemText(m + 1, n, strA);
//						}
//						//�����ڼ�¼���������е�iRecNOֻ�轻��1��
//						ddA = GetItemData(m);
//						ddB = GetItemData(m + 1);
//						SetItemData(m, ddB);
//						SetItemData(m + 1, ddA);
//
//						//SetItemText(m, 2, itos(ddB));		//�����ã�Show Data
//						//SetItemText(m + 1, 2, itos(ddA));	//�����ã�Show Data
//					}
//				}
//			}
//		}
//	}
//
//	//���캯����m_sortstyl = 1��ÿ�θı�Ŀ¼���ʼ����������Ϊ��������m_sortstyl = 1
//	//m_sortstyl == 1 �������У�m_sortstyl == -1 �������С�ÿ������󶼷�ת��������
//	m_sortstyl = m_sortstyl * (-1);
//
//	//ÿ��������Զ��������к�
//	for (j = 0; j < listRows; j++)
//		SetItemText(j + m_nTreeRec, 1, itos(j + 1));	//Set Number
//
//	SetRedraw(TRUE);
//
//	//*pResult = 0;
//
//
//	/*
//	EnsureVisible---���øú�����ȷ��ָ�����б���ͼ��Ŀ�ɼ�����ԭ��Ϊ��
//	BOOL EnsureVisible��int nItem,BOOL bPartialOK��
//	���� ����ֵ��Nonzero if successful; otherwise zero.
//	   ���� nItem��ָ���˱���ɼ�����Ŀ������
//		  ���� bPartialOK��ָ�����Ƿ������ֿɼ���
//			 ���� �б���ͼ�ؼ����ڱ�Ҫʱ���й�������ȷ��ָ����Ŀ�ɼ������bPartialOK����Ϊ����ֵ��
//				����Ŀ���ֿɼ�ʱ���б���ͼ�ؼ������й�����
//
//				ע�⣺���ListViewָ��LVS_NOSCROLL,����Scroll��EnsureVisible����FALSE
//				*/
//
//	/*
//	1������CListCtrlѡ����
//
//	m_list.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
//
//	ע�⣺������ֻ����ָ���и�����ʾ�����ʹ�ú�����GetSelectionMark�������õ�ѡ���У���������Ǵ���ġ�
//	���磬���������ѡ�е�5�У�����GetSelectionMark�����õ�����4(��5��)���ٵ���SetItemState����ѡ�е�2�У�
//	Ȼ�����GetSelectionMark�����õ��Ļ���4(��5��)�����ԣ���Ҫ������ѡ���и�����ʾʱ��һ����SetSelectionMark�������ã�
//	m_list.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,LVIS_FOCUSED | LVIS_SELECTED);   //ѡ����
//	m_list.SetSelectionMark(nIndexs);
//
//	*/
//
//	return 0L;
//}
