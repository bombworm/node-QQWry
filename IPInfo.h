/*
 File:		\Src\common\IPInfo.h
 Description:	解析IP地址
 Author:	cdutboy@gmail.com
 Build:		2011.09.21
*/
 
#ifndef IPINFO_HEADER_FILE
	#define IPINFO_HEADER_FILE
/*
QQWry.dat中都是小尾字节序
 
文件夹
 
记录区
 
索引区
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
 
#define IPINFO_INDEX_SIZE	7	//一条索引的大小
 
#pragma pack(pop)

class CIPInfo
{
public:
	CIPInfo(void);
	~CIPInfo();
 
	//在当前目录搜索默认的几个名字,QQWry.dat,IPwry.dat,等
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
 
	//查找指定IP所在记录的偏移
	ULONG FindIP(IN ULONG ulIP);
 
	//检查IP有效性
	BOOL CheckIP(IN char* pszIP);
 
private:
	//Map数据库文件
	BOOL Map(IN ULONG ulMapSize);
 
	//UnMap数据库文件
	BOOL UnMap(void);
 
private:
	ULONG	m_ulFileSize;
	HANDLE	m_hDatFile;
	HANDLE	m_hDatMap;
	void*	m_pDatBase;
 
	void*	m_pIndex;				//指向索引区
	ULONG	m_ulFirstIndexOffset;	//第一条索引的偏移
	ULONG	m_ulLastIndexOffset;	//最后一条索引的偏移
	ULONG	m_ulRecordNum;			//记录条数
};
 
#endif