// WTLfcData.cpp : implementation of the CJLDoc class
//

#include "stdafx.h"

#include "WTLfcData.h"
#include "Datatype.h"
#include <time.h>
#include "WTLfcView.h"
#include "DlgPassed.h"
#include "DlgAIShow.h"

//����֧��ϴ�Ƶĺ���
#include <algorithm>
#include <functional>

/////////////////////////////////////////////////////////////////////////////
// WTLfcData

#define FitTrash(card,x) (TYPE(x)==TYPE(card)&&NUM(x)==NUM(card)-1)

CWTLfcData::CWTLfcData()
{
	m_nSel = 0;
	//m_pOps = new CObList;
	//m_pOps = new list<CMyObject*>;
	m_Hints.ClrHints();

	srand(UINT(time(NULL)));
	m_nCurGameNumber = Random();

	m_bEnableAlert = false;
	m_bQuickMove = false;
	m_bEnableDbClick = true;
	m_bMaxMove = true;
	m_bAICalRunning = false;
	m_bRealTimeUpdate = false;
	m_nDDASpeed = 75;
}

CWTLfcData::~CWTLfcData()
{
	//ClrOpsRecords();
	//delete m_pOps;
}


/////////////////////////////////////////////////////////////////////////////
// CJLDoc commands
bool CWTLfcData::IsCol(UINT col)
{
	return (col <= 16 && col >= 1);
}


// +-----Buf----+    +---Recycle---+
// | 9 10 11 12 | JL | 13 14 15 16 |
// +------------+    +-------------+
// +------------Cards--------------+
// | 1   2   3   4   5   6   7   8 |
// +-------------------------------+
//���ĵ����Ƴ��򣺽�src�е�n�����ƶ���des��
void CWTLfcData::MoveCards(UINT des, UINT src, UINT n)
{
	ATLASSERT(IsCol(src) && !IsEmptyCol(src) //Դ�зǿ�
		&& n <= CntSeriIn(src) //�����ƶ�ȫ��������
		&& IsCol(des));

	//ȡ����ǰѡ��
	if (!m_bAICalRunning) UnselectCardCol();

	CRect rSrc, rDes;
	UINT *pSrc, *pDes, *pTop;
	if (src <= 8) {
		pTop = &m_iCards[src - 1][0];
		pSrc = &pTop[pTop[19] - 1];//ָ�����
		//ˢ�����ߵĲ���
		rSrc = RectOf(src, pTop[19] - n + 1, n);
		//�ı��Ƶļ���
		pTop[19] -= n;
		//ˢ������
		if (pTop[19] + n > 13) {
			rSrc.UnionRect(rSrc, RectOf(src, 1, pTop[19]));
		}
	}
	else if (src <= 12) {
		pSrc = &m_iBuffer[src - 9];
		rSrc = RectOf(src, 1, 1);
	}
	else {
		pTop = &m_iRecycle[src - 13][0];
		pSrc = &pTop[pTop[13] - 1];//ָ�����
		pTop[13] -= n;//�ı��Ƶļ���
		ATLASSERT(n == 1);
		rSrc = RectOf(src, 1, 1);
	}

	if (des <= 8) {
		pTop = &m_iCards[des - 1][0];
		pDes = &pTop[pTop[19]];//ָ�����֮��
		//ˢ�������Ĳ���
		rDes = RectOf(des, pTop[19] + 1, n);
		//�ı��Ƶļ���
		pTop[19] += n;
		//ˢ������
		if (pTop[19] > 13) {
			rDes.UnionRect(rDes, RectOf(des, 1, pTop[19]));
		}
	}
	else if (des <= 12) {
		pDes = &m_iBuffer[des - 9];
		rDes = RectOf(des, 1, 1);
		ATLASSERT(!m_iBuffer[des - 9] && n == 1);
	}
	else {
		pTop = &m_iRecycle[des - 13][0];
		pDes = &pTop[pTop[13]];//ָ�����֮��
		pTop[13] += n;//�ı��Ƶļ���
		ATLASSERT(n == 1);
		rDes = RectOf(des, 1, 1);
	}

	UINT *p = pSrc + 1 - n;//pָ�����������Ž�Ҫ���ƶ�����
	for (UINT i = 0; i < n; i++) {
		*pDes++ = *p;//�ƶ���Ŀ�괦
		*p++ = 0;//Դ������
	}

	if (m_bAICalRunning && !m_bRealTimeUpdate) {
		return;
	}
	g_pView->InvalidateRect(RectOfStep());//ˢ�²�����Ϣ
	g_pView->InvalidateRect(rSrc);//ˢ��Դ������
	g_pView->InvalidateRect(rDes);//ˢ��Ŀ��������
}

//���չ���f�������ж� a�ɷ���b�� ��һ�۶϶�������a��b�Ƿ����
//����f���£�
//    ���ƿ��Է��ں����£����ƿ��Է��ں�����
//    ���Ǳ��뱣֤������������ϣ�С������������
//    �ҵ���ֻ�����1��
//    ���磺
//        �մ˹��򣬺���5��ֻ���Էź���4����÷��4
//    
bool CWTLfcData::FitFormula(UINT b, UINT a)
{
	ATLASSERT(a <= 52 && a >= 1 && b <= 52 && b >= 1);
	//Type()   =  0 ����   1 ����   2 ÷��   3 ����
	//b��a��ͬ��ɫ��b�ĵ�����a��һ��
	return (TYPE(a) + TYPE(b)) % 2 == 1 && NUM(b) - NUM(a) == 1;
}
//ϴ��
void CWTLfcData::Shuffle()
{
	/*
	//׼��һ�����ƣ���ϴ��
	using namespace std;
	vector<int> cards(52);
	for(int i = 1, *it = cards.begin(); it != cards.end(); *it++ = i++) ;

	srand(m_nCurGameNumber >> 16);
	random_shuffle(cards.begin(),cards.end());
	srand(m_nCurGameNumber & 0xFFFF);
	random_shuffle(cards.begin(),cards.end());
	*/
	//׼��һ�����ƣ���ϴ��
	int cards[52];
	for (int i = 1; i <= 52; ++i) cards[i - 1] = i;

	using namespace std;

	srand(m_nCurGameNumber >> 16);
	random_shuffle(cards, cards + 52);
	srand(m_nCurGameNumber & 0xFFFF);
	random_shuffle(cards, cards + 52);

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
	//���Ƶ�����m_iCards
	for (int col = 0; col <= 3; col++) {
		UINT *pTop;

		pTop = &m_iCards[col][0];
		for (int i = 0; i < 7; i++) {
			UINT *pDes = &pTop[pTop[19]];//ָ�����֮��
			pDes[i] = cards[col * 7 + i];
		}
		pTop[19] = 7;

		pTop = &m_iCards[col + 4][0];
		for (int i = 0; i < 6; i++) {
			UINT *pDes = &pTop[pTop[19]];//ָ�����֮��
			pDes[i] = cards[28 + col * 6 + i];
		}
		pTop[19] = 6;
	}
}

void CWTLfcData::SelectCardCol(UINT col)
{
	ATLASSERT(IsCol(col) && !IsEmptyCol(col));

	//������У�ȡ����ǰѡ��
	UnselectCardCol();

	//ѡ����һ�в�ˢ��
	m_nSel = col;
	InvalidateRect(RectOf(col, CntCardsIn(col), 1));
}
//��ѡ�д���
void CWTLfcData::UnselectCardCol()
{
	//��Ϸ�տ�ʼ�����ȡ��ѡ�С���
	//���ƶ��ȶ��ᵼ�����κ��б�ѡ

	//���û��ѡ���κ��оͲ���
	if (!m_nSel) return;

	CRect r = RectOf(m_nSel, CntCardsIn(m_nSel), 1);
	//ȡ��ѡ�в�ˢ��
	m_nSel = 0;
	InvalidateRect(r);
}
//���������Ƿ�Ϊ��
bool CWTLfcData::IsEmptyCol(UINT col)
{
	ATLASSERT(IsCol(col));
	if (col <= 8) {
		return !m_iCards[col - 1][19];
	}
	else if (col <= 12) {
		return !m_iBuffer[col - 9];
	}
	else {
		return !m_iRecycle[col - 13][13];
	}
}

// ����ʵ������ӱ�ѡ�����ƶ�������ֽ�Ƶ�Ŀ����
//���������֮��������ú���MoveCards������ʵ�ʵ��ƶ���
UINT CWTLfcData::CntMaxMv(UINT desCol, UINT srcCol)
{
	ATLASSERT(IsCol(srcCol) && !ColInRecycle(srcCol) && !IsEmptyCol(srcCol));
	ATLASSERT(IsCol(desCol));

	UINT n = 0;
	//Ŀ����������
	if (desCol <= 8) {
		if (COL_IN_BUFF(srcCol)) { //Դ���ǻ�����
			if (IsEmptyCol(desCol) ||
				FitFormula(BottCard(desCol), BottCard(srcCol)))
				n = 1;
		}
		else {
			//Դ��������
			UINT nSeri = CntSeriIn(srcCol);//����������������
			if (IsEmptyCol(desCol)) { //Ŀ�����ǿ�����
				UINT maxSuppliment = CntMaxSuppliment(true);
				//�϶������ƶ�
				n = min(maxSuppliment, nSeri);
			}
			else {
				UINT bottSrc = BottCard(srcCol);//Դ�����������
				UINT bottDes = BottCard(desCol);//Ŀ�������������
				UINT numSrc = NUM(bottSrc);//�Ƶ���
				UINT numDes = NUM(bottDes);//�Ƶ���
				n = numDes - numSrc;
				UINT maxSuppliment = CntMaxSuppliment(false);
				//�����ϸ��������������ſ����ƶ���
				if ( 	//Ŀ���Ƶ�������Դ������֮�ϵ�ָ�������� ��
					numDes >= numSrc + 1 && numDes <= numSrc + nSeri &&
					//����Դ�ƴ��������Һ��������ż��������ͬ ��
					n % 2 == (TYPE(bottSrc) + TYPE(bottDes)) % 2 &&
					//���㹻�ռ����ƶ�
					n <= maxSuppliment)
				{
					;
				}
				else {
					n = 0;
				}
			}
		}
	}
	else if (desCol <= 12) { //Ŀ�����ǻ�����
		if (IsEmptyCol(desCol))
			n = 1;//��������������ƶ�һ��
	}
	else { //Ŀ�����ǻ�����
		int s = BottCard(srcCol);
		if (!IsEmptyCol(desCol)) {
			int d = BottCard(desCol);
			if (TYPE(s) == TYPE(d) && NUM(d) == NUM(s) - 1)
				n = 1;//��ɫ���������Сһ������Ի���
		}
		else if (NUM(s) == 1 && TYPE(s) + 13 == desCol)
			n = 1;//��A�һ�ɫ���������Ӧ�����������ƣ�
	}

	return n;
}

//�������в��Զ��ӳ�1-12������С����ֱ���޷��ӳ�Ϊֹ
void CWTLfcData::AutoThrow()
{
	UINT colSrc, cardSrc, numSrc, colDes, sons[2];
	while (true) { //ֱ��û���ƿ���Ϊֹ
		for (colSrc = 1; colSrc <= 12; colSrc++) { //Ѱ�ҿ��ӵ������ڵ���
			if (IsEmptyCol(colSrc)) continue;
			cardSrc = BottCard(colSrc);
			if (!Trashable(cardSrc)) continue;
			numSrc = NUM(cardSrc);
			colDes = TYPE(cardSrc) + 13;
			if (numSrc == 1 || numSrc == 2) break;
			if (m_bAICalRunning) break;//�Զ����ʱ�������Ӿ���

			//���������Ƿ��Ѿ�����
			sons[0] = sons[1] = colDes;
			sons[0] -= colDes > 13 ? 1 : -3;
			sons[1] += colDes < 16 ? 1 : -3;
			if (
				m_iRecycle[sons[0] - 13][13] && //���ƵĻ����зǿ�
				m_iRecycle[sons[1] - 13][13] && //���ƵĻ����зǿ�
				NUM(BottCard(sons[0])) >= numSrc - 1 &&
				NUM(BottCard(sons[1])) >= numSrc - 1
				) break;
		}
		if (colSrc > 12) break;
		if (!m_bQuickMove && ColInCard(colSrc)) { //�����ƶ���ʱ��û�ж���
			CRect rs = RectOf(colSrc, CntCardsIn(colSrc), 1);
			CRect rd = RectOf(colDes, 1, 1);
			::LineDDA(rs.left, rs.top, rd.left, rd.top, LineDDACallback, cardSrc);
			// ��������������ԭ���� g_fcData.m_nDDASpeed û�г�ʼ��
		}
		MoveCards(colDes, colSrc, 1);

		//if (m_pOps->empty())
		if (m_OpsList.empty())
		{
			//Record(new COperations(colDes, colSrc, 1));
			Record(colDes, colSrc, 1);
		}
		else
		{ //���ƺ���Զ����ƶ�����������ƶ�������һ��
			// GetTail	�����б��е�βԪ�أ��б���Ϊ�գ�
			//((COperations*)m_pOps->back())->AddOperation(colDes, colSrc, 1);
			m_OpsList.back().AddOperation(colDes, colSrc, 1);
		}
	}
}

//�����Ƿ���Ϸ����
bool CWTLfcData::GameOver()
{
	//��������ƶ��ڻ�����������Ϸ����
	for (UINT i = 13; i <= 16; i++) {
		if (m_iRecycle[i - 13][13] == 13) continue;
		return false;
	}
	return true;
}

//���չ����������м�������˳���ŵ�
UINT CWTLfcData::CntSeriIn(UINT col)
{
	//������Ϊ�����кͻ����е������Ƴ�Ϊ1
	ATLASSERT(IsCol(col) && !IsEmptyCol(col));
	UINT nSeri = 1;//�ǿյĻ�������������Ϊ1
	if (col <= 8) {
		UINT *pTop, *p1, *p2;
		pTop = &m_iCards[col - 1][0];
		p1 = &pTop[pTop[19] - 1];//ָ�����
		p2 = p1 - 1;//p2ָ��p1�������
		while (p2 >= pTop && FitFormula(*p2--, *p1--)) ++nSeri;
	}
	return nSeri;
}

//����������������ж������ơ�Cnt Ϊ Count����д
UINT CWTLfcData::CntCardsIn(UINT col)
{
	ATLASSERT(IsCol(col));

	if (col <= 8) {
		return m_iCards[col - 1][19];
	}
	else if (col <= 12) {
		return m_iBuffer[col - 9] ? 1 : 0;
	}
	else {
		return m_iRecycle[col - 13][13];
	}
}


//  | 1 ... 13 | 14 ... 26 | 27 ... 39  | 40 ... 52 |
//  | A ...  K |  A ...  K |  A ...  K  |  A ...  K |
//  | �����ң� |  �����ң� |  ��÷����  |  �����飭 |
UINT CWTLfcData::Num(UINT card) { return (card - 1) % 13 + 1; }//���� 1��13
UINT CWTLfcData::Type(UINT card) { return (card - 1) / 13; }//��ɫ: 0���� 1���� 2÷�� 3����

//����Ŀǰ���������������Ƶȴ��ƶ�
//����Ŀǰ�ճ���������(���ÿռ�)���ɹ�һ�����ƶ���������
//OccupyAnEmptyColָ���ڼ���ʱ�Ƿ�ʹ��ȫ�����ÿռ�
//��������ڼ����������һ�����ƶ���������ʱ�����еĿ���
//�ռ���ټ�һ��
UINT CWTLfcData::CntMaxSuppliment(bool OccupyAnEmptyCol)
{
	int a = 0, b = 0;
	//ͳ�ƿ������� 
	for (UINT i = 1; i <= 8; i++) {
		if (m_iCards[i - 1][19] == 0) ++b;
	}
	//ͳ�ƿյ���
	for (int i = 9; i <= 12; i++) {
		if (m_iBuffer[i - 9] == 0) ++a;
	}
	//��һ�����н��ᱻ��ΪĿ������
	if (OccupyAnEmptyCol) {
		//�����������ƶ������������˹�������ʱֻ��յ��й�
		if (!m_bAICalRunning) return a + b;
		//�����κ�����¶�һ��
		ATLASSERT(b);
		--b;
	}
	return a*(b + 1) + b*(b + 1) / 2 + 1;
}
//ȡ�������еĵ���
UINT CWTLfcData::BottCard(UINT col)
{
	ATLASSERT(IsCol(col) && !IsEmptyCol(col));

	if (col <= 8) {
		UINT *pTop = &m_iCards[col - 1][0];
		return pTop[pTop[19] - 1];//ָ�����
	}
	else if (col <= 12) {
		return m_iBuffer[col - 9];
	}
	else {
		UINT *pTop = &m_iRecycle[col - 13][0];
		return pTop[pTop[13] - 1];//ָ�����
	}
}

bool CWTLfcData::ColInCard(UINT col) {
	return (col <= 8 && col >= 1);
}

bool CWTLfcData::ColInBuf(UINT col) {
	return (col <= 12 && col >= 9);
}

bool CWTLfcData::ColInRecycle(UINT col) {
	return (col <= 16 && col >= 13);
}

bool CWTLfcData::IsCard(UINT card)
{
	return (card >= 1 && card <= 52);
}

void CWTLfcData::InvalidateRect(CRect r)
{
	//��������Զ������ô�п���������Ҫ���Ǽ����ٶȶ�����
	//ʱʵ�����ƾ�״̬�������Զ����ʱ�������ر�ʱʵ��ʾ�ƾ�
	//��һѡ����ܴ��ӿ����ٶ�

	//��ʱʵ��ʾ�ƾֵ���������Ȼ����ٶȽ���������
	//�󲿷ֵģ������90%���ϵģ��ƾ�ͨ�������ں̵ܶ�ʱ���ڽ�����
	//��ʵʱ��ʾ�ƾ�״̬����һѡ��Ĭ���Ǵ򿪵ģ��Ͼ��Զ����ʱ�ƾֵ�
	//�ݱ���̿����������е���˼�ġ����ǻ���ϣ��֪�����Զ��ڸ�Щʲô��
	//�������Ч����ˢ��
	if (m_bAICalRunning && !m_bRealTimeUpdate)return;
	//CJLView *pView = GetView();
	g_pView->InvalidateRect(r);
	SendMessage(g_pView->m_hWnd, WM_PAINT, 0, 0);
}

// Record(new COperations(colDes, colSrc, 1));
//void CWTLfcData::Record(CMyObject *thisStep)
void CWTLfcData::Record(UINT des, UINT src, UINT n)
{
	//����һ����¼��ˢ�²�����Ϣ
	//m_pOps->push_back(thisStep);
	m_OpsList.push_back(COperations(des, src, n));
	InvalidateRect(RectOfStep());
}

//����
//void CWTLfcData::OnUndo()
//{
//	Undo();
//	GetHints();
//}
//ִ�г�������
void CWTLfcData::Undo()
{
	//::MessageBoxW(NULL, L"Undo Test", NULL, NULL);
	//if (m_pOps->IsEmpty()) return;
	if (m_OpsList.empty()) return;

	//����һ��
	//COperations *pOpsLast = (COperations*)m_pOps->back();
	COperations& OpsLast = m_OpsList.back();
	// GetTail	�����б��е�βԪ�أ��б���Ϊ�գ�
	//list<CMyObject*>* pOps = pOpsLast->m_pObjlist;
	list<COperation>& ops = OpsLast.m_OpList;

	for (list<COperation>::iterator it = ops.begin(); it != ops.end(); it++)
	{
		COperation& op = *it;
		MoveCards(op.src, op.des, op.cnt);
	}
	//pOpsLast->ClrOps();
	//delete pOpsLast;
	m_OpsList.pop_back();

	InvalidateRect(RectOfStep());

	//CObList
	//GetHeadPosition���ص�������ͷԪ�ص�λ��
	//CObList..GetNext(pos)�ȷ���������pos��ָԪ�أ�Ȼ��posָ����һ��Ԫ�ء�����GetNext��pos��ֵ�͸ı���
	//΢���Կ��а���
		

	////����һ��
	//COperations *pOpsLast = (COperations*)m_pOps->GetTail();
	//CObList *pOps = pOpsLast->pOps;
	//POSITION posMov = pOps->GetHeadPosition();
	//while (posMov) {
	//	COperation *pOp = (COperation*)pOps->GetNext(posMov);
	//	MoveCards(pOp->src, pOp->des, pOp->cnt);
	//}
	//pOpsLast->ClrOps();
	//delete pOpsLast;
	//m_pOps->RemoveTail();

	//InvalidateRect(RectOfStep());
}

//��Ϸ���ص���ͷ���Ǳ��������¼��׼���ط�
void CWTLfcData::BackHome()
{
	int nSteps = m_OpsList.size();

	//��ԭ�ƾֵ����������¼
	while (nSteps > 0)
	{
		COperations *pOpsLast = getAt(m_OpsList, --nSteps);
		list<COperation>& Ops = pOpsLast->m_OpList;

		list<COperation>::iterator it = Ops.begin();
		for (; it != Ops.end(); it++)
		{
			COperation& op = *it;
			MoveCards(op.src, op.des, op.cnt);
		}
		InvalidateRect(RectOfStep());
	}
}

//void WTLfcData::OnSetting()
//{
//	// TODO: Add your command handler code here
//
//	CDlgSettings dlg;
//	//set values of settings
//	dlg.m_bEnableAlert = m_bEnableAlert;
//	dlg.m_bEnableDBClick = m_bEnableDbClick;
//	dlg.m_bQuickMove = m_bQuickMove;
//	dlg.m_bMaxMove = m_bMaxMove;
//	dlg.m_nDDASpeed = m_nDDASpeed;
//
//	//prompt
//	dlg.DoModal();
//
//	//get settings
//	m_bEnableAlert = dlg.m_bEnableAlert;
//	m_bEnableDbClick = dlg.m_bEnableDBClick;
//	m_bQuickMove = dlg.m_bQuickMove;
//	m_bMaxMove = dlg.m_bMaxMove;
//	m_nDDASpeed = dlg.m_nDDASpeed;
//}
//
//void WTLfcData::DeleteContents()
//{
//	// TODO: Add your specialized code here and/or call the base class
//	ClrOpsRecords();
//
//	CDocument::DeleteContents();
//}
//
//�����col�дӵ�idx���ƿ�ʼ��nCards����ռ�ݵľ���
//����Ϊѡ���ƶ����ػ������ṩ��ͼ��Ϣ
CRect CWTLfcData::RectOf(UINT col, UINT idx, UINT nCards)
{
	ATLASSERT(IsCol(col));
	CPoint org = g_fcCcs.ptOrg;

	CRect r;
	if (col <= 8) {
		org.x += g_fcCcs.CARD_INT;
		org.y += g_fcCcs.CARD_HEI + g_fcCcs.PILE_VINT;
		r.left = org.x + (g_fcCcs.CARD_WID + g_fcCcs.CARD_INT)*(col - 1);
		r.right = r.left + g_fcCcs.CARD_WID;
		//����ʮ�����ƺ�ÿ����һ���ƣ�¶�����ֵĸ߶Ⱦͼ���1������
		int n = m_iCards[col - 1][19] - 13;
		if (n > 0)			//����ʮ�����ƺ�ÿ����һ���ƣ�¶�����ֵĸ߶Ⱦͼ���2*ccs.dRate������
		{
			n = int((n + 1) * g_fcCcs.dRate);
		}
		int h = g_fcCcs.CARD_UNCOVER - max(n, 0);
		r.top = org.y + (idx - 1)*h;
		r.bottom = r.top + (nCards - 1)*h + g_fcCcs.CARD_HEI;
	}
	else if (col <= 12) {
		r.left = org.x + (col - 9)*g_fcCcs.CARD_WID;
		r.top = org.y;
		r.right = r.left + g_fcCcs.CARD_WID;
		r.bottom = r.top + g_fcCcs.CARD_HEI;
	}
	else {
		org.x += 4 * g_fcCcs.CARD_WID + g_fcCcs.PILE_HINT;

		r.left = org.x + (col - 13)*g_fcCcs.CARD_WID;
		r.top = org.y;
		r.right = r.left + g_fcCcs.CARD_WID;
		r.bottom = r.top + g_fcCcs.CARD_HEI;
	}
	return r;
}

//#include "mainfrm.h"
//#include "JLView.h"

//�����Զ����ƹ��̶����ĺ���LineDDACallback
VOID CALLBACK LineDDACallback(int x, int y, LPARAM lparam)
{
	//ȡ���ĵ�����ͼָ�룬��ͼ�豸�����Ͷ�����
	//POSITION tmplPos = AfxGetApp()->GetFirstDocTemplatePosition();
	//CDocTemplate* pTmpl = AfxGetApp()->GetNextDocTemplate(tmplPos);
	//POSITION docPos = pTmpl->GetFirstDocPosition();
	//CJLDoc  *pDoc = (CJLDoc*)pTmpl->GetNextDoc(docPos);
	//CJLView *g_pView = pDoc->GetView();
	CClientDC cdc(g_pView->m_hWnd);
	UINT card = lparam;

	static CRect rbk(0, 0, 0, 0);//���ڼ�¼�ϴλ��Ƶ�λ��

	//��ǰλ��
	CRect r(x, y, x + g_fcCcs.CARD_WID, y + g_fcCcs.CARD_HEI), R, rInter;

	//��һ�㴦���������棬ֻ��¼λ��
	if (rbk.IsRectEmpty()) {
		rbk = r;
		return;
	}
	//���������ε��ص�����ﵽ�����75%ʱ���ҽ�Ҫ���Ƶ�
	//���β��ͻ��������ཻʱ�����µľ���
	//��һ�㷨�Ǿ����������֮��õ��ģ��㷨�ﵽ��Ч����
	//�����Ҳ�֪��windows�Ŀյ�������ʲô�㷨��
	//
	//�ٷֱȺͶ����ٶȵĹ�ϵ��
	//
	//	�����ٶȣ����� -----����-----> ��
	//	�ٷֱ�  ��100% ------50%-----> 1%
	//
	//�ϴλ����뱾�λ��Ƶ��������ε��ص�����

#define SquareIsOK(a,b) (a*b <= g_fcCcs.CARD_WID*g_fcCcs.CARD_HEI*(1-g_fcData.m_nDDASpeed/100.))
	// ����������ԭ���� g_fcData.m_nDDASpeed û�г�ʼ��

	rInter.IntersectRect(r, rbk);//�ص����ο϶���Ϊ��
	if (SquareIsOK(rInter.Width(), rInter.Height())) { //���������ϵ
		//��ȥ�ϴεĻ���
		g_pView->InvalidateRect(rbk);
		//�ص��Ĳ��ֲ���ˢ�£���Ϊ�»��Ƶľ��ΰ����ⲿ��
		g_pView->ValidateRect(rInter);
		//�ڵ�ǰλ�����»���
		g_pView->DrawCard(r.TopLeft(), card, &cdc);
		//cdc.BitBlt(r.left,r.top,r.Width(),r.Height(),&cdc,rbk.left,rbk.top,SRCCOPY);
		//ǿ������ˢ����Ч����
		g_pView->SendMessage(WM_PAINT, 0, 0);
		//������ϡ����ݵ�ǰ��������
		rbk = r;
	}
	//�ӽ����մ�ʱ���������һ��ǰ���ٻ���
	R = g_fcData.RectOf(TYPE(card) + 13, 1, 1);
	rInter.IntersectRect(rbk, R);
	if (!SquareIsOK(R.Width(), R.Height())) {
		g_pView->InvalidateRect(rbk);//��ȥ�ϴεĻ���
		//rbk.SetRectEmpty();
		return;
	}
}

//ȡ��col�е�idx����
UINT CWTLfcData::GetCard(UINT col, UINT idx)
{
	ATLASSERT(IsCol(col) && !IsEmptyCol(col) && idx > 0 && idx <= CntCardsIn(col));
	if (col <= 8) {
		UINT *pTop = &m_iCards[col - 1][0];
		return pTop[--idx];
	}
	else if (col <= 12) {
		ATLASSERT(idx == 1);
		return m_iBuffer[col - 9];//��idx�޹�
	}
	else {
		UINT *pTop = &m_iRecycle[col - 13][0];
		return pTop[--idx];
	}
}


//�浵
//void WTLfcData::OnSave()
//{
//	// TODO: Add your command handler code here
//	CFileDialog dlg(FALSE, TEXT("rep"), GetTitle(), dwFlags, filter);
//	if (dlg.DoModal() == IDCANCEL) return;
//	CFile file(dlg.GetPathName(), modeCrWr);
//	CArchive ar(&file, CArchive::store);
//	Serialize(ar);
//	ar.Close();
//	file.Close();
//}
//
////����
//void WTLfcData::OnLoad()
//{
//	// TODO: Add your command handler code here
//	if (!GiveUp()) return;
//
//	//ѡ���ļ�
//	CFileDialog dlg(TRUE, TEXT("rep"), NULL, dwFlags, filter);
//	if (dlg.DoModal() == IDCANCEL) return;
//	CFile file(dlg.GetPathName(), modeRead);
//
//	/////////////////////////////////////////////////////////////////
//	//���Ǵ浵�ļ��п�����һ�����������Զ����ƾ�
//	int nGameNumber;
//	file.Read(&nGameNumber, sizeof(int));
//	if (nGameNumber == -1) {
//		AfxMessageBox("�뽫�Զ����ƾ֡�" + dlg.GetFileName() + "���༭������\n");
//		return;
//	}
//	file.SeekToBegin();
//	/////////////////////////////////////////////////////////////////
//	m_dlgScore.UpdateScore();//��¼ս��
//
//	//����
//	CArchive ar(&file, CArchive::load);
//	Serialize(ar);
//	ar.Close();
//	file.Close();
//
//	//ˢ���ƾ�
//	UpdateAllViews(NULL);
//
//	//���ô������Ϊ��ǰ�ƾִ��Ż��Զ����ƾֵ��ļ���
//	CString title;
//	if (m_nCurGameNumber > 0) {
//		title.Format(TEXT("%d"), m_nCurGameNumber);
//		SetTitle(title);
//	}
//	else {
//		SetTitle(dlg.GetFileName());
//	}
//
//	m_dlgScore.InitScore();//��¼ս��
//	CheckGame();//�����˾��Ƿ��Ѿ�����
//}

//void CWTLfcData::ClrOpsRecords()
//{
//	if (m_pOps == NULL) return;
//	////////////////////////////////
//	//���ԭ���Ĳ�����¼
//	while (!m_pOps->empty())
//	{
//		delete m_pOps->front();
//		m_pOps->pop_front();
//	}
//	m_pOps->clear();
//}
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

/*�Զ�����㷨ʹ��ȫ�ֵĵݹ��㷨�;ֲ��Ļ��ݷ��㷨���ϣ��������£�
�Զ�����㷨��
(1)  ��1-4��9-16���� ÿһ�� ���ƿ����ƶ��������е� �� ִ��(2)
�������ƿ����ƶ���������û�пյ���û���򷵻�false����·��ͨ����
����пյ��򣨱�ʾ1-4��9-16��ȫ�ǿյģ����ɹ�������true��
(2)  �Դ����Ƶ�ÿһ��Ŀ����(Ŀ���п��ܲ�ֹһ��)��ʹ�������㷨��
(3)  ������п��Ժϲ��������У��򽫴��кϲ��������С����
�������п��Ժϲ�������
(4)  �Զ�����
(5)  �����Զ�����㷨
���������true�򷵻�true
�������������е��ƶ�������(3)(4)�е����ƶ�����������false
(6)  ����false
*/

//-----------------------------------------------
volatile bool g_bStopAICal;//���ֹͣ���Զ����
//-----------------------------------------------
//�Զ����
bool CWTLfcData::AICal()
{
	//��ҵȵĲ��ͷ���
	if (g_bStopAICal) {
		//�������̵Ķ���Ч���رգ���������˲��
		//�������еĽ����
		//m_bRealTimeUpdate = false;
		return false;
	}
	//�Զ����õ��Ĳ�����������ֵ������������־���
	//�˾ֺܿ��ܵ����㷨������ѭ��
	if (!m_OpsList.empty() && m_OpsList.size() > 200) {
		//g_bStopAICal = true;
		return false;
	}
	AutoThrow();
	return GameOver() || Combine() || Splite();//�Ⱥϲ����ϲ��˾Ͳ�
}
/*-------------------------------------------��-------------------------------------------
��ÿһ��������֮�����Ŀ���еķǿ���
{
���û��Ŀ����
��������ǻ�����
����п�����
����������ܹ���Լ�ռ�
���������������
����
���Ŀ����ֻ�ڻ����У���Ϊ����������Ŀ�궼�ڻ����У�
����п�����
������һ��Ŀ�������õ��������У��������         // �������°�Ŀ�����кϲ�����������

������������ǻ����У�������һ��Ŀ�������У�
�õ�����һ������Ŀ���������������

����������Ժϲ�������                                  ���ռ��㹻�������к����кϲ������У�
�������Ŀ���ϵõ�������������			// ��ʹ�ǳ�����ͬҲ����
��Ŀ��������ȫ����K��ʼ��������
�ϲ���Ŀ�������ϣ��������
}
���ؼ�

�������
��¼�˴ζ���
�Զ�����
���ִ���Զ����ɹ�������
����
���ؼ�
----------------------------------------------------------------------------------------*/
/*-----------------------------------------��---------------------------------------------
�𣺷ǿղŲ�

�𻺴��У�	�п����о�ֱ��������������ʧ��
�����У�	�����Ƿ���ȫ������
�Ⱥ�����п��пյ�����������
���������ȫ�����ƣ�
ȫ�𵽿յ�����������
������ٲ���һ�����л�յ��У��˿յ��в����ǸղŲ���ȥ���У���Ȼѭ���ˣ�
����Ҳ������Բ𶯵����У���������ҵ������з���ʧ��
----------------------------------------------------------------------------------------*/
//ִ�кϲ�����
bool CWTLfcData::Combine()
{
	for (UINT i = 1; i <= 12; i++) {
		if (IsEmptyCol(i)) continue;
		if (CombimeCol(i)) return true;
	}
	return false;
}
//ִ�в�ֶ���
bool CWTLfcData::Splite()
{
	//��ÿ���ɲ�ķ���ȫ���н��в��
	UINT cols[8 + 1], *pFirst = cols, *pLast = SortByActivity(cols);
	for (; pFirst < pLast; ++pFirst) {
		if (SpliteCol(*pFirst)) return true;
	}
	for (UINT i = 9; i <= 12; i++) {
		if (IsEmptyCol(i)) continue;
		if (SpliteCol(i)) return true;
	}
	return false;
}
//�ϲ����У������еģ������򲿷֣������ƺϲ���������
bool CWTLfcData::CombimeCol(UINT col)
{
	//�ϲ��Ķ���ֻ�����ǻ����л��������Ƿǿ���
	ATLASSERT(IsCol(col) && !ColInRecycle(col) && !IsEmptyCol(col));

	int desCol = 0, srcCol = 0, cntCards = 0;

	int tar[2];
	GetTarget(col, tar);//Ѱ�Ҵ��е�Ŀ����

	UINT ntar = 0;//����Ŀ������Ŀ
	bool bAllTarInBuf = false;//�Ƿ�����Ŀ�궼�ڻ�����
	if (tar[0]) {
		++ntar;
		bAllTarInBuf = ColInBuf(tar[0]);
		if (tar[1]) {
			++ntar;
			bAllTarInBuf = bAllTarInBuf && ColInBuf(tar[1]);
		}
	}
	//----------
	//û��Ŀ����
	//----------
	if (ntar == 0) {
		//�����Ƿǿ�����û��Ŀ����
		if (!ColInBuf(col)) return false;
		//�����ǻ�����û��Ŀ����
		int a = CntEmptyBufs();//a�ǿյ���
		int b = CntEmptyCardCols();//b�ǿ�������
		//���û�п�����
		if (b == 0) return false;
		//����п�����
		int c = (2 * a + b)*(b + 1) / 2 + 1;//�ƶ�֮ǰ�Ŀռ�
		++a, --b;//����յ����ӿ����м���
		int d = (2 * a + b)*(b + 1) / 2 + 1;//�ƶ�֮��Ŀռ�
		if (c >= d) return false;
		//�����ӿռ䣬�õ������У��������
		srcCol = col;
		desCol = FindEmptyCardCol();
		cntCards = 1;
#ifdef DEBUG_ALERT
		ShowMessage("�ϲ������е������У����ӿռ�", srcCol, desCol, cntCards);
#endif
		goto doAI;
	}
	//���Ŀ���ж��ڻ�����
	//--------------------
	else if (bAllTarInBuf) {
		int empCardCol = FindEmptyCardCol();
		if (!empCardCol)return false;//����п�����
		//������һ��Ŀ�������õ��������У��������
		srcCol = tar[0];
		desCol = empCardCol;
		cntCards = 1;
#ifdef DEBUG_ALERT
		ShowMessage("�ϲ������е������У��������ƿ��Ժϲ�������",
			srcCol, desCol, cntCards);
#endif
		goto doAI;
	}
	//������һ��Ŀ����������
	//------------
	//�����ǻ�����
	else if (ColInBuf(col)) {
		//�������õ�����Ŀ���������������
		srcCol = col;
		desCol = tar[ntar == 1 ? 0 : (ColInCard(tar[0]) ? 0 : 1)];
		ATLASSERT(tar[0]);
		cntCards = 1;
#ifdef DEBUG_ALERT
		ShowMessage("�ϲ������е�����", srcCol, desCol, cntCards);
#endif
		goto doAI;
	}
	//�������������
	else {
		srcCol = col;
		if (ntar == 1) { //����һ��Ŀ�������Ŀ���п϶�������
			desCol = tar[0];
		}
		else {
			//������Ŀ���У��������һ��������Ŀ����������
			if (!ColInCard(tar[0])) { //tar[0]����������tar[1]�϶�������
				desCol = tar[1];
			}
			else if (ColInCard(tar[1])) { //����tar��������
				desCol = tar[CntSeriIn(tar[0]) > CntSeriIn(tar[1]) ? 0 : 1];
				//�Ⱥϲ��������Ƴ���Ŀ����
			}
			else {
				desCol = tar[0];
			};
		}
		cntCards = CntMaxMv(desCol, srcCol);
		ATLASSERT(cntCards > 0);
		//if( cntCards + CntSeriIn(desCol) <= CntSeriIn(srcCol) ) return false;
		//�ɳ����кϲ��������б������ƶ���Դ��¶�����Ʋ���
		if (cntCards + CntSeriIn(desCol) <= CntSeriIn(srcCol))
			if (!Trashable(GetCard(srcCol, CntCardsIn(srcCol) - cntCards)))
				return false;
		//���Ժϲ�������
#ifdef DEBUG_ALERT
		ShowMessage("�ϲ����е����У����ܵõ�����������", srcCol, desCol, cntCards);
#endif
		goto doAI;
	}
	return false;
doAI:	//���ƿ����ƶ�Ŷ
MoveCards(desCol, srcCol, cntCards);//�ƶ�
//Record(new COperations(desCol, srcCol, cntCards));//��¼�ƶ�����
Record(desCol, srcCol, cntCards);//��¼�ƶ�����
AutoThrow();//�Զ����ƣ��Զ���¼������
if (AICal())return true;//�ɹ����
Undo();
	return false;
}

/*
�ӣ����� | ���� ---> ����
�ϣ����� | ���� ---> ����
1. �����������õ���������¶�����л�ʣ����
2. �����Ʋ����õ���������¶������
3. �������õ���������
4. �������õ����г�Ϊ������

������ ---> ���� | �յ�
*/
bool CWTLfcData::SpliteCol(UINT col)
{
	//��Ķ���ֻ�����ǻ����л������ҷǿղŲ�
	ATLASSERT(IsCol(col) && !ColInRecycle(col) && !IsEmptyCol(col));
	if (ColInBuf(col))//�𻺴���
	{
		//û�п���������������
		UINT empCardCol = FindEmptyCardCol();
		if (empCardCol == 0)return false;
		//�п�����
		//����������ܸ�������ṩ�ϵĻ����������
		int tar[2];
		GetTarget(col, tar);//Ѱ��Ŀ����
		if (!tar[0] && !tar[1])return false;
#ifdef DEBUG_ALERT
		ShowMessage("�𻺴��У������ܺϲ���������", col, empCardCol, 1);
#endif
		MoveCards(empCardCol, col, 1);//��¼�ƶ�����
		//Record(new COperations(empCardCol, col, 1));
		Record(empCardCol, col, 1);
		if (AICal())return true;
		Undo();
		return false;
	}
	//������
	UINT empCardCol = FindEmptyCardCol();
	if (empCardCol)//�ܹ�ֱ���ƶ��������У�
	{
		int nCntCards = CntCardsIn(col);
		int nFitFomula = CntSeriIn(col);
		int nMovableCards = CntMaxMv(empCardCol, col);
		//ʵ�ʿ��ƶ����ƿ϶���������������
		if (nMovableCards == nFitFomula)
		{
			//��ȫ��������ֱ���Ƶ�������û������
			if (nFitFomula == nCntCards)return false;
#ifdef DEBUG_ALERT
			ShowMessage("�������Ƶ�������", col, empCardCol, nMovableCards);
#endif
			//����ȫ�������е�ȫ��������ֱ���Ƶ�������
			MoveCards(empCardCol, col, nMovableCards);
			//Record(new COperations(empCardCol, col, nMovableCards));
			Record(empCardCol, col, nMovableCards);
			if (AICal())return true;
			Undo();
			return false;
		}//else �����Ʋ���ֱ���õ������У�����Ҫ�����𣬿�����
	}
	else { //������

		//�Ⱥ�����У����м��յ�����������
		int inUse[12];//��¼ʹ�ù��Ŀռ�
		int steps = 0;//��¼ʹ���˶��ٿռ�
		int nMoved = 0;//��¼�ƶ���������
		int nCntCard = CntCardsIn(col);
		int nCntSeri = CntSeriIn(col);
		int tarCol = 0, empCardCol = 0, empBufCol = 0;
		while (nMoved != nCntSeri)
		{
			int tar[2];
			GetTarget(col, tar);//Ѱ��Ŀ����
			bool t0 = ColInCard(tar[0]);
			bool t1 = ColInCard(tar[1]);
			//û�п�����
			//������Բ��ֺϲ���������ʱ���ϲ�����������
			if (t0 || t1)
				tarCol = tar[t0 ? 0 : 1];
			else if ((empCardCol = FindEmptyCardCol()) != 0)//����������п�����
				tarCol = empCardCol;
			else if ((empBufCol = FindEmptyBuf()) != 0)//����������пյ�
				tarCol = empBufCol;
			else//���򣨲𲻿�������һ�У�
			{
				while (steps--)Undo();//����
				return false;
			}
			int n = CntMaxMv(tarCol, col);
			ATLASSERT(n > 0);
#ifdef DEBUG_ALERT
			ShowMessage("�������Ƶ����У����м��յ���", col, tarCol, n);
#endif
			MoveCards(tarCol, col, n);//��¼�ƶ�����
			//Record(new COperations(tarCol, col, n));
			Record(tarCol, col, n);
			nMoved += n;//�������ߵ�����
			inUse[steps++] = tarCol;//��¼��ǰʹ�õ�Ŀ����
			//�˳�ѭ��ʱ��step��¼ʹ���˶��ٿռ�

			//�����������ߺ�¶�����Ƶ����������
			//�ϲ��������Ѿ����Կ��ǹ��ˣ��ڴ˾Ͳ����ٿ�����
			/*
			if(nMoved < nCntSeri) {
			UINT bc = BottCard(col);
			if(Trashable(bc)) {
			MoveCards(TYPE(bc)+13,col,1);
			Record(new COperations(TYPE(bc)+13,col,1));
			if(AICal()) return true;
			Undo();
			}
			}
			*/
		}
		//���в���ȫ��������
		//------------------
		if (nCntCard != nCntSeri){
			if (AICal()) return true;
			while (steps--)Undo();//���ɹ���ȫ������
			return false;
		}
		//������ȫ��������
		//----------------
		//������ٲ���һ���У������������У�Ҳ�����ǻ����У�
		//�����Բ��������ڱ�ʹ�õ��У�Ҳ�������ǲ���˵ĵ�ǰ��	
		for (int another = 1; another <= 12; another++) {
			ATLASSERT(IsEmptyCol(col));
			//col�д�ʱΪ�����Կ��Ա����˵�
			if (IsEmptyCol(another))continue;
			bool isInUse = false;
			for (int j = 0; j < steps; j++){
				if (another != inUse[j])continue;
				isInUse = true;
				break;
			}
			//���˵��ոձ�ʹ�õ���
			if (isInUse)continue;
			if (SpliteCol(another))return true;//�ɹ����
			while (steps--)Undo();//���ɹ���ȫ������
			return false;
		}
	}

	return false;
}

//�ҵ�һ��������
UINT CWTLfcData::FindEmptyCardCol()
{
	for (UINT i = 1; i <= 8; i++)
		if (m_iCards[i - 1][19] == 0) return i;
	return 0;
}

//�ҵ�һ���յ�
UINT CWTLfcData::FindEmptyBuf()
{
	for (UINT i = 9; i <= 12; i++)
		if (m_iBuffer[i - 9] == 0) return i;
	return 0;
}

//ͳ�ƿ�������
UINT CWTLfcData::CntEmptyCardCols()
{
	int cnt = 0;
	for (UINT i = 1; i <= 8; i++)
		if (m_iCards[i - 1][19] == 0) ++cnt;
	return cnt;
}

//ͳ�ƿյ���
UINT CWTLfcData::CntEmptyBufs()
{
	int cnt = 0;
	for (UINT i = 9; i <= 12; i++)
		if (m_iBuffer[i - 9] == 0) ++cnt;
	return cnt;
}


//Ϊָ����Դ��Ѱ��Ŀ���У�Ŀ���п�����һ�������������Ǿ��Բ��ᳬ������
//���û��Ŀ���У��򷵻�ʱ��target[0]��target[1]������
//�����ǻ�������Ѱ��Ŀ����
void CWTLfcData::GetTarget(int col, int *target)
{
	ATLASSERT(IsCol(col) && !ColInRecycle(col) && !IsEmptyCol(col));

	int *p = target;
	p[0] = p[1] = 0;
	for (UINT i = 1; i <= 12; i++) {
		if (i > 8) {
			int d = m_iBuffer[i - 9];
			if (!IS_CARD(d)) continue;//���Կյ�
			int s = BottCard(col);
			int n = NUM(d) - NUM(s);
			int nSeri = CntSeriIn(col);
			if (n > 0 && n <= nSeri && n % 2 == (TYPE(s) + TYPE(d)) % 2) {
				*p++ = i;
			}
		}
		else if (m_iCards[i - 1][19] && CntMaxMv(i, col)) {
			*p++ = i;//Ŀ��������
		}
		if (p == target + 2) return;//Ŀ���о��Բ��ᳬ������
	}
}

#ifdef DEBUG_ALERT
//���Գ����ʱ���뽫DEBUG_ALERT���뵽Ԥ���뿪����
void CWTLfcData::ShowMessage(char* pMsg, int src, int des, int cnt)
{
	ATLASSERT(pMsg != NULL);

	CMsgDlg msgdlg;
	CString msg;
	msg.Format("%d--->%d(%d),%s", src, des, cnt, pMsg);

	msgdlg.m_strMsg = msg;
	msgdlg.DoModal();
}
#endif

//�����Ϸ�Ƿ����
//���û�н����������ʾ����
//����������ѡ���Ƿ񿪾ֻ�طŴ浵
void CWTLfcData::CheckGame()
{
	if (!GameOver())
	{
		GetHints();
		if (m_Hints.IsEmpty())
		{
			MessageBoxW(g_pView->m_hWnd, _T("û������һ���ˣ�\n����Գ������������ԡ�"), NULL, NULL);
			return;
		}
		return;
	}
	//���ֽ�����

	CDlgPassed dlg;
	BOOL b;
choice:	if (IDCANCEL == dlg.DoModal())
	return;
		switch (dlg.m_nChoice)
		{
		case RandGame:	OnRand();     break;
		case PrevGame:	g_pView->OnPrevGame(NULL, NULL, NULL, b); break;
		case NextGame:	g_pView->OnNextGame(NULL, NULL, NULL, b); break;
		case ThisGame:  g_pView->OnAgain(NULL, NULL, NULL, b);    break;
		//case SaveGame:	g_pView->OnSave(NULL, NULL, NULL, b);     goto choice;
		case PlayShow:
			//��Ϸ���ص���ͷ���Ǳ��������¼
			BackHome();

			CDlgAIShow dlgAIShow;
			dlgAIShow.DoModal();//�طŶԻ���
			goto choice;
		}
}

//ִ���Զ����
bool CWTLfcData::DoAICal(void)
{
	//���ݡ������ƶ���ѡ���ֵ
	BOOL quickmv_bk = m_bQuickMove;

	//�Զ�����ʱ�Ķ���Ч��������ʱ�ر�
	m_bQuickMove = TRUE;

	//��ʼ���
	m_bAICalRunning = true;
	g_bStopAICal = false;
	m_bRealTimeUpdate = false;
	bool bSuccess = AICal();
	m_bAICalRunning = false;
	g_bStopAICal = true;

	//�ָ��������ƶ���ѡ���ֵ
	m_bQuickMove = quickmv_bk;

	return bSuccess;
}

//�����Ҫ��ʾ������Ϣ�ľ���
CRect CWTLfcData::RectOfStep()
{
	CRect r;
	r.UnionRect(RectOf(12, 1, 1), RectOf(13, 1, 1));

	int inflateY = r.Height() - g_fcCcs.stepFont * 4 / 3;
	r.InflateRect(-g_fcCcs.CARD_WID - 1, 0, -g_fcCcs.CARD_WID - 1, -inflateY);
	return r;
}

//��������ť�ľ���λ��
CRect CWTLfcData::RectOf(UINT card)
{
	CRect r, r11, r14;
	r11 = RectOf(11, 1, 1);
	r14 = RectOf(14, 1, 1);
	r.left = r11.left + g_fcCcs.CARD_WID / 2;

	UINT rWid = (r14.right - r11.left - g_fcCcs.CARD_WID) / 13;
	r.left += (13 - NUM(card))*rWid;
	r.right = r.left + rWid + 1;
	r.top = 0;
	r.bottom = g_fcCcs.ptOrg.y - 1;
	return r;
}

//������һ�������п��ܵĶ�������¼�����ǣ�
void CWTLfcData::GetHints()
{
	//���ԭ���ļ�¼
	m_Hints.ClrHints();
	if (GameOver()) return;

	UINT nMove, i, j;
	//���Ǻϲ��ͻ��շ���
	for (i = 1; i <= 12; i++) {
		if (IsEmptyCol(i)) continue;
		UINT bc = BottCard(i);
		if (Trashable(bc)) {
			m_Hints.AddOperation(Type(bc) + 13, i, 1);//��ʾ���յ��ƣ����ƣ�
		}
	}
	for (i = 1; i <= 12; i++) {
		if (IsEmptyCol(i)) continue;
		for (j = 1; j <= 8; j++) {
			if (IsEmptyCol(j) || !(nMove = CntMaxMv(j, i))) continue;
			//�ϲ�Ҫ�õ��ϳ����л��з��ƿ����ӵ�
			if (CntSeriIn(i) - nMove < CntSeriIn(j) ||
				Trashable(GetCard(i, CntCardsIn(i) - nMove))) {
				m_Hints.AddOperation(j, i, nMove);
			}
		}
	}
	if (!m_Hints.IsEmpty()) return;

	//���ǲ������ȫ������
	for (i = 1; i <= 8; i++) {
		if (IsEmptyCol(i) || CntSeriIn(i) == CntCardsIn(i)) continue;
		for (j = 1; j <= 12; j++) { //�����ǻ�������Ϊ�޷���
			if (!IsEmptyCol(j)) continue;
			m_Hints.AddOperation(j, i, CntMaxMv(j, i));
			break;//�����ж�����У���ֻ��ʾ�ƶ�������֮һ�͹���
		}
	}
}
/*
////////////////////////////////////////////////////////////////
�����ӿռ�������ȿ��ǲ����
////////////////////////////////////////////////////////////////

// ��ּ�ߺϲ�
����ÿһ����ȫ���������У�
�����ջ�����ռʣ���Ƶı����ɸߵ��ͽ������򣬶�������ÿһ�У�
���������������ȫ���ߣ�ͨ���ϲ����������У��𵽿����У����ƶ����յ���
��ȫ��

�����ݻ����е��ƴӴ�С��˳��
// ���ӿռ�������������
��������ƿ����õ�������������
// ���ӿռ�������������
��������������������������ӿռ���������
// ���������Ƶ����ٿռ�
������������������ܳ�Ϊ��������������

// ���ӿռ䵫����������
����ÿһ��ȫ���������У�
���������������ȫ�Ƶ��յ�������
�������ӿռ�
��ȫ��
�����û�п�����
���õ��յ�������һ��������

�������Զ����
*/
//���ڷǿ����е��й���Ϣ�����У�����Ŀ�������Ƴ���������
struct COL_INF {
	UINT col, nCards, nSeri, act;
	void Set(UINT a, UINT b, UINT c, UINT d) {
		col = a; nCards = b; nSeri = c; act = d;
	}
};
/*
int CmpAct( const void *arg1, const void *arg2 )
{
COL_INF *p1 = (COL_INF*)arg1;
COL_INF *p2 = (COL_INF*)arg2;

int res = p1->act - p2->act;

if(p1->act == 0) {
if(p2->act == 0) { //���޻��ƾ��Ȳ����ٵ���
return p2->nCards - p1->nCards;
} else { //�л��Ƶ��б��޻��Ƶ����Ȳ�
return res;
}
} else {
if(p2->act == 0) { //�л��Ƶ��б��޻��Ƶ����Ȳ�
return res;
} else { //���л���
int diff = p1->act * (p2->nCards - p2->nSeri) - p2->act * (p1->nCards - p1->nSeri);
if(diff == 0) { //���Ʊ���һ����
if(p1->act == p2->act) { //����һ����
return p2->nSeri - p1->nSeri; //�Ȳ������ƶ̵���
} else { //���Ʋ�һ����
return res; //����ƶ����
}
} else { //�����Ȳ���ƣ�ռʣ���Ƶģ������ϴ����
return diff;
}
}
}

return res;
}
*/
//�����еķǿ����У����ݻ�������ÿ���ǿ����н�������
//������������������ǰ
UINT* CWTLfcData::SortByActivity(UINT *pCols)
{
	char bc[8 + 1], rc[4 + 1], *p, i;
	for (p = bc, i = 1; i <= 8; i++) { //��ȡ���Ƽ�
		if (!IsEmptyCol(i)) *p++ = BottCard(i);
	} *p = 0;
	for (p = rc, i = 13; i <= 16; i++) { //��ȡ���Ƽ�
		if (!IsEmptyCol(i)) *p++ = BottCard(i);
		else *p++ = 0;
	} *p = 0;

	/*
	//ͳ�Ʒǿ����е��й���Ϣ�����У�����Ŀ�������Ƴ���������
	COL_INF f[8+1],*pLast = f;
	for(i = 1 ; i <= 8 ; i++) {
	if(IsEmptyCol(i)) continue;

	UINT *pTop = &m_iCards[i-1][0];//pTopָ������ƵĶ���
	UINT nCards = pTop[19], nSeri = CntSeriIn(i);
	UINT *pCur = pTop + nCards-nSeri - 1 ;//pCurָ��ʣ���Ƶĵ���

	char b[8+1], r[4+1]; //���Ƶ��Ƽ��ͷ��Ƽ��ֱ�b��r
	strcpy(b,bc);
	strcpy(r,rc);

	UINT nAct = 0; //ͳ�ƴ��еĻ�����Ŀ
	while(pCur >= pTop) { //����ʣ������ͳ�ƻ�����
	char *pTarg = FindActiveCard(*pCur,b,r);
	if(pTarg) {
	*pTarg = *pCur;//ģ����Ʊ��ÿ��������У��õ��������л�յ�������У�
	++nAct;//ͳ�ƴ��еĻ�����Ŀ
	}
	--pCur;
	}
	pLast++->Set(i,nCards,nSeri,nAct);//�����йش����Ƶ���Ϣ
	} pLast->col = 0;
	//�����а��ջ���������
	qsort( (void*)f, (pLast-f)/sizeof(COL_INF), sizeof(COL_INF), CmpAct);
	//�������������е�����������
	for(COL_INF * pFirst = f; pFirst < pLast; ) {
	*pCols++ = pFirst++->col;
	} *pCols = 0;
	return pCols;
	*/
	//ͳ�Ʒǿ����е��й���Ϣ�����У�����Ŀ�������Ƴ���������
	COL_INF f[8 + 1], *pLast = f;
	for (i = 1; i <= 8; i++) {
		if (IsEmptyCol(i)) continue;

		UINT nCards = CntCardsIn(i);
		UINT nSeri = CntSeriIn(i);
		UINT nAct = 0;

		//ͳ�ƴ��еĻ�����Ŀ
		char b[8 + 1], r[4 + 1];
		//strcpy(b, bc);
		//strcpy(r, rc);//���Ƶ��Ƽ��ͷ��Ƽ��ֱ�b��r
		strcpy_s(b, bc);
		strcpy_s(r, rc);//���Ƶ��Ƽ��ͷ��Ƽ��ֱ�b��r

		UINT *pTop = &m_iCards[i - 1][0];//pTopָ������ƵĶ���
		UINT *pCur = pTop + pTop[19] - 1 - nSeri;//pCurָ��ʣ���Ƶĵ���
		while (pCur >= pTop) {
			char *pAct = FindActiveCard(*pCur, b, r);
			if (pAct) {
				*pAct = *pCur;
				++nAct;//ͳ�ƴ��еĻ�����Ŀ
			}
			--pCur;
		}
		//�����йش����Ƶ���Ϣ
		pLast++->Set(i, nCards, nSeri, nAct);
	}
	pLast->col = 0;

	//�����а��ջ���������
	COL_INF *pFirst = f;
	while (pFirst < pLast) {
		COL_INF *p = pLast - 1;
		while (--p >= pFirst) {
			if (p->act < (p + 1)->act) {
				COL_INF t = p[0];
				p[0] = p[1];
				p[1] = t; //����λ��
			}
		}
		++pFirst;//���������������Ѿ��ŵ���ǰ��
	}
	//�������������е�����������
	for (pFirst = f; pFirst < pLast;) {
		*pCols++ = pFirst++->col;
	} *pCols = 0;
	return pCols;
}

//���������Ƿ���Ի���
bool CWTLfcData::Trashable(UINT card)
{
	ATLASSERT(IsCard(card));

	UINT type = TYPE(card);
	if (IsEmptyCol(type + 13)) return NUM(card) == 1;//ֻ��A���Է������

	return FitTrash(card, BottCard(type + 13));//���뻨ɫ���������������
}

//����card�Ƿ�Ϊ����
char * CWTLfcData::FindActiveCard(UINT card, char *b, char *r)
{
	ATLASSERT(IsCard(card));

	//�鿴���Ƽ������Ƿ�����õ���������
	for (; *b; ++b) if (FitFormula(*b, card)) return b;

	//�鿴���Ƽ������Ƿ���ϻ��չ���
	UINT type = TYPE(card);
	UINT num = NUM(card);
	if (r[type] == 0) return num == 1 ? &r[type] : 0; //ֻ��A���Է������

	return FitTrash(card, r[type]) ? &r[type] : 0;//���뻨ɫ���������������
}

//����clock��rand������һ�������
//���ǿ��Բ���һ�����������ʹsrandÿ������ͬһ�����ӣ�
//���ô˺�����Ҳ����õ���ͬ��������������ȽϺã�
//��Ϊ������������������ѽ
int CWTLfcData::Random(void)
{
	int n = ((rand() << 16) | (clock() & 0xFFFF)) & MAX_GAME_NUMBER;
	return max(n, 1);
}

//BOOL WTLfcData::CanCloseFrame(CFrameWnd* pFrame)
//{
//	// TODO: Add your specialized code here and/or call the base class
//	if (!GameOver() && !m_pOps->IsEmpty())
//		if (IDNO == AfxMessageBox(TEXT("������ô��"), MB_YESNO))
//			return FALSE;
//
//	return CDocument::CanCloseFrame(pFrame);
//}

BOOL CWTLfcData::GiveUp()
{
	//������Ϊ�¾ֻ��ѽ�����ʱ�򣬿��Կ�ʼ��һ��
	if (m_OpsList.empty() || GameOver()) return true;
	//����Ҫ��������Ƿ������ǰ��
	//return IDYES == AtlMessageBox((HWND)_Module.get_m_hInst(),TEXT("������ǰ��Ϸ��"), MB_YESNO);
	//return IDYES == AtlMessageBox(g_pView->m_hWnd, TEXT("������ǰ��Ϸ��"), MB_YESNO);
	return IDYES == ::MessageBox(g_pView->m_hWnd, TEXT("������ǰ��Ϸ��"), TEXT("WTLfc"), MB_YESNO);
}

//������֣������ٲ����Ѿ����ֹ��ľ�
void CWTLfcData::OnRand()
{
	// TODO: Add your command handler code here
	if (!GiveUp()) return;
	int nUniqueGame = Random();
	while (m_dlgScore.IsOldGameNumber(nUniqueGame = Random()));
	StartGame(nUniqueGame);
}

////��һ��
//void CWTLfcData::OnPrevGame()
//{
//	// TODO: Add your command handler code here
//	if (!GiveUp()) return;
//	StartGame(max(m_nCurGameNumber - 1, MIN_GAME_NUMBER));
//}
////��һ��
//void CWTLfcData::OnNextGame()
//{
//	if (!GiveUp()) return;
//	StartGame(min(m_nCurGameNumber + 1, MAX_GAME_NUMBER));
//}
////����
//void CWTLfcData::OnAgain()
//{
//	// TODO: Add your command handler code here
//	if (!GiveUp()) return;
//	if (m_nCurGameNumber > 0) {
//		StartGame(m_nCurGameNumber);
//	}
//	else {
//		while (!m_pOps->IsEmpty()) {
//			Undo();//��������ͷ
//		}
//	}
//}
//
////ս��
//void WTLfcData::OnScore()
//{
//	m_dlgScore.DoModal();
//}
//
////ѡ��
//void WTLfcData::OnSelectgamenumber()
//{
//	if (!GiveUp()) return;
//
//	CDlgSelGame dlg;
//	dlg.m_nGameNumber = m_nCurGameNumber;
//	if (dlg.DoModal() != IDOK) return;
//
//	StartGame(dlg.m_nGameNumber);
//}
//���ݸ������ƾִ��ſ�ʼ�˾�
void CWTLfcData::StartGame(int gameNumber)
{
	m_dlgScore.UpdateScore();//��¼ս��

	//ClrOpsRecords();			//���������¼
	UnselectCardCol();		//���ѡ�б�־
	m_Hints.ClrHints();		//�����ʾ

	m_nCurGameNumber = gameNumber;
	Shuffle();				//ϴ�Ʒ���
	GetHints();
	//UpdateAllViews(NULL);	//���ƽ���
	g_pView->RedrawWindow();

	CString title;			//���ô������Ϊ��ǰ�ƾִ���
	title.Format(TEXT("%10d"), m_nCurGameNumber);
	//SetTitle(title);

	m_dlgScore.InitScore();//��¼ս��
}

//���ݸ����ı�ǩ�����˱�ǩָ������
CARD_POS * CWTLfcData::FindCardForLabel(UINT cardLabel, CARD_POS * pos)
{
	for (UINT col = 1; col <= 8; col++) {
		UINT n = CntCardsIn(col);
		for (UINT idx = 1; idx <= n; idx++) {
			if (NUM(cardLabel) == NUM(GetCard(col, idx))) {
				pos->col = col;
				pos->idx = idx;
				++pos;
			}
		}
	}
	return pos;
}
////�Զ����ƾ�
//void CWTLfcData::OnEdit()
//{
//	CDlgDefGame dlg;
//	dlg.DoModal();
//}
