#pragma once
#include "stdio.h"
#include "winsock2.h"
#pragma comment(lib,"ws2_32.lib")
#define SERVERIP    "127.0.0.1"
#define SERVERPORT  6666
#include<fstream>
#include<string>
#include<iostream>
using namespace std;

class TCPSendFile
{
public:
	int ret;
	char sendBuf[512], recvBuf[512];
	WSADATA data;
	SOCKET sockClient;

	TCPSendFile();
	int init();
	string FilterFilename(string filename);
	int sendFile(string filename);
	~TCPSendFile();

};
