#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define  BUFFER_SIZE 1000

using namespace std;



void printPrompt(char* login, char* hostname);
void printPrompt(char* login);

void execute(char* input);

int main()
{
	//getting login
	char* login = getlogin();
	
	//getting hostname
	char* hostname = new char[100];
	int hostnamecheck = gethostname(hostname, 100);
	
	//checking hostname
	if(hostnamecheck == -1) 
	{
		cout << "error while getting host name: error number " 
			<< errno << endl;
	} 
	
	//outputting prompt
	if (hostnamecheck != -1)
	{
		printPrompt(login, hostname);
	}
	else
	{
		printPrompt(login);
	}
	
	//getting input
	char input[BUFFER_SIZE];
	cin.getline(input, BUFFER_SIZE);
	
	//exectuting commands
	execute(input);
	
	//remember to fix the deletions

	return 0;
}

//prints the prompt
void printPrompt(char* login, char* hostname)
{	
	if(login != NULL)
	{
		cout << login << flush;
	}

	cout << "@" << hostname << "$ " << flush;
	
	return;
}

void printPrompt(char* login)
{
	if(login != NULL)
	{
		cout << login << flush;
	}
	
	cout << "$ " << flush;
	
	return;
}

//executes the commands
void execute(char* input)
{
	char* token;
	
	token = strtok(input, " ");
	
	if(string(token) == "exit")
	{
		cout << "exiting" << endl; 
		exit(0);
	}
	
	//while(token != NULL)
	{
		cout << token << endl;
	
		token = strtok(input, " ");
	}
	return;
}



