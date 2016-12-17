#if !defined __DATATYPE_H__
#define __DATATYPE_H__

#define CARD(num,type)   (num + type*13)
#define IS_CARD(card)    (card<=52 && card>=1)
#define TYPE(card)       ((card-1)/13)
#define NUM(card)        ((card-1)%13+1)

#define COL_IN_CARD(c)   (c<=8)
#define COL_IN_BUFF(c)   (c>=9 && c<=12)
#define COL_IN_RECY(c)   (c>=13)

#include <list>
using namespace std;

#include "WTLSerialize/File.h"
#include "WTLSerialize/Archive.h"

/*
const CPoint ptOrg(1,17);//�ƾ����Ͻ�

const CARD_HEI = 94; // ÿ���Ƶĸ߶�height
const CARD_WID = 69; // ÿ���ƵĿ��width
const CARD_INT = 8;
	// ����1-8����ÿ������֮��ļ��(interval)
	// ����9,16���зֱ��봰�����ߵļ��Ҳ�����ֵ��
const PILE_VINT = 8; // �����뻺���м䴹ֱ�����ϵļ��
const PILE_HINT = 9*CARD_INT; //�������������֮���ˮƽ���
const CARD_UNCOVER = 18; // ����һ����ѹ�ŵ�ʱ�򣬴�¶���Ĳ��ֵĸ߶�

//usage: clr[BIG/SML][HT/HX/MH/FK][UP/DN][X/Y]...
const UCHAR BIG=0, SML=1, big=16, sml=8;
const UCHAR clr[2][4][2][2] = {
		{	{	{0 *big,1 *big},//������
				{1 *big,1 *big},//������
			},{	{0 *big,2 *big},//������
				{1 *big,2 *big},//������
			},{	{0 *big,3 *big},//÷����
				{1 *big,3 *big},//÷����
			},{	{0 *big,4 *big},//������
				{1 *big,4 *big},//������
			}
		},{	{	{0 *sml,0 *sml},
				{0 *sml,1 *sml}
			},{	{1 *sml,0 *sml},
				{1 *sml,1 *sml}
			},{	{2 *sml,0 *sml},
				{2 *sml,1 *sml}
			},{	{3 *sml,0 *sml},
				{3 *sml,1 *sml}
			}
		}
	};
//��A������10��ͼ������
const UCHAR x12   = CARD_WID      /  2;
const UCHAR x13   = CARD_WID *  9 / 30;
const UCHAR x23   = CARD_WID * 21 / 30;
const UCHAR y12   = CARD_HEI      /  2;
const UCHAR y15   = CARD_HEI      /  5;
const UCHAR y25   = CARD_HEI *  2 /  5;
const UCHAR y35   = CARD_HEI *  3 /  5;
const UCHAR y45   = CARD_HEI *  4 /  5;
const UCHAR y310  = CARD_HEI *  3 / 10;
const UCHAR y710  = CARD_HEI *  7 / 10;
const UCHAR y720  = CARD_HEI *  7 / 20;
const UCHAR y1320 = CARD_HEI * 13 / 20;

const UCHAR cA[] = { x12 , y12 , 1, };
const UCHAR c2[] = { x12 , y15 , 1, x12 , y45 , 0, };
const UCHAR c3[] = { x12 , y15 , 1, x12 , y45 , 0, x12 , y12 , 1, };
const UCHAR c4[] = { x13 , y15 , 1, x23 , y15 , 1, x13 , y45 , 0, x23 , y45 , 0, };
const UCHAR c5[] = { x13 , y15 , 1, x23 , y15 , 1, x13 , y45 , 0, x23 , y45 , 0, x12 , y12 , 1, };
const UCHAR c6[] = { x13 , y15 , 1, x23 , y15 , 1, x13 , y45 , 0, x23 , y45 , 0, x13 , y12 , 1, x23 , y12 , 1, };
const UCHAR c7[] = { x13 , y15 , 1, x23 , y15 , 1, x13 , y45 , 0, x23 , y45 , 0, x13 , y12 , 1, x23 , y12 , 1, x12 , y720 , 1 , };
const UCHAR c8[] = { x13 , y15 , 1, x23 , y15 , 1, x13 , y25 , 1, x23 , y25 , 1, x13 , y35 , 0, x23 , y35 , 0, x13 , y45  , 0 , x23 , y45   , 0 };
const UCHAR c9[] = { x13 , y15 , 1, x23 , y15 , 1, x13 , y25 , 1, x23 , y25 , 1, x13 , y35 , 0, x23 , y35 , 0, x13 , y45  , 0 , x23 , y45   , 0 , x12 , y12  , 1 , };
const UCHAR c10[]= { x13 , y15 , 1, x23 , y15 , 1, x13 , y25 , 1, x23 , y25 , 1, x13 , y35 , 0, x23 , y35 , 0, x13 , y45  , 0 , x23 , y45   , 0 , x12 , y310 , 1 , x12 , y710 , 0 , };
const UCHAR c8FK[] = { x13 , y15 , 1, x23 , y15 , 1, x13 , y45 , 0, x23 , y45 , 0, x13 , y12 , 1, x23 , y12 , 1, x12 , y720 , 1 , x12 , y1320 , 0 , };

//�������������߶�
const UINT stepFont = 22;
*/

//const UINT modeCrWr = CFile::modeWrite | CFile::modeCreate;
//const UINT modeRead = CFile::modeRead;
//const DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
//static const wchar_t * filter = TEXT("�����浵�ļ�(*.rep)|*.rep||");//�ļ���������

////����
//void Decoding(CFile & desFile, CFile & srcFile);
//

//////////////////////////////////////////////////////////////
// CTList����ʵ�� list<T> m_tList; �Ĵ��л�

template <typename T>
class CTList : public CMyObject
{
public:
	list<T> m_tList;
	CTList(int n = 0) { m_tList.resize(n); }

	void Serialize(CXArchive& ar)
	{
		if (ar.IsStoring())
		{
			ar << m_tList.size();
		}
		else
		{
			int n;	ar >> n;		m_tList.resize(n);
		}

		for (list<T>::iterator it = m_tList.begin(); it != m_tList.end(); it++)
			(*it).Serialize(ar);
	}
};

// CTList����ʵ�� list<T> m_tList; �Ĵ��л�
//////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
class CMyObject
{
public:
	CMyObject() {}
	// ����������������Ҫdelete�ģ���ض���Ϊvirtual��������ܵ�������delete
	virtual ~CMyObject() {}

	////////////////////////////////////////////////////////////
	// ��ȡSTL list�е�ָ��Ԫ��ֻ���ñ�����
	//CMyObject* getAt(list<CMyObject*>* pList, unsigned n)
	template <typename T>
	T* getAt(list<T>& xList, unsigned n)
	{
		list<T>::iterator it = xList.begin();
		for (unsigned i = 0; i < n; i++)
			it++;

		return &(*it);
	}

};
////////////////////////////////////////////////////////////
class COperation : public CMyObject	// CObject ���滻Ϊ CMyObject
{
	//DECLARE_SERIAL(COperation)

public:
	UCHAR src,des,cnt;//��¼�ӵ�src���ƶ�cnt���Ƶ���des��

	//COperation() 
	//	: des(0),src(0),cnt(0) 
	//{ }

	COperation(UINT d = 0, UINT s = 0, UINT n = 0) : des(d),src(s),cnt(n) 
	{ }

	//void Serialize(CArchive &ar)	// ��Ĵ��л�֧�ֶ���ע�͵�
	//{
	//	//CObject does not serialize anything by default
	//	//CObject::Serialize(ar);
	//	if(ar.IsStoring()) { 
	//		ar<<src<<des<<cnt;
	//	} else {
	//		ar>>src>>des>>cnt;
	//	}
	//}

	void Serialize(CXArchive &ar)	// ��Ĵ��л�֧�ֶ���ע�͵�
	{
		//CObject does not serialize anything by default
		//CObject::Serialize(ar);
		if(ar.IsStoring()) { 
			ar<<src<<des<<cnt;
		} else {
			ar>>src>>des>>cnt;
		}
	}
};
////////////////////////////////////////////////////////////
//class COperation : public CObject
//{
//	DECLARE_SERIAL(COperation)
//
//public:
//	UCHAR src,des,cnt;//��¼�ӵ�src���ƶ�cnt���Ƶ���des��
//
//	COperation() 
//		: des(0),src(0),cnt(0) 
//	{ }
//
//	COperation(UINT d,UINT s,UINT n) 
//		: des(d),src(s),cnt(n) 
//	{ }
//
//	void Serialize(CArchive &ar) {
//		//CObject does not serialize anything by default
//		//CObject::Serialize(ar);
//		if(ar.IsStoring()) { 
//			ar<<src<<des<<cnt;
//		} else {
//			ar>>src>>des>>cnt;
//		}
//	}
//};

//////////////////////////////////////////////////////////////
// ����CTList<T>���������ʵ�� list<T> m_tList; �Ĵ��л�
////////////////////////////////////////////////////////////////
//// CMyOpList����ʵ�� list<COperation> m_opList; �Ĵ��л�
//class CMyOpList : public CMyObject
//{
//public:
//	list<COperation> m_opList;
//	CMyOpList(int n = 0) { m_opList.resize(n); }
//
//	void Serialize(CXArchive& ar)
//	{
//		if (ar.IsStoring())
//		{
//			ar << m_opList.size();
//		}
//		else
//		{
//			int n;	ar >> n;		m_opList.resize(n);
//		}
//
//		for (list<COperation>::iterator it = m_opList.begin(); it != m_opList.end(); it++)
//			(*it).Serialize(ar);
//	}
//};

//��MFC��CObList���Ͷ���ı����ɴ洢����󣬵��Ǹ�����Ҫ�̳�CObject�ࡣ��ΪCObList���CObject������ָ�롣
//ʾ����CObList list[num];
//ȡ��list�е�Ԫ��ʱ��ȡ������ָ�룬��Ҫ��CObject���͵�ָ���������ת�����£�
//Plan *oplan = (Plan*)list->GetHead();//ָ������ת��
//�����Ǵ����������
//Plan oplan = (Plan)*list->GetHead();//����ת��
//��
//Plan *oplan = list->GetHead();//����ת��
//��������ƴ���
//error C2440 : 'initializing' : cannot convert from 'class CObject *' to 'class Plan *'

////////////////////////////////////////////////////////////
class COperations : public CMyObject
{
public:
	COperations() 
	{	
		//m_pObjlist = new list<CMyObject*>;		// CObList ���滻ΪSTL�� list<CMyObject*>
	}

	//list<CMyObject*> *m_pObjlist;
	//list<COperation> m_opList;
	//CMyOpList m_opList;
	CTList<COperation> m_opList;

	void Serialize(CXArchive &ar)
	{
		//CObject does not serialize anything by default
		//CObject::Serialize(ar);
		m_opList.Serialize(ar);
	}

	COperations(UINT des, UINT src, UINT n)
	{
		//m_pObjlist = new list<CMyObject*>;
		//pOps->AddHead(new COperation(des,src,n)); 
		//m_pObjlist->push_front(new COperation(des, src, n));
		m_opList.m_tList.push_front(COperation(des, src, n));
	}

	void AddOperation(UINT des,UINT src,UINT n)
	{ 
		//pOps->AddHead(new COperation(des,src,n)); 
		//m_pObjlist->push_front(new COperation(des, src, n));
		m_opList.m_tList.push_front(COperation(des, src, n));
	}

	//////////////////////////////////////////////////////////////
	//// ��ȡSTL list�е�ָ��Ԫ��ֻ���ñ�����
	////CMyObject* getAt(list<CMyObject*>* pList, unsigned n)
	//template <typename T>
	//T* getAt(list<T>& opList, unsigned n)
	//{
	//	list<T>::iterator it = opList.begin();
	//	for (unsigned i = 0; i < n; i++)
	//		it++;

	//	return &(*it);
	//}

	//COperation* getAt(list<COperation>& opList, unsigned n)
	//{
	//	list<COperation>::iterator it = opList.begin();
	//	for (unsigned i = 0; i < n; i++)
	//		it++;

	//	return &(*it);
	//}

	//COperations* getAt(list<COperations>& opsList, unsigned n)
	//{
	//	list<COperations>::iterator it = opsList.begin();
	//	for (unsigned i = 0; i < n; i++)
	//		it++;

	//	return &(*it);
	//}

	// ��ȡSTL list�е�ָ��Ԫ��ֻ���ñ����ˡ���ͷ�ļ��ж����ģ�庯������ʵ����
	//template <typename T>
	//T listget(list<T>* pList, unsigned n)
	//{
	//	list<T>::iterator it = pList.begin();
	//	for (unsigned i = 0; i < n; i++)
	//		it++;
	//	
	//	return *it;
	//}

	//void ClrOps()
	//{
	//	// ��vector��ͬ�� list������list[i]�������list�м��Ԫ�أ�ֻ�ܴ����˷��ʡ�����ֻ����iterator
	//	for (list<CMyObject*>::iterator it = m_pObjlist->begin(); it != m_pObjlist->end(); ++it)
	//	{
	//		COperation *pOp = (COperation *)(*it);
	//		delete pOp;
	//	}
	//	m_pObjlist->clear();

	//	//POSITION p = pOps->GetHeadPosition();
	//	//while(p) {
	//	//	COperation *pOp = (COperation *)pOps->GetNext(p);
	//	//	delete pOp;
	//	//}
	//	//pOps->RemoveAll();
	//}

	// ����������������Ҫdelete�ģ���ض���Ϊvirtual��������ܵ�������delete
	//virtual ~COperations()
	//{ 
	//	ClrOps();
	//	delete m_pObjlist;
	//}

	//void Serialize(CArchive &ar)
	//{
	//	//CObject does not serialize anything by default
	//	//CObject::Serialize(ar);
	//	pOps->Serialize(ar);
	//}

	//DECLARE_SERIAL(COperations)
};

//////////////////////////////////////////////////////////////
//class COperations : public CObject
//{
//public:
//	COperations() 
//	{	
//		pOps = new CObList;
//	}
//
//	COperations(UINT des,UINT src,UINT n) 
//	{
//		pOps = new CObList;
//		pOps->AddHead(new COperation(des,src,n)); 
//	}
//
//	void AddOperation(UINT des,UINT src,UINT n) { 
//		pOps->AddHead(new COperation(des,src,n)); 
//	}
//
//	void ClrOps() {
//		POSITION p = pOps->GetHeadPosition();
//		while(p) {
//			COperation *pOp = (COperation *)pOps->GetNext(p);
//			delete pOp;
//		}
//		pOps->RemoveAll();
//	}
//
//	~COperations() { 
//		ClrOps();
//		delete pOps;
//	}
//
//	void Serialize(CArchive &ar) {
//		//CObject does not serialize anything by default
//		//CObject::Serialize(ar);
//		pOps->Serialize(ar);
//	}
//
//	CObList *pOps;
//	DECLARE_SERIAL(COperations)
//};
//

//////////////////////////////////////////////////////////////
// ����CTList<T>���������ʵ�� list<T> m_tList; �Ĵ��л�
////////////////////////////////////////////////////////////////
//// CMyOpsList����ʵ�� list<COperations> m_opsList; �Ĵ��л�
//class CMyOpsList : public CMyObject
//{
//public:
//	list<COperations> m_opsList;
//	CMyOpsList(int n = 0) { m_opsList.resize(n); }
//
//	void Serialize(CXArchive& ar)
//	{
//		if (ar.IsStoring())
//		{
//			ar << m_opsList.size();
//		}
//		else
//		{
//			int n;	ar >> n;		m_opsList.resize(n);
//		}
//
//		for (list<COperations>::iterator it = m_opsList.begin(); it != m_opsList.end(); it++)
//			(*it).Serialize(ar);
//	}
//};

////////////////////////////////////////////////////////////
//��¼��һ�������п��ܵ��ƶ�����������ʾ
class HINTS : public COperations
{
	int curHint;
public:
	HINTS() { curHint = 0; }

	//
	// before call NextHint, you must firstly 
	// call COperations::AddOperation to fill
	// the operation list, for it should NOT be 
	// empty
	//
	const COperation* NextHint(void)
	{
		//ATLASSERT(!m_pObjlist->empty());
		//int cnt = m_pObjlist->size();
		ATLASSERT(!m_opList.m_tList.empty());
		int cnt = m_opList.m_tList.size();
		if(curHint >= cnt) { curHint = 0; }

		//������ʾ��β�Ķ���
		//const COperation* pOp = (const COperation*)getAt(m_pObjlist, cnt - ++curHint);
		const COperation* pOp = getAt(m_opList.m_tList, cnt - ++curHint);
		
		//(const COperation*)m_pObjlist->GetAt(m_pObjlist->FindIndex(cnt - ++curHint));
		// GetAt ��ȡָ��λ�õ�Ԫ��
		// FindIndex ��ȡ���������ָ��Ԫ�ص�λ��
		// POSITION FindIndex(INT_PTR nIndex	) const;

		return pOp;
	}

	void ClrHints(void)
	{
		//ClrOps();
		m_opList.m_tList.clear();
		curHint = 0;
	}

	BOOL IsEmpty()
	{
		//return m_pObjlist->empty();
		return m_opList.m_tList.empty();
	}
};

//////////////////////////////////////////////////////////////
////��¼��һ�������п��ܵ��ƶ�����������ʾ
//class HINTS : public COperations
//{
//	int curHint;
//public:
//	HINTS() {
//		curHint = 0;
//	}
//	//
//	// before call NextHint, you must firstly 
//	// call COperations::AddOperation to fill
//	// the operation list, for it should NOT be 
//	// empty
//	//
//	const COperation* NextHint(void) {
//		ASSERT(!pOps->IsEmpty());
//		int cnt = pOps->GetCount();
//		if(curHint >= cnt) {
//			curHint = 0;
//		}
//		//������ʾ��β�Ķ���
//		const COperation* pOp = (const COperation*)
//			pOps->GetAt(pOps->FindIndex(cnt - ++curHint));
//		return pOp;
//	}
//	void ClrHints(void) {
//		ClrOps();
//		curHint = 0;
//	}
//	BOOL IsEmpty() {
//		return pOps->IsEmpty();
//	}
//};
//
///////////////////////////////////
struct CARD_POS { UINT col,idx; };
///////////////////////////////////

#define MIN_GAME_NUMBER 1
#define MAX_GAME_NUMBER 0x7FFFFFFF

#endif
