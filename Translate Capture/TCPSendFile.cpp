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
		printf("WSAStartup ����");
		return -1;
	}

	sockClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sockClient)
	{
		printf("socket ����");
		return -2;
	}
	struct sockaddr_in addrServer;
	int len;
	addrServer.sin_addr.S_un.S_addr = inet_addr(SERVERIP);
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(SERVERPORT);
	len = sizeof(sockaddr);

	printf("�ͻ��˳�ʼ����ɣ��������ӷ�����\n\n");
	ret = connect(sockClient, (sockaddr*)&addrServer, len);
	if (SOCKET_ERROR == ret)
	{
		printf("connect ����");
		return -3;
	}

	printf("���ӷ������ɹ������濪ʼͨ��...\n\n");

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
		cout << "���ļ�ʧ��" << endl;
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
			printf("send ����");
			return -5;
		}
		file.read(sendBuf, 512);
		ret = send(sockClient, sendBuf, file.gcount(), 0);
		haveSend += file.gcount();

		//��ӡ����
		if ((int)(double(haveSend) / size * 100.0) > proceed)
		{
			proceed = (int)(double(haveSend) / size * 100.0);
			printf("\r");
			printf("                                ");
			printf("\r");
			printf("�Ѿ����䣺%d %%", (int)(double(haveSend) / size * 100.0));
		}


	}
	printf("\n");
	return 0;
}

TCPSendFile::~TCPSendFile()
{
}
