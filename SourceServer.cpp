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

static const char SERVER_IP[] = "127.0.0.1"; //default server ip (localhost)

void startThreadPorts(const int port, FILE *logFile);
void servClient(SOCKET clientInstance, int port, FILE *logfile);
void printBuffer(char* bufferPtr, int size);
void putClientOnDiferrentPort(SOCKET clientInstance, int port);
void getClientResource(SOCKET clientInstance);
void openFileWithPathAndSend(string filePath, SOCKET clientInstance);
void sendFile(FILE* m_file, SOCKET clientInstance);
string processRequest(char* bufferPtr);
string getFilePath(string p_toParse);
void printClientPortAndIP(SOCKET clientInstance, int port, FILE *logfile);
int roundRobinGetNextPort(int port);

std::mutex g_lockPrint;
std::mutex g_lockCounter;
std::recursive_mutex g_r_lock;
int g_counter{ 1 };
int g_clientId{ 0 };


int main()
{
	FILE * logFile;
	int err;
	err = fopen_s(&logFile, "logFile.txt", "wb");

	if (err == 0)//if i found the file i can send it back to browser
	{
		cout << "Log file : was created and opened." << endl;
	}
	else
	{
		cout << "PROBLEM" << endl;
	}
	// Initialises Winsock dll (raises error if failure)
	int iResult;
	WSADATA wsaData;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}

	vector<std::thread> portThreads; //this is vector which holds all my different servers on different ports
	int startPort = 2000; //the starting and also main listening port which will accept all trafic and sends out the ports for connecting
	int numServerPorts = 5;//number of ports which is server listening on

	for (int i = 0; i <= numServerPorts; i++)
	{
		portThreads.push_back(std::thread(startThreadPorts, startPort + i, logFile));//create one thread per one port
	}
	for (auto& portThread : portThreads) {
		portThread.join();
	}
	fclose(logFile);

	// Terminates Winsock dll
	WSACleanup();
	cin.ignore();
	return 0;
}


// Function to initialise the socket
void startThreadPorts(const int port, FILE *logFile)
{
	//creates my listening socket and client socket which will be different client each time
	SOCKET listenSock;
	SOCKET client;
	int iResult; //error handling

	//structure to hold socket data
	sockaddr_in addr;
	// initialises variable with size of structure addr (necessary for function accept later on)
	socklen_t sa_size = sizeof(sockaddr_in);
	// "Family" of socket (AF_INET is the standard type)
	addr.sin_family = AF_INET;
	// Port of socket
	addr.sin_port = htons(port); // htons function ensures that the port address is in the correct network format.
	// inet_pton("family", "text representation of IP address", "network representation of IP address")
	// The inet_pton function will output the IP address in the correct network format in the structure variable addr.sin_addr
	inet_pton(AF_INET, SERVER_IP, &(addr.sin_addr));

	// Actually creates the socket (called listenSock)
	listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	// Raises error if socket is invalid
	if (listenSock == INVALID_SOCKET)
	{
		wprintf(L"socket function failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
	}

	// Attaches the socket listenSock to the structure addr
	iResult = bind(listenSock, (sockaddr*)&addr, sizeof(sockaddr_in));

	// Raises error if bind function fails and tries to close the socket
	if (iResult == SOCKET_ERROR) {
		wprintf(L"bind function failed with error %d\n", WSAGetLastError());
		iResult = closesocket(listenSock);
		// Raises error if also closesocket function fails
		if (iResult == SOCKET_ERROR)
			wprintf(L"closesocket function failed with error %d\n", WSAGetLastError());
		// Terminates use of the Winsock dll 
		WSACleanup();
	}
	// Tries to listen the port of the socket listenSock, with maximum SOMAXCONN connections at the same time. Raises error if it fails.
	if (listen(listenSock, SOMAXCONN) == SOCKET_ERROR)
		wprintf(L"listen function failed with error: %d\n", WSAGetLastError());

	//  Output current thread ID
	cout << "Thread with id " << GetCurrentThreadId() << "is on port: " << port << endl;

	int clientNum = 0;
	int breakAfterServed = 1; //this is the number of clients that will be served by the server
	bool listening = true;
	while (listening)
	{	
		// exits the server when maximum number of clients is reached
		//if you want to change the number of clients than you have to change the number when server stop running
		if (clientNum == breakAfterServed && port != 2000)
		{
			cout << "BREAKING " << endl;
			std::chrono::milliseconds dura(2000);
			std::this_thread::sleep_for(dura);
			break;
		}

		if (clientNum == breakAfterServed && port == 2000)
		{
			cout << "BREAKING " << endl;
			//cin.ignore();
			std::chrono::milliseconds dura(2000);
			std::this_thread::sleep_for(dura);
			break;
		}

		// Tries to accept connections. If it works, creates and detaches thread, then executes specified operation. If it fails, raises error.
		if ((client = accept(listenSock, (sockaddr*)&addr, &sa_size)) != INVALID_SOCKET)
		{
			
			std::thread t(servClient, client, port, logFile);
			t.detach(); //this will allow the thread run on its own

			//Operation to execute on the socket
			cout << "Client: " << clientNum++ << "on port " << port << endl;


		}
		else
		{
			cout << "invalid client socket" << endl;
			cin.ignore();
		}
	}

}

void servClient(SOCKET clientInstance, int port, FILE *logfile)
{
	
	// send client on next free port
	if (port == 2000)
	{
		putClientOnDiferrentPort(clientInstance, roundRobinGetNextPort(port));
	}
	else
	{
		//this is the timing part of the code
		std::chrono::time_point<std::chrono::system_clock> start, end;
		start = std::chrono::system_clock::now();
		printClientPortAndIP(clientInstance, port, logfile);

		getClientResource(clientInstance);//this function will get the webpage for client or the error message

		end = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = end - start;
		std::time_t end_time = std::chrono::system_clock::to_time_t(end);
		wchar_t buf[26];
		errno_t err;
		err = _wctime_s(buf, 26, &end_time);
		if (err != 0)
		{
			printf("Invalid Arguments for _wctime_s. Error Code: %d\n", err);
		}
		wprintf_s(L"The client disconected at: %s", buf);
		fwrite(buf, sizeof(wchar_t), 26, logfile);
		
		string requestTook = "Request took: " + std::to_string(elapsed_seconds.count()) + " seconds. \r\n";
		cout << "Request took: " << elapsed_seconds.count() << " seconds" << endl;
		fwrite(requestTook.c_str(), sizeof(char), requestTook.size(), logfile);
	}

}

//this will guaratee that each port will get diferent number
int roundRobinGetNextPort(int port)
{	
	// Uses mutex to guarantee that other threads cannot change the variables.
	std::lock_guard<std::mutex> lock(g_lockPrint);//this allow to change the variable only in one thread

	// Increases by one client ID and port number (using integers from 1 to 5)
	g_clientId++;
	port = port + g_counter;
	g_counter++;
	if (g_counter == 6)
		g_counter = 1;

	return port;
}

void printClientPortAndIP(SOCKET clientInstance, int port, FILE *logfile)
{
	//this gets clients ip from sock_addr_in
	struct sockaddr_in addr;
	socklen_t addr_size = sizeof(struct sockaddr_in);
	int res = getsockname(clientInstance, (struct sockaddr *)&addr, &addr_size);
	sockaddr_in* pV4Addr = (struct sockaddr_in*)&addr;
	int ipAddr = pV4Addr->sin_addr.s_addr;
	char clientIp[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &ipAddr, clientIp, INET_ADDRSTRLEN);
	string cId = "ID: " + std::to_string(g_clientId) + " ";
	string portStr = "The Client port is: " + std::to_string(port) + " ";
	string clientIP = "The Client IP is: ";
	clientIP.append(clientIp);
	clientIP.append(" ");
	cout << portStr << endl;
	cout << clientIP << endl;

	fwrite(cId.c_str(), sizeof(char), cId.size(), logfile);
	fwrite(portStr.c_str(), sizeof(char), portStr.size(), logfile);
	fwrite(clientIP.c_str(), sizeof(char), clientIP.size(), logfile);

}

void putClientOnDiferrentPort(SOCKET clientInstance, int port)
{
	char buffer[1024];
	int recvMsgSize;
	int iResult;
	do {

		// Checks if data are received
		recvMsgSize = recv(clientInstance, buffer, 1024, 0);
		if (recvMsgSize > 0)
		{
			//printf("Bytes received: %d\n", recvMsgSize);
		}
		else if (recvMsgSize == 0)
		{

			//printf("Connection closed\n");
		}
		else
		{
			printf("recv failed: %d\n", WSAGetLastError());
		}

	} while (recvMsgSize > 0); // while data are received
	// stops receiving
	iResult = shutdown(clientInstance, SD_RECEIVE);
	
	//raises error if fails, closes socket and cleanup
	if (iResult == SOCKET_ERROR) {
		wprintf(L"shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(clientInstance);
		WSACleanup();
	}

	string portS = std::to_string(port);

	cout << endl;
	cout << endl;
	cout << "Next client is going to reconnect on port: " << portS << endl;

	// send data through another port
	iResult = send(clientInstance, portS.c_str(), (int)portS.size(), 0);
	// raises error and cleanup
	if (iResult == SOCKET_ERROR) {
		wprintf(L"send failed with error: %d\n", WSAGetLastError());
		closesocket(clientInstance);
		WSACleanup();
	}
	//shutdown the connection since no more data will be sent
	iResult = shutdown(clientInstance, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		wprintf(L"shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(clientInstance);
		WSACleanup();
	}

	// close the socket
	iResult = closesocket(clientInstance);
	if (iResult == SOCKET_ERROR) {
		wprintf(L"close failed with error: %d\n", WSAGetLastError());
		WSACleanup();
	}

	//cout << "SOCKET WAS CLOSED" << endl;
}

void getClientResource(SOCKET clientInstance)
{
	string filePath = { " " };
	char buffer[1024];
	int recvMsgSize;
	int bufError;

	do {

		recvMsgSize = recv(clientInstance, buffer, 1024, 0);
		if (recvMsgSize > 0)
		{
			//printf("Bytes received: %d\n", recvMsgSize);
			filePath = processRequest(buffer);
			cout << "Extracted filename: " << filePath << endl;
			//printBuffer(buffer, recvMsgSize);

			// Send file to client
			openFileWithPathAndSend(filePath, clientInstance);
			//shutdown the connection since no more data will be sent
			bufError = shutdown(clientInstance, SD_SEND);
			if (bufError == SOCKET_ERROR) {
				wprintf(L"shutdown failed with error: %d\n", WSAGetLastError());
				closesocket(clientInstance);
				WSACleanup();
			}

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

	bufError = shutdown(clientInstance, SD_RECEIVE);
	if (bufError == SOCKET_ERROR) {
		wprintf(L"shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(clientInstance);
		WSACleanup();
	}

	// close the socket
	bufError = closesocket(clientInstance);
	if (bufError == SOCKET_ERROR) {
		wprintf(L"close failed with error: %d\n", WSAGetLastError());
		WSACleanup();
	}

}

void openFileWithPathAndSend(string filePath, SOCKET clientInstance)
{
	FILE* m_file;
	errno_t err;

	// Tries to open the file
	err = fopen_s(&m_file, filePath.c_str(), "r");

	if (err == 0)//if i found the file i can send it back to browser
	{
		cout << "The file :" << filePath << " was opened." << endl;
		sendFile(m_file, clientInstance);

	}
	else//i didnt find the file i have to send 404 page not found
	{
		string responseNotFound = "HTTP/1.1 404 Not Found \r\n";
		responseNotFound.append("Content-Type: text/html \r\n");
		responseNotFound.append("<HTML><HEAD><TITLE>Not Found Camille</TITLE></HEAD><BODY>Not Found</BODY></HTML>");
		cout << "Sending file not found." << endl;
		send(clientInstance, responseNotFound.c_str(), (int)responseNotFound.size(), 0);
	}

	if (m_file)//if i openet the file i have to close it 
	{
		err = fclose(m_file);
		if (err == 0)
		{
			printf("The file was closed\n");
		}
		else
		{
			printf("The file was not closed\n");
		}
	}
}

void sendFile(FILE* m_file, SOCKET clientInstance)
{
	char statusLine[] = "HTTP/1.1 200 OK\r\n";
	char contentTypeLine[] = "Content-Type: text/html\r\n";

	fseek(m_file, 0, SEEK_END);
	int bufferSize = ftell(m_file);
	cout << "The file lenght is :" << bufferSize << endl;
	rewind(m_file);
	//this creates unique pointer to my array 
	unique_ptr<char[]> myBufferedFile = make_unique<char[]>(bufferSize);

	//this reads whole file into buffert.
	int numRead = fread_s(myBufferedFile.get(), bufferSize, sizeof(char), bufferSize, m_file);

	int totalSend = bufferSize + strlen(statusLine) + strlen(contentTypeLine);

	unique_ptr<char[]> myUniqueBufferToSend = make_unique<char[]>(totalSend);

	memcpy(myUniqueBufferToSend.get(), &statusLine, strlen(statusLine));
	memcpy(myUniqueBufferToSend.get() + strlen(statusLine), &contentTypeLine, strlen(contentTypeLine));
	memcpy(myUniqueBufferToSend.get() + strlen(statusLine) + strlen(contentTypeLine), myBufferedFile.get(), bufferSize);

	cout << "Sending response." << endl;

	int iResult = send(clientInstance, myUniqueBufferToSend.get(), totalSend, 0);
	if (iResult == SOCKET_ERROR)
	{
		wprintf(L"send failed with error: %d\n", WSAGetLastError());
		closesocket(clientInstance);
		WSACleanup();
	}

	cout << "Total bytes send: " << iResult << endl;

}

string processRequest(char* bufferPtr)
{
	string firstLine = { "" };
	//extract the first line from buffer
	while (*bufferPtr != '\r') {
		firstLine += *bufferPtr;
		bufferPtr++;
	}
	cout << "Client request: " << firstLine << endl;
	return getFilePath(firstLine);
}

//function to get the file that client want from http request using regex
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

//support function to check what is in the buffer
void printBuffer(char* bufferPtr, int size)
{

	for (int i = 0; i < size; i++) {
		cout << *bufferPtr;
		bufferPtr++;
	}

}