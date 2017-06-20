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

	////========����Winsock��̬���ӿ�======	
	WORD	version = MAKEWORD(2, 0);
	WSADATA wsadata;
	if (WSAStartup(version, &wsadata))
	{
		MessageBox(_T("����Winsockʧ��"));
		return;
	}
	SOCKET socket_client;
	//=============�����׽���======================
	socket_client = socket(AF_INET, SOCK_STREAM, NULL);
	if (INVALID_SOCKET == socket_client)
	{
		MessageBox(_T("�׽��ִ���ʧ��"));
		return;
	}
	//==============��ȡ��������ַ===================

	UpdateData(true);
	SOCKADDR_IN addr;
	memset(&addr, 0, sizeof(SOCKADDR_IN));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(Server_Port );
	addr.sin_addr.S_un.S_addr = htonl(m_ip);//inet_addr
	if (connect(socket_client, (sockaddr *)&addr, sizeof(sockaddr)))
	{
		MessageBox(_T("�����������������ʧ��"));
		return;
	}

}



