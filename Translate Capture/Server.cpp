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
ON_BN_CLICKED(IDC_BUTTON2, &CServer::OnBnClickedButton2)
END_MESSAGE_MAP()


// CServer message handlers


void CServer::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	WORD	version = MAKEWORD(2, 0);
	WSADATA wsadata;
	if (WSAStartup(version, &wsadata))
	{
		MessageBox(_T("����Winsock dll ʧ��"));
		return;
	}

	//========�����׽���===============
	SOCKET socket_tmp = socket(AF_INET, SOCK_STREAM, NULL);
	if (INVALID_SOCKET == socket_tmp)
	{
		MessageBox(_T("�׽��ִ���ʧ��"));
		return;
	}
	//========�Զ���ȡ������IP��ַ==========

	char hostname[20] = "";
	if (gethostname(hostname, 20))
	{
		MessageBox(_T("��������ȡʧ��"));
		return;
	}

	hostent *htent = gethostbyname(hostname);
	if (htent == NULL)
	{
		MessageBox(_T("����IP��ȡʧ��"));
		return;
	}
	LPSTR lpAddr = htent->h_addr_list[0];
	IN_ADDR inAddr;
	memmove(&inAddr, lpAddr, 4);//4���ֽڵ�IP��ַ
	char *ipAddress = inet_ntoa(inAddr);//�������ַת����������ַ��ʽ
	SOCKADDR_IN addr;
	memset(&addr, 0, sizeof(SOCKADDR_IN));//��ʼ�����׽��ֵ�ַ��ȫ��0
	addr.sin_family = AF_INET;
	addr.sin_port = htons(Server_Port);
	addr.sin_addr.S_un.S_addr = inet_addr(ipAddress);

	//============���׽���========================================

	if (bind(socket_tmp, (sockaddr *)&addr, sizeof(sockaddr)))
	{
		MessageBox(_T("��IP��port����"));
		return;
	}
	//============��������������==================================

	if (listen(socket_tmp, SOMAXCONN))
	{
		MessageBox(_T("��������"));
		return;
	}
	//============�����첽�׽��������׽��ִ�����Ϣӳ��===========================

	if (WSAAsyncSelect(socket_tmp, this->m_hWnd, WM_SOCKET, FD_ACCEPT | FD_READ | FD_CLOSE))
	{
		MessageBox(_T("�첽���ó���"));
		return;
	}
	m_link += "�����������ɹ�\n";
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
	switch (lParam)//��Ϣֵ
	{
	case FD_ACCEPT:
	{
		SOCKADDR_IN addr;

		int len = sizeof(SOCKADDR_IN);
		socket_client = accept(socket_server, (sockaddr *)&addr, &len);
		if (socket_client == INVALID_SOCKET)
		{
			MessageBox(_T("�ͻ����׽��ִ�������"));
			return -1;
		}
		else {
			//��ȡ�Է�ip
			m_link += "�ͻ���IP��";
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

CString getTime()
{
	long long t = time(NULL);
	CString a;
	a.Format(_T("%lld"), t);
	return a;
}

CString getCapture(CDialogEx * pwin)
{

	CString filename;
	filename = "ServerScreen\\";
	filename += getTime();
	filename += ".jpg";

	CImage Img;
	if (!Img.IsNull())Img.Destroy();
	CDC *pDC;//��ĻDC
	pDC = pwin->GetDesktopWindow()->GetDC();//��ȡ��ǰ������ĻDC
	int BitPerPixel = pDC->GetDeviceCaps(BITSPIXEL);//�����ɫģʽ
	int Width = pDC->GetDeviceCaps(HORZRES);//���ˮƽ�ֱ���
	int Height = pDC->GetDeviceCaps(VERTRES);//��ô�ֱ�ֱ���
	CDC memDC;//��������Ļ���ݵ��ڴ�DC
	memDC.CreateCompatibleDC(pDC);
	CBitmap memBitmap, *oldmemBitmap;//��������Ļ���ݵ�bitmap
	memBitmap.CreateCompatibleBitmap(pDC, Width, Height);
	oldmemBitmap = memDC.SelectObject(&memBitmap);//��memBitmapѡ���ڴ�DC
	memDC.BitBlt(0, 0, Width, Height, pDC, 0, 0, SRCCOPY);//������Ļͼ���ڴ�DC
	//���´��뱣��memDC�е�λͼ���ļ�

	Img.Attach((HBITMAP)memBitmap.GetSafeHandle());
	Img.Save(filename);
	Img.Detach();
	pDC->DeleteDC();
	memBitmap.DeleteObject();
	memDC.DeleteDC();
	pwin->ReleaseDC(pDC);
	Img.Destroy();

	return filename;
}

void CServer::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	CString str = getCapture(this);
	CImage img;
	img.Load(str);
	CDC *pdc;
	CWnd *pw = GetDlgItem(IDC_STATIC);
	pdc = pw->GetDC();
	pdc->SetStretchBltMode(COLORONCOLOR);
	CRect rect;
	this->GetClientRect(rect);
	img.Draw(pdc->m_hDC, 0, 0, rect.Width(), rect.Height());
//	img.Destroy();
}
