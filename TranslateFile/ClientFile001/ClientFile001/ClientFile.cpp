#include <WinSock2.h>
#include<iostream>
#include<thread>
#include<windows.h>
#include"../../command/MyStr.h"
#include<fstream>
using namespace std;
#pragma comment(lib,"ws2_32.lib")
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" ) 
#define RECVBUFSIZE 1*1024*1024
#define READBUFSIZE 1*1024*1024

WSADATA wsaData = { 0 };
SOCKET ClientSocket = INVALID_SOCKET;
SOCKADDR_IN ServerAddr = { 0 };



bool start_send_file = false;	//send thread run only
bool send_state = false;		//if server send stop,this changed true,and stop send file
char send_file_path[MAX_PATH] = {0};
char send_src_file_path[MAX_PATH] = {0};
char send_file_speech[MAX_PATH]={0};
char send_file_size[MAX_PATH] = {0};
char send_file_location[30] = { 0 };

char *send_buf;


char ip[20];
int port;

bool startConnectServer(char *ip, int port)
//connnect server
{
	//1.connct server
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))return false;
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)return false;
	ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ClientSocket == INVALID_SOCKET)return false;

	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(port);
	ServerAddr.sin_addr.S_un.S_addr = inet_addr(ip);

	if (SOCKET_ERROR == connect(ClientSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr)))
	{
		closesocket(ClientSocket);
		WSACleanup();
		return false;
	}
	//2.send ip and port to server
	char buf[1024] = { 0 };
	char c_port[6];
	MyString::intToChar(port, c_port);
	MyString::myStrcat(buf, ip);
	MyString::myStrcat(buf, "&");
	MyString::myStrcat(buf, c_port);
	int iSnedLen = send(ClientSocket, buf, MyString::myStrLen(buf), 0);
	if (!iSnedLen)
	{
		closesocket(ClientSocket);
		WSACleanup();
		return false;
	}
	std::wcout << buf;
	return true;
}

UINT sendFileThread(char *file_path, DWORDLONG speech,char *str)
{
	send_state = true;
	Sleep(10);
	send(ClientSocket, str,MyString::myStrLen(str)+1 ,0);

	//1. open file
	long long ret = 0;

	/*int temp_size = MultiByteToWideChar(CP_ACP, 0, file_path, -1, NULL, 0);
	wchar_t *wfile_path = new wchar_t[temp_size];
	memset(wfile_path, 0, temp_size);
	MultiByteToWideChar(CP_ACP, 0, file_path, -1, wfile_path, temp_size);*/

	fstream file(file_path, std::ios_base::in | std::ios_base::binary);
	long long location = MyString::charToDwordlong(send_file_location);
	file.seekg(location,std::ios_base::beg);
	if (!file.is_open())
	{
		Sleep(10);
		send(ClientSocket, "weiyibiaoshifu@send file error", 60, 0);
		std::wcout << L"file open error";
		std::wcout << "w path is:" << file_path << endl;
		start_send_file = false;
		return 1;
	}
	std::cout << "seek :" << location;
	//2. start to send file
	long long nStartTime=MyString::myGetSysTimeMicros(), nNowSize=location;
	while (file)
	{
		if (send_state != true) break; 
		if (speech * 1024< READBUFSIZE)file.read(send_buf, speech*1024);
		else file.read(send_buf, READBUFSIZE-1);
		//file.read(send_buf, speech * 1024 - 1);
		Sleep(5);
		nNowSize += file.gcount();
		std::cout << "send size:"<<nNowSize << endl;
		MyString::getTranslateSpeech(nStartTime, speech, nNowSize-location);
		send(ClientSocket, (const char *)send_buf, file.gcount(), 0);
	}
	if (file.is_open())file.close();
	Sleep(5);
	send(ClientSocket, "weiyibiaoshifu@send file over", 60, 0);
	Sleep(5);
	send(ClientSocket, "weiyibiaoshifu@send file over", 60, 0);
	start_send_file = false;	
	std::wcout << L"\n\nfile send success\n\n\n";
	return 0;
}


bool readMessage()
{
	char *read_buf = new char[RECVBUFSIZE];
	memset(read_buf, 0, RECVBUFSIZE);

	fstream file;
	char file_name[MAX_PATH];
	long long file_size;
	long long recv_file_size = 0;
	char buf_type[3] = { 0 };
	int64_t ret = 0;
	bool start_recv_file = false;
	send_buf = new char[READBUFSIZE];
	memset(send_buf, 0, READBUFSIZE);
	
	while (1)
	{
		//1.recive message
		memset(read_buf, 0,RECVBUFSIZE);
		memset(buf_type, 0, 3);
		ret = 0;
		ret = recv(ClientSocket, read_buf, RECVBUFSIZE, 0);
		if (ret <= 0)								//now renew connect
		{
			start_recv_file = false;
			closesocket(ClientSocket);
			Sleep((rand()%66));
			startConnectServer(ip, port);
			continue;
		}

		//2.judge message ,it is name or size or file content
		if (!start_recv_file)//recv file and don`t to judge message type
		{
			buf_type[0] = read_buf[MyString::myStrLen(read_buf) - 2];
			buf_type[1] = read_buf[MyString::myStrLen(read_buf) - 1];
			buf_type[2] = '\0';
			std::wcout << L"\ndata:" << read_buf;
		}
		if (MyString::myStrSame(buf_type, "@N"))			//name
		{
			std::wcout << L"file name\n";
			read_buf[MyString::myStrLen(read_buf) - 2] = '\0';
			MyString::myStrcp(file_name, read_buf);
			std::wcout << L"\n" << file_name;
		}
		else if (MyString::myStrSame(buf_type, "@S"))		//size
		{
			std::wcout << L"file size\n";
			read_buf[MyString::myStrLen(read_buf) - 2] = '\0';
			if (MyString::myStrLen(read_buf) == 0)MyString::myStrcp(read_buf,"0");
			file_size = MyString::charToDwordlong(read_buf);
			std::wcout << L"\n" << file_size << endl;
		}
		else if (MyString::myStrSame(buf_type, "@E"))		//close file
		{	
			
			if (file.is_open())file.close();
			recv_file_size = 0;
			start_recv_file = false;
			std::wcout << L"\nfile is close on two\n";
		}
		else if (MyString::myStrSame(buf_type, "@O"))		//open file
		{
			std::wcout << L"file open\n";
			recv_file_size = 0;
			start_recv_file = true;

			file.open(file_name, ios_base::in | ios_base::binary);
			file.seekg(0, std::ios_base::end);
			long long size = file.tellg();
			char temp[30] = { 0 };
			if (size <= 0 || size>file_size)
			{
				size = 0;
				MyString::myStrcp(temp, "0");
			}
			else
			{
				MyString::DwordlongToChar(size, temp);
			}
			recv_file_size = size;
			
			MyString::myStrcp(send_buf, "weiyibiaoshifu@send file agin#");
			MyString::myStrcat(send_buf, temp);
			send(ClientSocket, send_buf, MyString::myStrLen(send_buf) + 1, 0);
			std::wcout << "send data is:" << send_buf << endl;

			if (file.is_open())file.close();
			if(size==0)file.open(file_name, ios_base::out | ios_base::binary);
			else file.open(file_name, ios_base::app | ios_base::binary);
		}
		else
		{
			//2.1 get command to send file
			if (MyString::myStrContainS(read_buf, "start to send weiyibiaoshifu"))
			{
				if (start_send_file==true)continue;
				start_send_file = true;	//pre send file
				//2.1.1 extract file path and file name and src path name
				char temp_str[MAX_PATH+100];
				MyString::myStrSplit(read_buf, send_file_path, '#', temp_str);
				MyString::myStrSplit(temp_str, send_file_path, '$', send_file_speech);
				MyString::myStrcp(temp_str, send_file_speech);
				MyString::myStrSplit(temp_str, send_file_speech, '@', send_src_file_path);
				MyString::myStrcp(temp_str, send_src_file_path);
				MyString::myStrSplit(temp_str, send_src_file_path, '&',send_file_location);
				std::wcout << L"send file path:" << send_file_path;
				std::wcout << L"\nsend file speech:" << send_file_speech;
				std::wcout << L"\nsend file src path:" << send_src_file_path ;
				std::wcout << L"\nsend file location:" << send_file_location << L"\n";
				//2.1.2 get file size
				long long file_size=0;

				int temp_size = MultiByteToWideChar(CP_ACP, 0, send_file_path, -1, NULL, 0);
				wchar_t *wfile_path = new wchar_t[temp_size];
				memset(wfile_path, 0, temp_size);
				MultiByteToWideChar(CP_ACP, 0, send_file_path, -1, wfile_path, temp_size);

				fstream target_file(wfile_path, ios_base::in | ios_base::binary);
				if (!target_file.is_open())
				{
					send(ClientSocket, "weiyibiaoshifu@send file error", 60, 0);
					std::wcout << L"send file error,open error\n";
					start_send_file = false;
					continue;
				}
				char buf_temp[1024];
				target_file.seekg(0, std::ios_base::end);
				file_size = target_file.tellg();
				if (target_file.is_open())target_file.close();

				MyString::DwordlongToChar(file_size, send_file_size);
				std::wcout << L"file size:" << file_size << endl;
				//2.1.3 send message to server,
				//message="weiyibiaoshifu@send file head#"+src_path +"$"+file_size
				char file_name[100];
				MyString::myStrSubSplitR(send_file_path, file_name, '/');
				MyString::myStrcp(buf_temp, "weiyibiaoshifu@send file head#");
				MyString::myStrcat(buf_temp, send_src_file_path);
				MyString::myStrcat(buf_temp, "/");
				MyString::myStrcat(buf_temp, file_name);
				MyString::myStrcat(buf_temp, "$");
				MyString::myStrcat(buf_temp, send_file_size);
				MyString::myStrcat(buf_temp, "&");
				MyString::myStrcat(buf_temp, send_file_location);
				DWORDLONG speech = MyString::charToDwordlong(send_file_speech);
				
				//send(ClientSocket, buf_temp, MyString::myStrLen(buf_temp) + 1, 0);
				thread sendThread(sendFileThread, send_file_path, speech,buf_temp);
				sendThread.detach();
				ret = 0;
				continue;
			}
				//2.14 if sending file and get command to kill it
			if (start_send_file == true && send_state == true)
			{
				if (MyString::myStrSame(read_buf, "weiyibiaoshifu@stop send to me"))
				{
					send_state = false;
					continue;
				}
			}

			//2.2 recv file 
			if (start_recv_file)									//recv file
			{
				if (file_size <= recv_file_size || MyString::myStrSame(read_buf, "@E"))
				{
					start_recv_file = false;
					recv_file_size = 0;
					if (file.is_open())file.close();
					std::wcout << L"\n\nfile receive over\n\n";
					continue;
				}
				file.write(read_buf, ret);
				recv_file_size += ret;
				std::wcout << L"\n\n\nwrite file:" << recv_file_size << endl;
				file.flush();
				if (file_size <= recv_file_size || MyString::myStrSame(read_buf, "@E"))
				{
					start_recv_file = false;
					recv_file_size = 0;
					if (file.is_open())file.close();
					std::wcout << "\n\nfile receive over\n\n";
					continue;
				}
				continue;
			}
			//2.3 terminal self process
			if (MyString::myStrSame(read_buf, "weiyibiaoshifu@terminal it"))exit(0);


		}//end if else

	}//end while
	delete[]read_buf;
	delete[]send_buf;
	return true;
}


int main(int argc,char **argv)
{
	
	if (argc != 5 || !MyString::myStrSame(argv[1], "-c") || !MyString::myStrSame(argv[3], "-p"))return 0;
	MyString::myStrcp(ip, argv[2]);
	port=MyString::charToDwordlong(argv[4]);
	thread read_thread(readMessage);
	read_thread.join();
	
	return 0;
}

