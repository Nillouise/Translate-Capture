// Client.cpp : implementation file
//

#include "stdafx.h"
#include "Translate Capture.h"
#include "Client.h"
#include "afxdialogex.h"


// CClient dialog

IMPLEMENT_DYNAMIC(CClient, CDialogEx)

CClient::CClient(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_client, pParent)
	, m_ip(0)
{

}

CClient::~CClient()
{
}

void CClient::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_IPAddress(pDX, IDC_IPADDRESS1, m_ip);
}


BEGIN_MESSAGE_MAP(CClient, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CClient::OnBnClickedButton1)
END_MESSAGE_MAP()


// CClient message handlers


void CClient::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here

	////========加载Winsock动态链接库======	
	WORD	version = MAKEWORD(2, 0);
	WSADATA wsadata;
	if (WSAStartup(version, &wsadata))
	{
		MessageBox(_T("加载Winsock失败"));
		return;
	}
	SOCKET socket_client;
	//=============创建套接字======================
	socket_client = socket(AF_INET, SOCK_STREAM, NULL);
	if (INVALID_SOCKET == socket_client)
	{
		MessageBox(_T("套接字创建失败"));
		return;
	}
	//==============获取服务器地址===================

	UpdateData(true);
	SOCKADDR_IN addr;
	memset(&addr, 0, sizeof(SOCKADDR_IN));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(Server_Port );
	addr.sin_addr.S_un.S_addr = htonl(m_ip);//inet_addr
	if (connect(socket_client, (sockaddr *)&addr, sizeof(sockaddr)))
	{
		MessageBox(_T("尝试与服务器的连接失败"));
		return;
	}

}



