// Client.cpp : implementation file
//

#include "stdafx.h"
#include "Translate Capture.h"
#include "Client.h"
#include "afxdialogex.h"
#include<fstream>
#include<string>
#include<iostream>
using namespace std;
#define SERVERIP    "192.168.28.151"
#define SERVERPORT  8944
// CClient dialog

IMPLEMENT_DYNAMIC(CClient, CDialogEx)

CClient::CClient(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_client, pParent)
	, m_ip(3232242839)
	, frameNum(0)
{

}

CClient::~CClient()
{
}

void CClient::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_IPAddress(pDX, IDC_IPADDRESS1, m_ip);
	DDX_Text(pDX, IDC_EDIT1, frameNum);
}


BEGIN_MESSAGE_MAP(CClient, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CClient::OnBnClickedButton1)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS1, &CClient::OnIpnFieldchangedIpaddress1)
	ON_STN_CLICKED(IDC_IMG, &CClient::OnStnClickedImg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CClient::OnDeltaposSpin1)
	ON_BN_CLICKED(IDC_BUTTON2, &CClient::OnBnClickedButton2)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CClient message handlers

string FilterFilename(string filename)
{
	int pos = filename.find_last_of('\\');
	string r = filename.substr(pos + 1);
	return r.replace(r.find('\n'), 1, "");
}

CString receivefile(SOCKET socket)
{
	int ret;
	char sendBuf[512], recvBuf[512];


	memset(recvBuf, 0, sizeof(recvBuf));
	ret = recv(socket, recvBuf, sizeof(recvBuf), 0);
	//
//	return CString(recvBuf);
	//
	if (SOCKET_ERROR == ret)
	{
		printf("recv 错误");
		return CString();
	}
	printf("从服务端收到以下信息:\n%s\n\n", recvBuf);
	string filename = "rev " + FilterFilename(string(recvBuf));
	ofstream file("ClientScreen\\"+filename, ios::trunc | ios::binary | ios::out);
	//		ofstream file("text.docx", ios::trunc| ios::binary| ios::out);
	if (!file.is_open())
	{
		filename = "new " + FilterFilename(string(recvBuf));
		file = ofstream("ClientScreen\\"+filename, ios::trunc | ios::binary | ios::out);
		if (!file.is_open())
		{
			cout << "打开文件失败" << endl;
			return CString();
		}
	}
	long long total = 0;
	while (true)
	{
		ret = recv(socket, recvBuf, sizeof(recvBuf), 0);
		total += ret;
		if (ret == SOCKET_ERROR  || ret == 0)
//		if (ret == SOCKET_ERROR)
		{
			file.close();
			break;
		}
		file.write(recvBuf, ret);

	}
	cout << "接受成功" << endl;
	return CString(filename.c_str());
}


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
	
	CString filename = receivefile(socket_client);
	drawpic(filename);
	frameNum = getpicNum(filename);
	UpdateData(FALSE);
}





void CClient::OnIpnFieldchangedIpaddress1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CClient::OnStnClickedImg()
{
	// TODO: Add your control notification handler code here


}

void CClient::drawpic(CString filename)
{
	if (filename == "")return;

	CString pathname;
	pathname = "ClientScreen\\";
	CImage Img;
	Img.Load(pathname + filename);
	CDC *pdc;
	CWnd* pw = this->GetDlgItem(IDC_IMG);
	pdc = pw->GetDC();
	pdc->SetStretchBltMode(COLORONCOLOR);
	CRect rect1;
	pw->GetClientRect(rect1);
	int wh = rect1.Height();
	int ww = rect1.Width();
	Img.Draw(pdc->m_hDC, 0, 0, ww, wh);
}

CString CClient::findpic(int num)
{
	CFileFind finder;
	BOOL bWorking = finder.FindFile(_T("ClientScreen\\*.*"));
	int cnum = 0;
	while (bWorking)
	{

		bWorking = finder.FindNextFile();
		if (finder.IsDirectory() || finder.IsDots())continue;
		cnum++;
		if (bWorking == true && cnum == num)
		{
			CString r =  finder.GetFileName();
			finder.Close();
			return r;
		}
	}
	finder.Close();

	return CString();
}

int CClient::getpicNum(CString filename)
{

	CFileFind finder;
	BOOL bWorking = finder.FindFile(_T("ClientScreen\\*.*"));
	int num=0;
	while (bWorking)
	{

		bWorking = finder.FindNextFile();
		if (finder.IsDirectory() || finder.IsDots())continue;
		num++;
		if (filename == finder.GetFileName())
		{
			finder.Close();
			return num;
		}
	}
	finder.Close();

	return -1;
}


void CClient::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	UpdateData(true);
	if (pNMUpDown->iDelta == -1) // 如果此值为-1 , 说明点击了Spin的往下的箭头  
	{
		frameNum++;
	}
	else if (pNMUpDown->iDelta == 1) // 如果此值为1, 说明点击了Spin的往上的箭头  
	{
		frameNum--;
	}
	UpdateData(false);

	drawpic(findpic(frameNum));


	*pResult = 0;


}
int timerid = 32132;
static int startcapture = 0;
void CClient::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	startcapture = !startcapture;

	if (startcapture)
	{
		SetTimer(timerid, 500, 0);
	}
	else {
		KillTimer(timerid);
	}



}


void CClient::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	if (nIDEvent == timerid)
	{
		OnBnClickedButton1();
	}

	CDialogEx::OnTimer(nIDEvent);
}
