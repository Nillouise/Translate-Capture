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
	CDC *pDC;//屏幕DC
	pDC = pwin->GetDesktopWindow()->GetDC();//获取当前整个屏幕DC
	int BitPerPixel = pDC->GetDeviceCaps(BITSPIXEL);//获得颜色模式
	int Width = pDC->GetDeviceCaps(HORZRES);//获得水平分辨率
	int Height = pDC->GetDeviceCaps(VERTRES);//获得垂直分辨率
	CDC memDC;//建立和屏幕兼容的内存DC
	memDC.CreateCompatibleDC(pDC);
	CBitmap memBitmap, *oldmemBitmap;//建立和屏幕兼容的bitmap
	memBitmap.CreateCompatibleBitmap(pDC, Width, Height);
	oldmemBitmap = memDC.SelectObject(&memBitmap);//将memBitmap选入内存DC
	memDC.BitBlt(0, 0, Width, Height, pDC, 0, 0, SRCCOPY);//复制屏幕图像到内存DC
	//以下代码保存memDC中的位图到文件

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
