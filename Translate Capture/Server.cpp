// Server.cpp : implementation file
//

#include "stdafx.h"
#include "Translate Capture.h"
#include "Server.h"
#include "afxdialogex.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS

// CServer dialog

IMPLEMENT_DYNAMIC(CServer, CDialogEx)

CServer::CServer(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_server, pParent)
	, m_link(_T(""))
{

}

CServer::~CServer()
{
}

void CServer::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_link);
}


BEGIN_MESSAGE_MAP(CServer, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CServer::OnBnClickedButton1)
//	ON_REGISTERED_MESSAGE(WM_SOCKET, &CServer::OnSocket)
//ON_MESSAGE(WM_SOCKET, &CServer::OnSocket)
//ON_REGISTERED_MESSAGE(WM_SOCKET, &CServer::OnSocket)
ON_MESSAGE(WM_SOCKET, &CServer::OnSocket)
END_MESSAGE_MAP()


// CServer message handlers


void CServer::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	WORD	version = MAKEWORD(2, 0);
	WSADATA wsadata;
	if (WSAStartup(version, &wsadata))
	{
		MessageBox(_T("加载Winsock dll 失败"));
		return;
	}

	//========创建套接字===============
	SOCKET socket_tmp = socket(AF_INET, SOCK_STREAM, NULL);
	if (INVALID_SOCKET == socket_tmp)
	{
		MessageBox(_T("套接字创建失败"));
		return;
	}
	//========自动获取主机的IP地址==========

	char hostname[20] = "";
	if (gethostname(hostname, 20))
	{
		MessageBox(_T("主机名获取失败"));
		return;
	}

	hostent *htent = gethostbyname(hostname);
	if (htent == NULL)
	{
		MessageBox(_T("主机IP获取失败"));
		return;
	}
	LPSTR lpAddr = htent->h_addr_list[0];
	IN_ADDR inAddr;
	memmove(&inAddr, lpAddr, 4);//4个字节的IP地址
	char *ipAddress = inet_ntoa(inAddr);//将网络地址转换成主机地址格式
	SOCKADDR_IN addr;
	memset(&addr, 0, sizeof(SOCKADDR_IN));//初始化，套接字地址域全清0
	addr.sin_family = AF_INET;
	addr.sin_port = htons(Server_Port);
	addr.sin_addr.S_un.S_addr = inet_addr(ipAddress);

	//============绑定套接字========================================

	if (bind(socket_tmp, (sockaddr *)&addr, sizeof(sockaddr)))
	{
		MessageBox(_T("绑定IP和port出错"));
		return;
	}
	//============启动服务器监听==================================

	if (listen(socket_tmp, SOMAXCONN))
	{
		MessageBox(_T("监听出错"));
		return;
	}
	//============设置异步套接字启动套接字窗口消息映射===========================

	if (WSAAsyncSelect(socket_tmp, this->m_hWnd, WM_SOCKET, FD_ACCEPT | FD_READ | FD_CLOSE))
	{
		MessageBox(_T("异步设置出错"));
		return;
	}
	m_link += "服务器启动成功\n";
	socket_server = socket_tmp;
	//	m_start.EnableWindow(false);
	//	m_stop.EnableWindow(true);
	UpdateData(false);

}


//afx_msg LRESULT CServer::OnSocket(WPARAM wParam, LPARAM lParam)
//{
//	return 0;
//}


//afx_msg LRESULT CServer::OnSocket(WPARAM wParam, LPARAM lParam)
//{
//	return 0;
//}


//afx_msg LRESULT CServer::OnSocket(WPARAM wParam, LPARAM lParam)
//{
//	return 0;
//}


afx_msg LRESULT CServer::OnSocket(WPARAM wParam, LPARAM lParam)
{
	UpdateData(TRUE);
	int J = 0;
	SOCKET socket_client;
	switch (lParam)//消息值
	{
	case FD_ACCEPT:
	{
		SOCKADDR_IN addr;

		int len = sizeof(SOCKADDR_IN);
		socket_client = accept(socket_server, (sockaddr *)&addr, &len);
		if (socket_client == INVALID_SOCKET)
		{
			MessageBox(_T("客户端套接字创建出错"));
			return -1;
		}
		else {
			//获取对方ip
			m_link += "客户端IP：";
			m_link += inet_ntoa(addr.sin_addr);
			UpdateData(false);
			break;
		}
	}


	case FD_CLOSE:
		/*
		for(int i=0;i<5;i++){
		if(socket_client[i]==(SOCKET)wParam){
		J=i;
		i=5;
		}
		}
		closesocket(socket_client[J]);
		ClientNum--;*/
		break;
	}



	return 0;
}
