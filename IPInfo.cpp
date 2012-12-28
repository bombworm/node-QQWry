/*
 File:		\Src\common\IPInfo.h
 Description:	解析IP地址
 Author:	cdutboy@gmail.com
 Build:		2011.09.21
*/

#include <stdio.h>
#include <winsock2.h>
#include "IPInfo.h"
#pragma comment(lib, "ws2_32.lib")

CIPInfo::CIPInfo(void)
{
	m_hDatFile	= INVALID_HANDLE_VALUE;
	m_hDatMap	= NULL;
	m_pDatBase	= NULL;
 
	m_pIndex			 = NULL;	//指向索引区
	m_ulFirstIndexOffset =0;	//第一条索引的偏移
	m_ulLastIndexOffset	 =0;	//最后一条索引的偏移
	m_ulRecordNum		 =0;	//记录条数
}
 
CIPInfo::~CIPInfo()
{
	//UnMap();
	//Close();
}
 
//在当前目录搜索默认的几个名字,QQWry.dat,IPwry.dat,等
BOOL CIPInfo::OpenDefDat(void)
{
	BOOL bReturnFlag=FALSE;
	int	nIndex=0;
 
	char DefNameArray[][20] = 
	{
		"QQWry.dat",
		"CoralWry.dat",
		"MyIP.dat",
		"IPWry.dat",
		"IP.dat",
		"Coral.dat",
		"Wry.dat"
	};
 
	char	szSelfFolder[MAX_PATH]={0};
	char	szDatFile[MAX_PATH]={0};
 
 
	do 
	{
		GetModuleFileNameA(GetModuleHandle(NULL),szSelfFolder,MAX_PATH);
 
		*(strrchr(szSelfFolder,'\\')) = '\0';
 
		for (nIndex=0; nIndex < sizeof(DefNameArray)/sizeof(DefNameArray[0]); nIndex++)
		{
			RtlZeroMemory(szDatFile,sizeof(szDatFile));
 
			_snprintf(szDatFile,MAX_PATH-1,"%s\\%s",szSelfFolder,DefNameArray[nIndex]);
 
			if (OpenA(szDatFile))
			{
				bReturnFlag =TRUE;
				break;
			}
		}
 
	} while (FALSE);
 
	return bReturnFlag;
}
 
BOOL CIPInfo::OpenA(IN char* pszDatPath)
{
 
	#if 0
		m_hDatFile = CreateFileA(pszDatPath,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,\
			NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	#else
		m_hDatFile = CreateFileA(pszDatPath,GENERIC_READ,FILE_SHARE_READ,\
			NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	#endif
 
	if (INVALID_HANDLE_VALUE == m_hDatFile)
	{
		return FALSE;
	}
 
	return this->Map(0);
}
 
BOOL CIPInfo::OpenW(IN WCHAR* pszDatPath)
{
 
	#if 0
		m_hDatFile = CreateFileW(pszDatPath,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,\
			NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	#else
		m_hDatFile = CreateFileW(pszDatPath,GENERIC_READ,FILE_SHARE_READ,\
			NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	#endif
 
	if (INVALID_HANDLE_VALUE == m_hDatFile)
	{
		return FALSE;
	}
 
	return this->Map(0);
}
 
BOOL CIPInfo::Map(IN ULONG ulMapSize)
{
	BOOL bReturnFlag=FALSE;
 
	do 
	{
		if (INVALID_HANDLE_VALUE == m_hDatFile)
		{
			break;
		}
 
		m_ulFileSize = GetFileSize(m_hDatFile,NULL);
		if (0 == m_ulFileSize)
		{
			break;
		}
 
		#if 0
			m_hDatMap = CreateFileMapping(m_hDatFile,NULL,PAGE_READWRITE,0,ulMapSize,NULL);
			if(NULL == m_hDatMap)
			{
				break;
			}
 
			m_pDatBase = MapViewOfFile(m_hDatMap,FILE_MAP_READ|FILE_MAP_WRITE,0,0,0);
			if (NULL == m_pDatBase)
			{
				//XDebug((_T("map错误? %s"),xGetLastErrorString() ));
				break;
			}
		#else
			m_hDatMap = CreateFileMapping(m_hDatFile,NULL,PAGE_READONLY,0,ulMapSize,NULL);
			if(NULL == m_hDatMap)
			{
				break;
			}
 
			m_pDatBase = MapViewOfFile(m_hDatMap,FILE_MAP_READ,0,0,0);
			if (NULL == m_pDatBase)
			{
				//XDebug((_T("map错误? %s"),xGetLastErrorString() ));
				break;
			}
		#endif
 
		//////////////////////////////////////////////////////////////////////////
		//第一条索引的偏移
		m_ulFirstIndexOffset = *( (ULONG*)m_pDatBase );
 
		//最后一条索引的偏移
		m_ulLastIndexOffset =  *( (ULONG*) ( (BYTE*)m_pDatBase + sizeof(ULONG) ) );
 
		//指向索引区
		m_pIndex	= ((BYTE*)m_pDatBase + m_ulFirstIndexOffset);
 
		//记录条数
		m_ulRecordNum = 1 + (m_ulLastIndexOffset - m_ulFirstIndexOffset) / IPINFO_INDEX_SIZE;
 
		//////////////////////////////////////////////////////////////////////////
 
 
		bReturnFlag =TRUE;
	} while (FALSE);
 
	return bReturnFlag;
}
 
 
BOOL CIPInfo::UnMap(void)
{
	BOOL	bReturnFlag=FALSE;
 
	do 
	{	
		if (NULL ==  m_pDatBase)
		{
			break;
		}
 
		if (!FlushViewOfFile(m_pDatBase,m_ulFileSize))
		{
			break;
		}
 
		if (!UnmapViewOfFile(m_pDatBase))
		{
			break;
		}
 
		if (NULL == m_hDatMap)
		{
			break;
		}
 
		if (!CloseHandle(m_hDatMap))
		{
			break;
		}
 
		bReturnFlag =TRUE;
	} while (FALSE);
 
	return bReturnFlag;
}
 
BOOL CIPInfo::Close(void)
{
	BOOL	bReturnFlag=FALSE;
 
	do 
	{
		if (INVALID_HANDLE_VALUE != m_hDatFile)
		{
			break;
		}
 
		if (!CloseHandle(m_hDatFile))
		{
			break;
		}
 
		bReturnFlag =TRUE;
	} while (FALSE);
 
	return bReturnFlag;
}
 
BOOL CIPInfo::QueryIPA(IN char* pszIP,OUT char* pszAddr,IN ULONG ulAddrBuffSize)
{
	BOOL	bReturnFlag=FALSE;
	ULONG	ulTemp=0;
	ULONG	ulNetByteOrderAddr=0;
 
	do 
	{
		if ( (NULL==pszIP) || (0==strlen(pszIP)) || (NULL==pszAddr) || (0==ulAddrBuffSize) )
		{
			break;
		}
 
		if (!CheckIP(pszIP))
		{
			//IP格式不正确
			break;
		}
 
		ulTemp=inet_addr(pszIP);
		if (INADDR_NONE == ulTemp)
		{
			break;
		}
		ulNetByteOrderAddr = htonl(ulTemp);
 
		bReturnFlag = this->QueryIPA(ulNetByteOrderAddr,pszAddr,ulAddrBuffSize);
 
	} while (FALSE);
 
	return bReturnFlag;
}
 
BOOL CIPInfo::QueryIPA(IN ULONG ulIP,OUT char* pszAddr,IN ULONG ulAddrBuffSize)
{
	BOOL	bReturnFlag=FALSE;
	ULONG	ulRecordOffset=0;
	ULONG	ulSecondOffset=0;
	BYTE	bMode=0;
 
	ULONG	ulRedirect=0;
 
	char*	pszMajor=NULL;
	char*	pszMinor=NULL;
	BYTE*	pRecord=NULL;
 
	do 
	{
		if ( (0==ulIP) || (NULL==pszAddr) || (0==ulAddrBuffSize) )
		{
			break;
		}
		RtlZeroMemory(pszAddr,ulAddrBuffSize);
 
		ulRecordOffset =FindIP(ulIP);
		if( 0 == ulRecordOffset)
		{
			break;
		}
 
		pRecord = (BYTE*)m_pDatBase + ulRecordOffset;
		bMode = *( pRecord+ sizeof(ULONG) );
 
		if ( 0 == bMode)
		{
			//未知国家记录
		}
		else if ( 1 == bMode)
		{
			//国家名需要重定向,
			memcpy(&ulRedirect,pRecord + sizeof(ULONG) + sizeof(BYTE),3);
 
			//判断国家名重定向后的国家名是否需要再次重定向,重定向最多2次
			memcpy(&bMode,(BYTE*)m_pDatBase + ulRedirect,1);
 
			//如果国家名发生了第二次重定向，则其第二次重定向一定为模式2
			if( 2 != bMode)
			{
				//国家名不需要二次重定向,就是简单的重定向模式1
				//重定向后的国家记录192.168.1.1
				pszMajor = (char*)((BYTE*)m_pDatBase + ulRedirect);
 
				memcpy(&bMode,(BYTE*)m_pDatBase+ ulRedirect + strlen(pszMajor) +1,1);
				//判断地区是否需要重定向
				if ( 0 == bMode)
				{
					//未知地区
					pszMinor =NULL;
				}
				else if ( (1==bMode) || (2==bMode) )
				{
					//地区需要重定向1.2.3.4
					memcpy(&ulRedirect,(BYTE*)m_pDatBase+ ulRedirect + strlen(pszMajor) +1 + sizeof(BYTE),3);
					pszMinor = (char*)m_pDatBase + ulRedirect;
				}
				else
				{	
					pszMinor = (char*)( (BYTE*)m_pDatBase+ ulRedirect + strlen(pszMajor) +1);
				}
			}
			else
			{
				//国家名需要2次重定向,第二次重定向之后一定为模式2,只会发生在国家记录上
				memcpy(&ulSecondOffset,(BYTE*)m_pDatBase + ulRedirect + sizeof(BYTE),3);
 
				//国家名
				pszMajor = (char*)((BYTE*)m_pDatBase + ulSecondOffset);
 
				//判断地区是否需要重定向
				memcpy(&bMode,(BYTE*)m_pDatBase + ulRedirect + sizeof(ULONG),1);
				if ( 0 == bMode)
				{
					//未知地区
				}
				else if ( (1== bMode) || (2 ==bMode) )
				{
					//地区需要重定向 119.119.119.119
					memcpy(&ulSecondOffset,(BYTE*)m_pDatBase + ulRedirect + sizeof(ULONG) +1,3);
 
					//地区名
					pszMinor = (char*)((BYTE*)m_pDatBase + ulSecondOffset);
				}
				else
				{
					//普通的,跟重定向模式2一样,直接跟在国家偏移之后
					//12.34.56.78
					/*
					BYTE  bMode=2;	//一个字节表示是重定向模式2
					BYTE  CountryOffset[3];	//3个字节,存放国家名的绝对偏移
					char[不定长] 的地区名,以\0结尾.
					*/
					pszMinor = (char*)((BYTE*)m_pDatBase + ulRedirect + sizeof(ULONG));
				}
			}
		}
		else if ( 2 == bMode)			//国家需要重定向,地区不需要,国家和地区不是在一块的
		{
			/*
			重定向模式2		202.115.128.13
 
			ULONG ulEndIP;
			BYTE  bMode=2;	//一个字节表示是重定向模式2
			BYTE  CountryOffset[3];	//3个字节,存放国家名的绝对偏移
			char[不定长] 的地区名,以\0结尾.
			*/
 
			memcpy(&ulRedirect,pRecord + sizeof(ULONG) + sizeof(BYTE),3);			
 
			//国家名需要重定向
			pszMajor = (char*)((BYTE*)m_pDatBase + ulRedirect);
 
			//判断地区是否需要重定向
			memcpy(&bMode,(BYTE*)pRecord+ sizeof(ULONG) + sizeof(ULONG),1);
			if ( 0 == bMode)
			{
				//未知地区
				pszMinor =NULL;
			}
			else if ( (1==bMode) || (2==bMode) )
			{
				//地区需要重定向1.2.3.4
				memcpy(&ulRedirect,(BYTE*)pRecord+ sizeof(ULONG) + sizeof(ULONG) +sizeof(BYTE),3);
				pszMinor = (char*)m_pDatBase + ulRedirect;
			}
			else
			{	
				//地区名没有重定向
				pszMinor = (char*)( pRecord+ sizeof(ULONG) + sizeof(ULONG));
			}
 
		}
		else
		{
			//最简单的类型,国家不需要重定向
			pszMajor = (char*)pRecord+ sizeof(ULONG);
 
			//判断地区是否需要重定向
			memcpy(&bMode,(BYTE*)pRecord + sizeof(ULONG) + strlen(pszMajor) +1,1);
			if ( 0 == bMode)
			{
				//未知地区
				pszMinor =NULL;
			}
			else if ( (1==bMode) || (2==bMode) )
			{
				//地区需要重定向1.2.3.4
				memcpy(&ulRedirect,(BYTE*)pRecord + sizeof(ULONG) + strlen(pszMajor) +1 +sizeof(BYTE),3);
				pszMinor = (char*)m_pDatBase + ulRedirect;
			}
			else
			{	
				/*
				ULONG ulEndIP;	//4个字节的结束IP
				char[不定长] 的国家名.后面以\0结尾
				char[不定长] 的地区名,后面以\0结尾.
				*/
				pszMinor = (char*)pRecord + sizeof(ULONG) + strlen(pszMajor) +1;
			}
		}
 
		//综合国家名和地区名
		if ( NULL== pszMajor )
		{
			//strcpy(pszAddr,"未知国家");
			break;
		}
 
		RtlZeroMemory(pszAddr,ulAddrBuffSize);
		strncpy(pszAddr,pszMajor,strlen(pszMajor));
 
		if (NULL != pszMinor)
		{
			strcat(pszAddr,pszMinor);
		}
 
		bReturnFlag =TRUE;
	} while (FALSE);
 
	return bReturnFlag;
}
 
BOOL CIPInfo::QueryIPW(IN WCHAR* pszWideIP,OUT WCHAR* pszWideAddr,IN ULONG ulWideBuffSize)
{
	char	szAnsiIP[40]={0};
	char*	pszAnsiAddr=NULL;
	ULONG	ulAnsiBuffSize=0;
	BOOL	bFlag=FALSE;
 
	do 
	{
		if ( (NULL==pszWideIP) || (NULL==pszWideAddr) || (0==ulWideBuffSize) )
		{
			break;
		}
 
		if ( 0 == WideCharToMultiByte(CP_ACP,0,pszWideIP,-1,szAnsiIP,sizeof(szAnsiIP),0,0))
		{
			break;
		}
 
		ulAnsiBuffSize = ulWideBuffSize * sizeof(WCHAR);
		pszAnsiAddr = new char[ulAnsiBuffSize];
		if (NULL == pszAnsiAddr)
		{
			break;
		}
		RtlZeroMemory(pszAnsiAddr,ulAnsiBuffSize);
 
		if (!QueryIPA(szAnsiIP,pszAnsiAddr,ulAnsiBuffSize))
		{
			break;
		}
 
		if ( 0 == MultiByteToWideChar(CP_ACP,MB_ERR_INVALID_CHARS,pszAnsiAddr,-1,pszWideAddr,ulWideBuffSize))
		{
			break;
		}
 
		bFlag =TRUE;
	} while (FALSE);
 
	if (NULL != pszAnsiAddr)
	{
		delete[] pszAnsiAddr;
		pszAnsiAddr = NULL;
	}
 
	return bFlag;
}
 
BOOL CIPInfo::QueryIPW(IN ULONG ulIP,OUT WCHAR* pszWideAddr,IN ULONG ulWideBuffSize)
{
	char*	pszAnsiAddr=NULL;
	ULONG	ulAnsiBuffSize=0;
	BOOL	bFlag=FALSE;
 
	do 
	{
		if ( (0==ulIP) || (NULL==pszWideAddr) || (0==ulWideBuffSize) )
		{
			break;
		}
 
		ulAnsiBuffSize = ulWideBuffSize * sizeof(WCHAR);
		pszAnsiAddr = new char[ulAnsiBuffSize];
		if (NULL == pszAnsiAddr)
		{
			break;
		}
		RtlZeroMemory(pszAnsiAddr,ulAnsiBuffSize);
 
		if (!QueryIPA(ulIP,pszAnsiAddr,ulAnsiBuffSize))
		{
			break;
		}
 
		if ( 0 == MultiByteToWideChar(CP_ACP,MB_ERR_INVALID_CHARS,pszAnsiAddr,-1,pszWideAddr,ulWideBuffSize))
		{
			break;
		}
 
		bFlag =TRUE;
	} while (FALSE);
 
	if (NULL != pszAnsiAddr)
	{
		delete[] pszAnsiAddr;
		pszAnsiAddr = NULL;
	}
 
	return bFlag;
}
 
//查找指定IP,返回记录的偏移
ULONG CIPInfo::FindIP(IN ULONG ulIP)
{
	ULONG			ulBegin=0;
	ULONG			ulEnd=m_ulRecordNum;
	ULONG			ulMid=0;
	ULONG			ulStartIP=0;	//索引区,起始IP
	ULONG			ulEndIP=0;		//记录区,结束IP
	ULONG			ulOffset=0;
 
 
	//索引结构共7个字节
	//前4个字节表示一条记录的起始IP
	//后3个字节表示这条记录在文件中的绝对偏移.
 
	//二分法查找索引
	while(ulBegin < ulEnd-1)
	{
		ulMid = ulBegin + ( (ulEnd - ulBegin)/2 );
 
		//索引的前四个字节就是起始IP
		ulStartIP = * ( (ULONG*)( (BYTE*)m_pIndex +  ulMid * IPINFO_INDEX_SIZE ));
 
		//找到了,返回
		if (ulIP < ulStartIP)
		{
			ulEnd = ulMid;
		}
		else
		{	
			ulBegin = ulMid;	
		}			
	}
 
	//ulOffset 就是对应的记录在
	memcpy(&ulOffset,(BYTE*)m_pIndex + ulBegin*IPINFO_INDEX_SIZE + sizeof(ULONG),3);
 
	//记录结构
	//结束IP(4字节) + 国家名(不定长) + 地区名(不定长)
	//偏移记录的第五个字节是1或者2,就表示需要重定向,QueryIPA里有处理
 
	//记录的前4个字节就是结束IP
	ulEndIP = *( (ULONG*)((BYTE*)m_pDatBase + ulOffset));
 
	if (ulIP >  ulEndIP )
	{
		return 0;
	}
 
	return ulOffset;
}
 
//检查IP有效性
BOOL CIPInfo::CheckIP(IN char* pszIP)
{	
	BOOL bReturnFlag=FALSE;
	int  a=0;
	int  b=0;
	int  c=0;
	int  d=0;
	int  e=0;
	int  nIPLen=0;
 
 
	do 
	{
		if ( (NULL == pszIP) || (0==strlen(pszIP)) )
		{
			break;
		}
 
		//255.255.255.255
		//0.0.0.0
		nIPLen = strlen(pszIP);
		if ( (7 > nIPLen) || (nIPLen > 15 ) )
		{
			break;
		}
 
		if ( 4 != sscanf(pszIP,"%d.%d.%d.%d%c",&a,&b,&c,&d,&e))
		{
			break;
		}
 
		if ( (0>a) || (a>255) )
		{
			break;
		}
 
		if ( (0>b) || (b>255) )
		{
			break;
		}
 
		if ( (0>c) || (c>255) )
		{
			break;
		}
 
		if ( (0>d) || (d>255) )
		{
			break;
		}
 
		bReturnFlag =TRUE;
	} while (FALSE);
 
	return bReturnFlag;
}