#include "stdio.h"
#include "winsock2.h"
#include<iostream>
#include<fstream>
#include<string>
#pragma comment(lib,"ws2_32.lib")
#define SERVERIP    "127.0.0.1"
#define SERVERPORT  6666
using namespace std;

class TCPSendFile
{
public:
	int ret;
	char sendBuf[512], recvBuf[512];
	WSADATA data;
	SOCKET sockClient;

	TCPSendFile() {
		memset(sendBuf, 0, sizeof(sendBuf));
		memset(recvBuf, 0, sizeof(recvBuf));

	}
	int init()
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

	string FilterFilename(string filename)
	{
		int pos = filename.find_last_of('/');
		return string(filename.substr(pos + 1));
	}

	int sendFile(string filename)
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

	~TCPSendFile()
	{
		closesocket(sockClient);
		WSACleanup();
	}

};

int main(void)
{
	TCPSendFile tcp;
	tcp.init();
	//	tcp.sendFile("C:\\Users\\win7x64\\Documents\\visual studio 2015\\Projects\\FileClient\\Debug\\fsdf.docx");
	string filename;
	cout << "输入要传输的文件名，可以是绝对路径，也可以是同一文件夹内的文件： ";
	cin >> filename;

	tcp.sendFile(filename);
	tcp.~TCPSendFile();
	getchar();
	return 0;
}