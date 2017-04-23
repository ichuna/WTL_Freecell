// DlgAIShow.cpp : implementation file
//

#include "stdafx.h"
//#include "JL.h"
//#include "JLDoc.h"
//#include "JLView.h"
#include "DlgAIShow.h"
#include "datatype.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAIShow dialog

#define ID_TIMER 200
//extern CJLDoc * AfxGetDocument();
//extern CJLView * AfxGetView();

//CDlgAIShow::CDlgAIShow(CWnd* pParent /*=NULL*/)
//	: CDialog(CDlgAIShow::IDD, pParent)
//{
//	//{{AFX_DATA_INIT(CDlgAIShow)
//	m_pCurPos = 0;
//	m_nElapse = 240;
//	m_bPlaying = 0;
//	//}}AFX_DATA_INIT
//}

//BOOL CDlgAIShow::OnInitDialog() 
LRESULT CDlgAIShow::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	//CDialog::OnInitDialog();
	//CenterWindow(GetParent());

	m_pCurPos = 0;
	m_nElapse = 240;
	m_bPlaying = 0;

	// TODO: Add extra initialization here
	//�Ѵ����ƶ���������Ķ���

	CRect r, wr;
	//AfxGetApp()->GetMainWnd()->GetWindowRect(r);
	::GetWindowRect(g_pMain->m_hWnd, r);
	GetWindowRect(wr);
	//MoveWindow(r.left, r.top-wr.Height(), wr.Width(), wr.Height());

	int y = r.top;
	if (g_pMain->IsZoomed())
	{
		y = 0;
	}

	//�Ѵ����ƶ����������ϲ��м�
	MoveWindow(r.left + (r.Width() - wr.Width()) / 2, y, wr.Width(), wr.Height());

	//((CButton *)GetDlgItem(IDB_PREV))->EnableWindow(FALSE);
	//((CButton *)GetDlgItem(IDB_FIRST))->EnableWindow(FALSE);
	::EnableWindow(GetDlgItem(IDB_PREV), FALSE);
	::EnableWindow(GetDlgItem(IDB_FIRST), FALSE);

	SetTimer(ID_TIMER, m_nElapse, NULL);
	m_bPlaying = 0;

	ShowStepInfo();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

//BEGIN_MESSAGE_MAP(CDlgAIShow, CDialog)
//	//{{AFX_MSG_MAP(CDlgAIShow)
//	ON_BN_CLICKED(IDB_NEXT, OnNext)
//	ON_BN_CLICKED(IDB_PREV, OnPrev)
//	ON_BN_CLICKED(IDB_FIRST, OnFirst)
//	ON_BN_CLICKED(IDB_LAST, OnLast)
//	ON_BN_CLICKED(IDB_AUTO_PLAY, OnAutoPlay)
//	ON_WM_TIMER()
//	//}}AFX_MSG_MAP
//END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAIShow message handlers
LRESULT CDlgAIShow::OnNext(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//void CDlgAIShow::OnNext() 
{
	// TODO: Add your control notification handler code here
	CancelCurMv();

	//CJLDoc *pDoc = AfxGetDocument();
	if (g_fcData.m_OpsList.m_tList.empty())
		return 0;

	COperations *pOpsCur = NULL;
	unsigned cnt = g_fcData.m_OpsList.m_tList.size();
	if(m_pCurPos < cnt) {
		//ȡ����ǰ�Ĳ�����¼
		//ָ����һ��������¼
		//POSITION posCur = g_fcData.m_pOps->FindIndex(m_pCurPos);
		//pOpsCur = (COperations*)pDoc->m_pOps->GetAt(posCur);
		pOpsCur = g_fcData.getAt(g_fcData.m_OpsList.m_tList, m_pCurPos);
		++m_pCurPos;
	}

	//CObList *pOps= pOpsCur->pOps;
	//POSITION pos = pOps->GetTailPosition();
	//while(pos) {
	//	COperation *pOp = (COperation*)pOps->GetPrev(pos);
	//	pDoc->MoveCards(pOp->des,pOp->src,pOp->cnt);
	//}

	// STL-list��β������-OK
	list<COperation>& ops = pOpsCur->m_opList.m_tList;
	for (list<COperation>::iterator it = ops.end(); it != ops.begin();)
	{
		it--;
		COperation & op = *it;
		g_fcData.MoveCards(op.des, op.src, op.cnt);
	}


	//��������ִ����һ��
	//((CButton *)GetDlgItem(IDB_PREV))->EnableWindow(TRUE);
	//((CButton *)GetDlgItem(IDB_FIRST))->EnableWindow(TRUE);
	//if(m_pCurPos == cnt) {
	//	((CButton *)GetDlgItem(IDB_NEXT))->EnableWindow(FALSE);
	//	((CButton *)GetDlgItem(IDB_LAST))->EnableWindow(FALSE);
	//	((CButton *)GetDlgItem(IDB_PREV))->SetFocus();
	//}

	//��������ִ����һ��
	::EnableWindow(GetDlgItem(IDB_PREV), TRUE);
	::EnableWindow(GetDlgItem(IDB_FIRST), TRUE);
	if(m_pCurPos == cnt) {
		::EnableWindow(GetDlgItem(IDB_NEXT), FALSE);
		::EnableWindow(GetDlgItem(IDB_LAST), FALSE);
		::SetFocus(GetDlgItem(IDB_PREV));
	}

	ShowStepInfo();

	return 0;
}

//void CDlgAIShow::OnPrev() 
LRESULT CDlgAIShow::OnPrev(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	CancelCurMv();

	//CJLDoc *pDoc = AfxGetDocument();
	//if(pDoc->m_pOps->IsEmpty())
	//	return;
	if (g_fcData.m_OpsList.m_tList.empty())
		return 0;

	COperations *pOpsCur = NULL;
	if (m_pCurPos > 0) {
		//ָ����һ��������¼
		//ȡ����һ��������¼
		--m_pCurPos;
		//POSITION posCur = pDoc->m_pOps->FindIndex(m_pCurPos);
		//pOpsCur = (COperations*)pDoc->m_pOps->GetAt(posCur);
		//CMyObject* getAt(list<CMyObject*>* pList, unsigned n)
		pOpsCur = (COperations*)g_fcData.getAt(g_fcData.m_OpsList.m_tList, m_pCurPos);
	}

	//CObList *pOps= pOpsCur->pOps;
	//POSITION pos = pOps->GetHeadPosition();
	//while(pos) {
	//	COperation *pOp = (COperation*)pOps->GetNext(pos);
	//	pDoc->MoveCards(pOp->src,pOp->des,pOp->cnt);
	//}

	list<COperation>& ops = pOpsCur->m_opList.m_tList;
	for (list<COperation>::iterator it = ops.begin(); it != ops.end(); it++)
	{
		COperation& op = *it;
		g_fcData.MoveCards(op.src, op.des, op.cnt);
	}

	//CObList
	//GetHeadPosition���ص�������ͷԪ�ص�λ��
	//CObList..GetNext(pos)�ȷ���������pos��ָԪ�أ�Ȼ��posָ����һ��Ԫ�ء�����GetNext��pos��ֵ�͸ı���
	//΢���Կ��а���

	//��������ִ����һ��
	//((CButton *)GetDlgItem(IDB_NEXT))->EnableWindow(TRUE);
	//((CButton *)GetDlgItem(IDB_LAST))->EnableWindow(TRUE);
	//if(m_pCurPos == 0) {
	//	ShowStepInfo();

	//	((CButton *)GetDlgItem(IDB_PREV))->EnableWindow(FALSE);
	//	((CButton *)GetDlgItem(IDB_FIRST))->EnableWindow(FALSE);
	//	((CButton *)GetDlgItem(IDB_NEXT))->SetFocus();
	//}

	//��������ִ����һ��
	::EnableWindow(GetDlgItem(IDB_NEXT), TRUE);
	::EnableWindow(GetDlgItem(IDB_LAST), TRUE);
	if(m_pCurPos == 0)
	{
		ShowStepInfo();
		::EnableWindow(GetDlgItem(IDB_PREV), TRUE);
		::EnableWindow(GetDlgItem(IDB_FIRST), TRUE);
		::SetFocus(GetDlgItem(IDB_NEXT));
	}

	ShowStepInfo();
	return 0;
}

//��ʾ����
void CDlgAIShow::ShowStepInfo()
{
	//CJLDoc* pDoc = AfxGetDocument();

	WTL::CString step_info;
	step_info.Format(TEXT("���ȡ� %d / %d ��"),
		(int)m_pCurPos,
		(int)g_fcData.m_OpsList.m_tList.size());
	SetWindowText(step_info);
}
 
//void CDlgAIShow::OnFirst() 
LRESULT CDlgAIShow::OnFirst(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	BOOL b;
	while(m_pCurPos > 0) OnPrev(NULL, NULL, NULL, b);
	return 0;
}

//void CDlgAIShow::OnLast() 
LRESULT CDlgAIShow::OnLast(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	//CJLDoc *pDoc = AfxGetDocument();
	BOOL b;
	while (m_pCurPos < g_fcData.m_OpsList.m_tList.size()) OnNext(NULL, NULL, NULL, b);
	return 0;
}

//void CDlgAIShow::OnAutoPlay() 
LRESULT CDlgAIShow::OnAutoPlay(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	if(m_bPlaying) { 
		//��ͣʱ��ҲҪ�ѻطŹ��̵ĵ�ǰ����ȡ��
		CancelCurMv();
	}
	m_bPlaying = !m_bPlaying ? 1 : 0;
	SetDlgItemText(IDB_AUTO_PLAY, m_bPlaying ? TEXT("��ͣ(&T)") : TEXT("�ط�(&A)"));
	return 0;
}

void CDlgAIShow::CancelCurMv()
{
	//������ڻطţ����ȰѻطŹ��̵ĵ�ǰ����ȡ��
	while(m_bPlaying) {
		if(m_bPlaying < 7) {
			OnTimer(ID_TIMER);//������Ϊ�˻ָ��ѷ�ɫ������
		} else {
			// ����m_bPlaying==5���´ε���OnTimer��m_bPlaying==1
			// ����Ϊ��ʹ����ɱ��ε��ƶ���ֱ�Ӹ�m_bPlaying��ֵ1
			m_bPlaying = 1;
			break;
		}
	}
}

//void OnTimer(UINT uTimerID);
void CDlgAIShow::OnTimer(UINT uTimerID)
{
	// TODO: Add your message handler code here and/or call default
	if(!m_bPlaying) return;//��ͣ��ʾ

	BOOL b;
	//CJLDoc *pDoc = AfxGetDocument();
	if (m_pCurPos == g_fcData.m_OpsList.m_tList.size()) {
		m_bPlaying = 0; //������ͣ��־
		SetDlgItemText(IDB_AUTO_PLAY, TEXT("�ط�(&A)"));
	}
	else if(m_bPlaying == 7) { 
		OnNext(NULL, NULL, NULL, b);//�ƶ�
		m_bPlaying = 1;
	}
	else if(m_bPlaying < 5){
		//CJLDoc *pDoc = AfxGetDocument();
		int cnt = g_fcData.m_OpsList.m_tList.size();

		//ȡ����ǰ�Ĳ�����¼
		//POSITION posCur = pDoc->m_pOps->FindIndex(m_pCurPos);//λ��
		//pOpsCur = (COperations*)pDoc->m_pOps->GetAt(posCur);//ָ��
		COperations* pOpsCur = g_fcData.getAt(g_fcData.m_OpsList.m_tList, m_pCurPos);

		list<COperation>& ops = pOpsCur->m_opList.m_tList;
		//CObList *pOps= pOpsCur->pOps;//������
		//������¼���ƶ��������Զ����ƶ���
		//��ĩβ�����ݼ�¼�����ƶ�����
		//POSITION pos = pOps->GetTailPosition();
		//COperation *pOp = (COperation*)pOps->GetPrev(pos);
		//COperation *pOp = (COperation*)g_fcData.getAt(g_fcData.m_pOps, m_pCurPos);
		list<COperation>::iterator it = ops.end();
		it--;
		COperation& op = *it;

		//�Խ�Ҫ�ƶ����ƽ��з�ɫ����ȷ��ʾ�����һ���Ķ���
		//CClientDC cdc(AfxGetView());
		CClientDC cdc(g_pView->m_hWnd);
		int nSrc = g_fcData.CntCardsIn(op.src);
		cdc.InvertRect(g_fcData.RectOf(op.src, nSrc - op.cnt + 1, op.cnt));
		int nDes = g_fcData.CntCardsIn(op.des);
		cdc.InvertRect(g_fcData.RectOf(op.des, max(nDes, 1), 1));
		//״̬����
		m_bPlaying++;
	} else {
		//״̬����
		m_bPlaying++;
	}

	//CDialog::OnTimer(nIDEvent);
}

//void CDlgAIShow::OnCancel() 
LRESULT CDlgAIShow::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add extra cleanup here
	BOOL b;
	OnLast(NULL, NULL, NULL, b);//�����ط�ʱ��ͣ����Ϸ�����ĵط�
	KillTimer(ID_TIMER);
	//CDialog::OnCancel();
	EndDialog(0);
	return 0;
}

