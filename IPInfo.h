/*
 File:		\Src\common\IPInfo.h
 Description:	����IP��ַ
 Author:	cdutboy@gmail.com
 Build:		2011.09.21
*/
 
#ifndef IPINFO_HEADER_FILE
	#define IPINFO_HEADER_FILE
/*
QQWry.dat�ж���Сβ�ֽ���
 
�ļ���
 
��¼��
 
������
*/
#define _WINSOCKAPI_ //// stops windows.h including winsock.h
#include <Windows.h>
/*#define IN
#define OUT
typedef bool BOOL;
typedef wchar_t WCHAR;
typedef unsigned long ULONG;
typedef void *HANDLE;
*/
#pragma pack(push,1)
 
#define IPINFO_INDEX_SIZE	7	//һ�������Ĵ�С
 
#pragma pack(pop)

class CIPInfo
{
public:
	CIPInfo(void);
	~CIPInfo();
 
	//�ڵ�ǰĿ¼����Ĭ�ϵļ�������,QQWry.dat,IPwry.dat,��
	BOOL OpenDefDat(void);
 
	BOOL OpenA(IN char*  pszDatPath);
	BOOL OpenW(IN WCHAR* pszDatPath);
 
	BOOL Close(void);
 
	BOOL QueryIPA(IN char* pszIP,OUT char* pszAddr,IN ULONG ulAddrBuffSize);
	BOOL QueryIPW(IN WCHAR* pszIP,OUT WCHAR* pszAddr,IN ULONG ulAddrBuffSize);
 
	BOOL QueryIPA(IN ULONG ulIP,OUT char* pszAddr,IN ULONG ulAddrBuffSize);
	BOOL QueryIPW(IN ULONG ulIP,OUT WCHAR* pszAddr,IN ULONG ulAddrBuffSize);
 
	#ifdef _UNICODE
		#define QueryIP	QueryIPW
	#else
		#define QueryIP	QueryIPA
	#endif
 
	//����ָ��IP���ڼ�¼��ƫ��
	ULONG FindIP(IN ULONG ulIP);
 
	//���IP��Ч��
	BOOL CheckIP(IN char* pszIP);
 
private:
	//Map���ݿ��ļ�
	BOOL Map(IN ULONG ulMapSize);
 
	//UnMap���ݿ��ļ�
	BOOL UnMap(void);
 
private:
	ULONG	m_ulFileSize;
	HANDLE	m_hDatFile;
	HANDLE	m_hDatMap;
	void*	m_pDatBase;
 
	void*	m_pIndex;				//ָ��������
	ULONG	m_ulFirstIndexOffset;	//��һ��������ƫ��
	ULONG	m_ulLastIndexOffset;	//���һ��������ƫ��
	ULONG	m_ulRecordNum;			//��¼����
};
 
#endif