#include "stdafx.h"
#include"TCPSendFile.h"
using namespace std;


TCPSendFile::TCPSendFile()
{
}

int TCPSendFile::init()
{
	ret = WSAStartup(MAKEWORD(2, 2), &data);
	if (SOCKET_ERROR == ret)
	{
		printf("WSAStartup 错误");
		return -1;
	}

	sockClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sockClient)
	{
		printf("socket 错误");
		return -2;
	}
	struct sockaddr_in addrServer;
	int len;
	addrServer.sin_addr.S_un.S_addr = inet_addr(SERVERIP);
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(SERVERPORT);
	len = sizeof(sockaddr);

	printf("客户端初始化完成，正在连接服务器\n\n");
	ret = connect(sockClient, (sockaddr*)&addrServer, len);
	if (SOCKET_ERROR == ret)
	{
		printf("connect 错误");
		return -3;
	}

	printf("连接服务器成功，下面开始通信...\n\n");

	return 0;
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

		if (SOCKET_ERROR == ret)
		{
			printf("send 错误");
			return -5;
		}
		file.read(sendBuf, 512);
		ret = send(sockClient, sendBuf, file.gcount(), 0);
		haveSend += file.gcount();

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
