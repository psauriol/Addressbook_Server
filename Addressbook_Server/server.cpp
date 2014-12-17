
#include "Server.h"

using namespace std;


string stringParser(string &input)//returns the first part of a string as a token and removes the token from the string
{
	string token = "";
	char delim = ' ';
	for(int i = 0; i < input.size(); i++)
	{
		if(input[i] != delim)
		{
			token += input[i];
		}
		else
		{
			input.erase(0, i + 1);
			return token;
		}
	}
	return token;
}

string databaseSearch(string searchFor)//returns name
{
	searchFor = " " + searchFor;
	string fromFile;
	string output;
	ifstream inFile("database.txt");
	if(inFile.is_open())
	{
		while (getline(inFile, fromFile))
		{
			//search string fromFile
			if(fromFile.find(searchFor) != string::npos)//if found
			{
				output = stringParser(fromFile) + " " + stringParser(fromFile);
				break;
			}
			else
			{
				cout << ".";
			}
		}
	}
	else
	{
		cout << "Failed to open database." << endl;
	}
	inFile.close();

	cout << endl;
	if(output == "")
	{
		output = "Entry not found.";
	}
	return output;
}

string databaseInsert(string insert)
{
	string output;

	string fname, lname, email;
	fname = stringParser(insert);
	lname = stringParser(insert);
	email = stringParser(insert);

	//valid email check
	bool validEmail = false;
	for(int i = 0; i < email.size(); i++)
	{
		if(email[i] == '@')
		{
			validEmail = true;
		}
	}
	//end valid email check

	if(validEmail != true)
	{
		output = "Not a valid email.";
		return output;
	}

	//actual insert code
	ofstream outFile;
	outFile.open("database.txt", ios::app);
	if (outFile.is_open())
	{
		outFile << "\n" << insert;
		outFile.close();
	}
	else
	{
		cout << "Unable to locate database.";
	}
	//end insert code
}

string processQuery(string &request)
{
	string output = "";
	string token = stringParser(request);
	if(token == "quit")
	{
		request = "quit";
		output = "Goodbye.";
		cout << "Client logged off.";
	}
	else if (token == "find")
	{
		cout << "searching...";
		output = databaseSearch(request);

	}
	else if (token == "insert")
	{
		databaseInsert(request);
		cout << "Data inserted"<< endl;
	}
	//else if (token == "delete")
	//{

	//}
	//else if (token == "modify")
	//{

	//}
	else if (token == "")
	{
		//cout << "go away" <<endl;
		output = "Please enter a command";
	}
	else
	{
		output = "Unrecognized request";
	}
	request = "";
	return output;
}


void main()
{
	//local variables
	long successful;
	WSAData WinSockData;
	WORD DLLVERSION;
	string CONVERTER;
	char MESSAGE[200];

	DLLVERSION = MAKEWORD(2,1);//concats word

	//start winsock
	successful = WSAStartup(DLLVERSION, &WinSockData);

	//create socket struct
	SOCKADDR_IN address;
	int addressSize = sizeof(address);

	//create socket
	SOCKET sock_LISTEN;
	SOCKET sock_CONNECTION;

	//socket arguements
	sock_CONNECTION = socket(AF_INET, SOCK_STREAM, NULL);
	address.sin_addr.s_addr = inet_addr("127.0.0.1");//localhost //IP address to use change as needed for computer to computer test
	address.sin_family  = AF_INET;
	address.sin_port = htons(7777);//port number

	sock_LISTEN = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sock_LISTEN, (SOCKADDR*)&address, sizeof(address));
	listen(sock_LISTEN, SOMAXCONN); //listen

	//find connection then do:
	string serverCommand = "connect";
	string clientRequest = "null";
	while (serverCommand != "quit")
	{
		cout << "Server: listening for connection...";

		if(sock_CONNECTION = accept(sock_LISTEN, (SOCKADDR*)&address, &addressSize))
		{
			cout << "Connection found." << endl;
			string serverReturn = "Server Connected";
			int temp = serverReturn.size();
			for(int i = 0; i < temp; i++)
			{
				MESSAGE[i] = serverReturn[i];
			}
			MESSAGE[temp] = NULL;
			successful = send(sock_CONNECTION, MESSAGE, 46, NULL);
			//end connect server

			//client request loop
			while(clientRequest != "quit")
			{
				//find query
				cout << "Assessing Query..." << endl;
				successful = recv(sock_CONNECTION, MESSAGE, sizeof(MESSAGE), NULL);//socket recieve
				clientRequest = MESSAGE;//convert char[] to string

				cout << "Message from client: " << clientRequest << endl;

				//process query
				serverReturn = processQuery(clientRequest);


				//serverReturn = databaseSearch(clientRequest);//search the database
				//end process query


				//send message to client
				cout << "Message to client: " << serverReturn << endl;
				int tempSize = serverReturn.size();
				for(int i = 0; i < tempSize; i++)
				{
					MESSAGE[i] = serverReturn[i];
				}
				MESSAGE[tempSize] = NULL;
				successful = send(sock_CONNECTION, MESSAGE, 46, NULL);
				//end send message to client

				clientRequest = "null";
			}
		}
	}
}