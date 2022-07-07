// C++ program for creating a socket using Winsock API
#include<stdio.h>
#include<iostream>
#include<string.h>
#include<sstream>
#include<tchar.h>
// winsock api header file
#include <winsock2.h>
// tcp/ip functionality
#include<ws2tcpip.h>
constexpr auto MAX_SIZE = 1024;

#pragma comment(lib, "Ws2_32.lib")

using std::cout;
using std::endl;
using std::string;
using std::stringstream;

//#include "SQLAPI.h"		 // main SQLAPI

int	main(int c, char* argv[])
{
	printf("Hello!\n");

	// Initialize winsock by loading winsock DLL
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);
	int wsa_err;
	wsa_err = WSAStartup(wVersionRequested, &wsaData);

	// create client socket
	SOCKET client_socket = INVALID_SOCKET;
	client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	/*if (wsa_err != 0)
	{
		cout << "The winsock dll not found!" << endl;
	}

	else {
		cout << "The winsock dll is found!" << endl;
		cout << "The system status: "<<wsaData.szSystemStatus << endl;
	}*/

	if (client_socket != INVALID_SOCKET)
		cout << "socket " << client_socket << " is working" << endl;

	// connect client socket to server socket defined in another application

	sockaddr_in clientservice;
	clientservice.sin_family = AF_INET;
	InetPton(AF_INET, _T("127.0.0.1"), &clientservice.sin_addr.s_addr);
	int port = 55555;
	clientservice.sin_port = htons(port);

	if (connect(client_socket, (sockaddr*)&clientservice, sizeof(clientservice)) != SOCKET_ERROR) {
		cout << "Connected to server" << endl;


	}
	//system("pause");
	//closesocket(client_socket);
	char receive_buffer[MAX_SIZE];
	// free resources by de-registering winsock DLL
	//WSACleanup();
	//int byte_no = recv(client_socket, receive_buffer, sizeof(receive_buffer), NULL);
	//if (byte_no > 0) cout << receive_buffer << endl;

	// receive integer and float from server
	unsigned char data_buffer[MAX_SIZE];
	/*int a; float f;
	bool flag ;*/
	char rec_schema[100]{};
	//int bytes = recv(client_socket, (char*)rec_schema, 100, NULL);
	while (recv(client_socket, (char*)rec_schema, 100, NULL) > 0)
	{
		// change rec_schema in server side
		cout << "--------------------------------------------------------------------------" << endl;
		if (strlen(rec_schema) == 0) {
			
			cout << "The current file is empty. There is nothing to read"<<endl;
				continue;
		}

			string file_schema(rec_schema);
			cout << "The schema of message is: ";
			cout << file_schema << endl;


		stringstream ss(file_schema);
		recv(client_socket, (char*)data_buffer, MAX_SIZE, NULL);
		cout << "Received from file:  " << endl;
		int ptr = 0;
		string field_type;
		while (getline(ss, field_type, ','))
		{
			if (ptr >= MAX_SIZE) break;
			//cout <<"now" << field_type << endl;
			if (field_type.substr(0, 4) == "CHAR")
			{
				//memcpy_s;
				//string value;
				int string_size = stoi(field_type.substr(4));
				//cout << string_size << "test" << endl;
				//value.resize(string_size);
				char string_val[100]{};
				memcpy(string_val, data_buffer + ptr, string_size);

				cout << "string value is " << string_val << endl;
				ptr += string_size;
			}

			if (field_type == "INT")
			{
				int value = 0;
				memcpy(&value, data_buffer + ptr, sizeof(int));
				cout << "Integer value is " << value << endl;
				ptr += sizeof(int);
			}

			if (field_type == "FLOAT")
			{
				float value;
				memcpy(&value, data_buffer + ptr, sizeof(float));
				cout << "Float value is " << value << endl;
				ptr += sizeof(float);
			}
		}
		//memset(data_buffer, 0, 1024);
		 memset(rec_schema,0,100);
		//memset(rec_schema, 0, 100);
		// cout << "--------------------------------------------------------------------------" << endl;
	}
	cout << "Server has stopped sending mess.Thank you and restart program on server!" << endl;
	return 0;
}
