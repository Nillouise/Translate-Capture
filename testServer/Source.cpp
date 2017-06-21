#include "stdio.h"
#include "winsock2.h"
#pragma comment(lib,"ws2_32.lib")
#define SERVERIP    "127.0.0.1"
#define SERVERPORT  6666
#include<fstream>
#include<string>
#include<iostream>
using namespace std;

class testTCPReceiveFile
{
public:
	int ret;
	char sendBuf[512], recvBuf[512];
	WSADATA data;
	SOCKET sockListen, sockTongxun;
	int len;
	testTCPReceiveFile()
	{
		memset(sendBuf, 0, sizeof(sendBuf));
		memset(recvBuf, 0, sizeof(recvBuf));
		ret = 0;
	}
	int init()
	{
		ret = WSAStartup(MAKEWORD(2, 2), &data);
		if (SOCKET_ERROR == ret)
		{
			printf("WSAStartup 错误");
			return -1;
		}

		sockListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (INVALID_SOCKET == sockListen)
		{
			printf("socket 错误");
			return -2;
		}

		struct sockaddr_in addrServer;
		addrServer.sin_addr.S_un.S_addr = inet_addr(SERVERIP);
		addrServer.sin_family = AF_INET;
		addrServer.sin_port = htons(SERVERPORT);
		len = sizeof(sockaddr);
		ret = bind(sockListen, (sockaddr*)&addrServer, len);
		if (SOCKET_ERROR == ret)
		{
			printf("bind 错误");
			return -3;
		}

		ret = listen(sockListen, 1);
		if (SOCKET_ERROR == ret)
		{
			printf("listen 错误");
			return -4;
		}

		printf("服务器启动成功，正在监听...\n\n");
	}

	string FilterFilename(string filename)
	{
		int pos = filename.find_last_of('/');
		string r = filename.substr(pos + 1);
		return r.replace(r.find('\n'), 1, "");
	}
	int run()
	{

		struct sockaddr_in addrClient;
		len = sizeof(sockaddr);
		sockTongxun = accept(sockListen, (sockaddr*)&addrClient, &len);
		if (INVALID_SOCKET == sockListen)
		{
			printf("accept 错误");
			return -5;
		}
		printf("接收一个客户端连接，下面开始通信\n\n");

		memset(recvBuf, 0, sizeof(recvBuf));
		ret = recv(sockTongxun, recvBuf, sizeof(recvBuf), 0);
		if (SOCKET_ERROR == ret)
		{
			printf("recv 错误");
			return -7;
		}
		printf("从客户端收到以下信息:\n%s\n\n", recvBuf);
		ofstream file(FilterFilename("rev " + string(recvBuf)), ios::trunc | ios::binary | ios::out);

		//		ofstream file("text.docx", ios::trunc| ios::binary| ios::out);
		if (!file.is_open())
		{
			file = ofstream("new " + FilterFilename(string(recvBuf)), ios::trunc | ios::binary | ios::out);
			if (!file.is_open())
			{
				cout << "打开文件失败" << endl;
				return -1;
			}
		}
		while (true)
		{
			ret = recv(sockTongxun, recvBuf, sizeof(recvBuf), 0);
			if (ret == SOCKET_ERROR || ret == 0)
			{
				file.close();
				break;
			}
			file.write(recvBuf, ret);

		}
		cout << "接受成功" << endl;
		return 0;
	}
	~testTCPReceiveFile()
	{
		closesocket(sockListen);
		closesocket(sockTongxun);
		WSACleanup();
	}

};


int main(void)
{

	testTCPReceiveFile tcp;
	tcp.init();
	tcp.run();

	getchar();
	return 0;

}