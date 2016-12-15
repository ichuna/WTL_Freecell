// DlgScore.cpp : implementation file
//

#include "stdafx.h"
//#include "JL.h"
//#include "JLDoc.h"
#include "DlgScore.h"
#include <atltime.h>

//extern CJLDoc * AfxGetDocument();

class CScore : public CMyObject
{
public:
	int gameNumber;
	int steps;
	CTime tmStart, tmEnd;
	enum { gameInit = 0, gamePassed, gameGiveUp, gameDead } gameStatus;
};
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

	for (list<CMyObject*>::iterator it = m_score.end(); it != m_score.begin();)
	{
		//���һ�ֻ�û�н⣬����״̬������ʾ
		it--;	//	�����������������б�����ֹ��COblist �� STL list�Ĳ�ͬ

		//CScore *p = (CScore*)m_score.GetPrev(pos);
		CScore *pScore = (CScore*)(*it);	// it = m_score.end()��������it--������*it����������ʱ������ֹ
		delete pScore;

		//if(pos == m_score.GetHeadPosition()) break;
		//if (m_score.begin() == it)
		//	break;
	}
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
	for (list<CMyObject*>::iterator it = m_score.end(); it != m_score.begin(); i++)
	{
		//���һ�ֻ�û�н⣬����״̬������ʾ
		it--;	//	�����������������б�����ֹ��COblist �� STL list�Ĳ�ͬ
		//if(pos == m_score.GetHeadPosition()) break;
		if (m_score.begin() == it)
			break;

		//CScore *p = (CScore*)m_score.GetPrev(pos);
		CScore *p = (CScore*)(*it);	// it = m_score.end()��������it--������*it����������ʱ������ֹ

		s.Format(TEXT("%d"), p->gameNumber);
		m_lcScore.InsertItem(i, s);

		s.Format(TEXT("%d"), p->steps);
		m_lcScore.SetItemText(i, 1, s);

		m_lcScore.SetItemText(i, 2, statusStr[p->gameStatus]);

		CTimeSpan ts = p->tmEnd - p->tmStart;	// #include <atltime.h>
		s.Format(TEXT("%2dm%2ds"), ts.GetMinutes(), ts.GetSeconds());
		m_lcScore.SetItemText(i, 3, s);

		m_lcScore.SetItemText(i, 4,
			p->tmStart.Format(fmts) + TEXT("/") + p->tmEnd.Format(fmts));

		++nTotal;

		if (p->gameStatus == CScore::gamePassed) {
			++nPassed;
			nStepsPassed += p->steps;
		}
		else if (p->gameStatus == CScore::gameGiveUp) {
			++nGiveUp;
		}
		else if (p->gameStatus == CScore::gameDead) {
			++nDead;
		}
	}

	//ͳ��ս��
	m_lcScoreInfo.SetBkColor(RGB(0, 128, 0));
	m_lcScoreInfo.SetTextBkColor(RGB(0, 128, 0));
	m_lcScoreInfo.SetTextColor(RGB(218, 218, 218));

	// �ÿո����\t����������2���ش�	listbox����������������ȡ���������ݻ�����ȷ�ġ�
	m_lcScoreInfo.InsertColumn(0, TEXT("��Ŀ"), LVCFMT_LEFT, charWid * 14);
	m_lcScoreInfo.InsertColumn(1, TEXT("ͳ��"), LVCFMT_LEFT, charWid * 12);
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
	if (m_score.empty()) return;

	//CScore *p = (CScore*)m_score.GetHead();
	CScore *p = (CScore*)m_score.front();
	p->tmEnd = CTime::GetCurrentTime();

	//ʮ�������ڽ⿪�ľ���Ϊ��ʹ�����Զ����İ������������¼
	if ((p->tmEnd - p->tmStart).GetTotalSeconds() < 15) {
		//m_score.RemoveHead();
		m_score.pop_front();
		delete p;
		return;
	}

	//CJLDoc *pDoc = AfxGetDocument();
	p->gameStatus =
		g_fcData.GameOver() ? CScore::gamePassed :
		g_fcData.m_Hints.IsEmpty() ? CScore::gameDead :
		CScore::gameGiveUp;
	p->steps = g_fcData.m_OpsList.size();
}

void CDlgScore::InitScore()
{
	//CScore *p = new CScore;
	CScore *p = new CScore;
	ATLASSERT(p);
	m_score.push_front(p);

	//CJLDoc *pDoc = AfxGetDocument();

	p->gameNumber = g_fcData.m_nCurGameNumber;
	p->tmStart = CTime::GetCurrentTime();
}

//�����˾��Ƿ��������
bool CDlgScore::IsOldGameNumber(int gameNum)
{
	//for(POSITION pos = m_score.GetHeadPosition(); pos != NULL; )
	//	if(gameNum == ((CScore*)m_score.GetNext(pos))->gameNumber)
	//		return true;
	for (list<CMyObject*>::iterator it = m_score.begin(); it != m_score.begin(); it++)
	{
		if (gameNum == ((CScore*)*it)->gameNumber)
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
