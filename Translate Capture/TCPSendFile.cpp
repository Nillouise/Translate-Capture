#include "stdafx.h"
#include"TCPSendFile.h"
using namespace std;


TCPSendFile::TCPSendFile()
{
}

string TCPSendFile::FilterFilename(string filename)
{
	int pos = filename.find_last_of('/');
	return string(filename.substr(pos + 1));
}

int TCPSendFile::sendFile(string filename)
{
	ifstream file(filename, ios::in | ios::binary | ios::ate);
	if (!file.is_open())
	{
		cout << "打开文件失败" << endl;
		return -1;
	}
	long long size = file.tellg();
	file.seekg(0, ios::beg);

	ret = send(sockClient, (FilterFilename(filename) + "\n").c_str(), FilterFilename(filename).length() + 2, 0);
	long long haveSend = 0;
	int proceed = -1;
	while (haveSend < size)
	{
		if (haveSend * 2 > size)
		{
			int a = 0;
		}


		file.read(sendBuf, 512);
		long long curread = file.gcount();
		int a=1;
		if (curread != 512)
		{
			break;
		}
		ret = send(sockClient, sendBuf, curread, 0);
		haveSend += curread;
		if (SOCKET_ERROR == ret)
		{
			printf("send 错误");
			while(SOCKET_ERROR == ret && WSAGetLastError() == 10035)//处理发送窗口塞满的情况
			{
				Sleep(3);
				ret = send(sockClient, sendBuf, curread, 0);
			}
		}
		//打印进度
		if ((int)(double(haveSend) / size * 100.0) > proceed)
		{
			proceed = (int)(double(haveSend) / size * 100.0);
			printf("\r");
			printf("                                ");
			printf("\r");
			printf("已经传输：%d %%", (int)(double(haveSend) / size * 100.0));
		}


	}
	printf("\n");
	return 0;
}

TCPSendFile::~TCPSendFile()
{
}
