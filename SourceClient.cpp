//mainthreadsock define the entry point for console application

#define WIN32_LEAN_AND_MEAN

#include <ws2tcpip.h>
#include <winsock2.h>
#include <process.h>
#include <memory>
#include <thread>
#include <stdio.h>
#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <mutex>
#include <chrono>
#include <fstream>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
using std::cin;		using std::cout;
using std::endl;	using std::string;
using std::regex;   using std::regex_token_iterator;
using std::unique_ptr;
using std::make_unique;
using std::thread;
using std::vector;
using std::ofstream;


static const char SERVER_IP[] = "127.0.0.1";//the default ip for client connecting

void clientThread(const int port, int clientNum);
void printBuffer(char* bufferPtr, int size);
void selectOneWebpage(string &sendbuf);
void saveDownloadedPage(string filePath, string text);
string getFilePath(string p_toParse);

std::recursive_mutex g_r_lock;
std::mutex g_mutex;
int g_webpage = 0;

int main()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	vector<std::thread> clientThreads;//create thread for each client we want to create
	int startPort = 2000; //the default port which will be redirectet to another port
	int numClientsCreated = 1;//the number of clients that will be created
	int sleepBeforeClient = 100;//the time that will be between creating clients

	for (int i = 0; i < numClientsCreated; i++)
	{
		std::chrono::milliseconds dura(sleepBeforeClient);
		std::this_thread::sleep_for(dura);
		clientThreads.push_back(std::thread(clientThread, startPort, i));

	}

	for (auto& clientThread : clientThreads) {
		clientThread.join();
	}

	WSACleanup();
	cin.ignore();
	return 0;


}


void clientThread(const int port, int clientNum)
{
	SOCKET sock;
	int iResult; //the error handling
	int filePath;
	sockaddr_in addr;
	socklen_t sa_size = sizeof(sockaddr_in);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	inet_pton(AF_INET, SERVER_IP, &(addr.sin_addr));
	ofstream myFlow;

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET) {
		wprintf(L"socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
	}

	iResult = connect(sock, (SOCKADDR*)&addr, sizeof(sockaddr_in));
	if (iResult == SOCKET_ERROR) {
		wprintf(L"connect failed with error: %d\n", WSAGetLastError());
		closesocket(sock);
		WSACleanup();
	}

	cout << "Thread with id " << GetCurrentThreadId() << "is on port: " << port << "with client number: " << clientNum << endl;

	string sendbuf{ " " };
	selectOneWebpage(sendbuf);

	char recvBuffer[1024];
	string receivedFile{ "" };

	iResult = send(sock, sendbuf.c_str(), sendbuf.size(), 0);
	if (iResult == SOCKET_ERROR) {
		wprintf(L"send failed with error: %d\n", WSAGetLastError());
		closesocket(sock);
		WSACleanup();
	}

	int recvMsgSize = 0;

	iResult = shutdown(sock, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		wprintf(L"shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(sock);
		WSACleanup();
	}
	do {

		recvMsgSize = recv(sock, recvBuffer, 1024, 0);

		if (recvMsgSize > 0) {

			receivedFile.append(recvBuffer, recvMsgSize);
		}
		else if (recvMsgSize == 0)
		{
			//printf("Connection closed\n");
		}
		else
		{
			printf("recv failed: %d\n", WSAGetLastError());
		}

	} while (recvMsgSize > 0);

	iResult = shutdown(sock, SD_RECEIVE);
	if (iResult == SOCKET_ERROR) {
		wprintf(L"shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(sock);
		WSACleanup();
	}
	iResult = closesocket(sock);
	if (iResult == SOCKET_ERROR) {
		wprintf(L"close failed with error: %d\n", WSAGetLastError());
		WSACleanup();
	}
	//cout << "socket closed" << endl;
	//get the right port or resource dosplayed
	if (receivedFile.size() == 4)//need to reconect
	{
		clientThread(std::stoi(receivedFile), clientNum);
	}
	else//display html and save to directory
	{
		cout << "Whole File: " << receivedFile << endl;
		cout << getFilePath(sendbuf) << endl;
		//filePath = getFilePath(sendbuf);
		//ofstrea
	}
}

void selectOneWebpage(string &sendbuf)
{
	if (g_webpage == 0)
	{
		//sendbuf = "GET /‪C:/Users/camille/Desktop/example.txt HTTP1.1 \r\n";
		sendbuf = "GET /C:/Users/ALESSANDRO/Downloads/Prova.txt HTTP1.1 \r\n";
		
		g_webpage++;
	}
	else if (g_webpage == 1)
	{
		//sendbuf = "GET /C:/Users/camille/Desktop/example.txt HTTP1.1 \r\n";
		sendbuf = "GET /C:/Users/ALESSANDRO/Downloads/Prova.txt HTTP1.1 \r\n";
		//C:\Users\ALESSANDRO\Downloads\Fwd__Code_monthly_rebalancing
		
		g_webpage++;
	}
	else
	{
		sendbuf = "GET /notfound.html HTTP1.1 \r\n";
		g_webpage = 0;
	}
}

void printBuffer(char* bufferPtr, int size)
{

	for (int i = 0; i < size; i++) {
		cout << *bufferPtr;
		bufferPtr++;
	}
}

static const string REGEX_GET = R"((GET)\s\/(.+)\s(HTTP.+))";
string getFilePath(string p_toParse)
{
	regex rx(REGEX_GET);
	string extractedSubmatchPath = { "" };

	std::smatch pieces_match;
	if (std::regex_match(p_toParse, pieces_match, rx))
	{

		std::ssub_match sub_match = pieces_match[2];
		extractedSubmatchPath = sub_match.str();

	}
	return extractedSubmatchPath;//if there is no match so the request is not HTTP it will return empty string
}