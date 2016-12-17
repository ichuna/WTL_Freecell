// DlgScore.cpp : implementation file
//

#include "stdafx.h"
//#include "JL.h"
//#include "JLDoc.h"
#include "DlgScore.h"

//extern CJLDoc * AfxGetDocument();

static const wchar_t * statusStr[4] = { TEXT("δ��"), TEXT("ͨ��"), TEXT("����"), TEXT("����") };
static const wchar_t * fmts = TEXT("%H:%M:%S");

/////////////////////////////////////////////////////////////////////////////
// CDlgScore dialog

//CDlgScore::CDlgScore(CWnd* pParent /*=NULL*/)
//	: CDialog(CDlgScore::IDD, pParent)
//{
//	//{{AFX_DATA_INIT(CDlgScore)
//	//}}AFX_DATA_INIT
//}

CDlgScore::CDlgScore()
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

	// First DDX call, hooks up variables to controls.
	DoDataExchange(false);

	//ÿ�ֵ���Ϣ
	m_lcScore.SetBkColor(RGB(0, 128, 0));
	m_lcScore.SetTextBkColor(RGB(0, 128, 0));
	m_lcScore.SetTextColor(RGB(218, 218, 218));

	int charWid = m_lcScore.GetStringWidth(TEXT("9"));
	m_lcScore.InsertColumn(0, TEXT("�ƾ�"), LVCFMT_LEFT, charWid * (11 + 2));
	m_lcScore.InsertColumn(1, TEXT("����"), LVCFMT_LEFT, charWid * (4 + 2));
	m_lcScore.InsertColumn(2, TEXT("״̬"), LVCFMT_LEFT, charWid * (4 + 2));
	m_lcScore.InsertColumn(3, TEXT("��ʱ"), LVCFMT_LEFT, charWid * (8 + 2));
	m_lcScore.InsertColumn(4, TEXT("ʱ��"), LVCFMT_LEFT, charWid * (20 + 2));

	int nTotal = 0;
	int nPassed = 0;
	int nGiveUp = 0;
	int nDead = 0;
	int nStepsPassed = 0;

	//WTL::CString s;	//WTL::CString.Format ��֧�� %f
	ATL::CString s;		//ATL::CString.Format ֧�� %f
	int i = 0;

	// list<CMyObject*> m_score;
	//for (POSITION pos = m_score.GetTailPosition(); pos != NULL; ++i )
	for (list<CScore>::iterator it = m_score.m_tList.end(); it != m_score.m_tList.begin(); i++)
	{
		//���һ�ֻ�û�н⣬����״̬������ʾ
		it--;	//	�����������������б�����ֹ��COblist �� STL list�Ĳ�ͬ
		//if(pos == m_score.m_tList.GetHeadPosition()) break;
		if (m_score.m_tList.begin() == it)
			break;

		//CScore *p = (CScore*)m_score.m_tList.GetPrev(pos);
		CScore& cs = *it;	// it = m_score.m_tList.end()��������it--������*it����������ʱ������ֹ

		s.Format(TEXT("%d"), cs.gameNumber);
		m_lcScore.InsertItem(i, s);

		s.Format(TEXT("%d"), cs.steps);
		m_lcScore.SetItemText(i, 1, s);

		m_lcScore.SetItemText(i, 2, statusStr[cs.gameStatus]);	// �þ䱨����ֹ

		CTimeSpan ts = cs.tmEnd - cs.tmStart;	// #include <atltime.h>
		s.Format(TEXT("%2dm%2ds"), ts.GetMinutes(), ts.GetSeconds());
		m_lcScore.SetItemText(i, 3, s);

		m_lcScore.SetItemText(i, 4,
			cs.tmStart.Format(fmts) + TEXT("/") + cs.tmEnd.Format(fmts));

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

	return TRUE;
}

//����ս��
void CDlgScore::UpdateScore()
{
	if (m_score.m_tList.empty()) return;

	//CScore *p = (CScore*)m_score.m_tList.GetHead();
	CScore& cs = m_score.m_tList.front();
	cs.tmEnd = CTime::GetCurrentTime();

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
	for (list<CScore>::iterator it = m_score.m_tList.begin(); it != m_score.m_tList.begin(); it++)
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
