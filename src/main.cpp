#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <errno.h>

using namespace std;

void printPrompt(char* login, char* hostname);
void printPrompt(char* login);

char* getCommand();

void execute(char* command);

int main()
{
	//getting login
	char* login = new char[100];
	login = getlogin();
	
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
	string input_temp;
	cin >> input_temp;
	
	//declare the array and initialize
	char *input = new char[input_temp.length()+1];
	for(int i = 0 ; i <= input_temp.length(); i++) input[i] = '\0';
	//input = input_temp.c_str();	

	//cout << input << endl;
	
	//exectuting commands
	execute(input);
	
	delete[] input;
	
	//remember to fix the deletions
	//delete[] login;
	//delete[] hostname;

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
void execute(char* command)
{
	
	return;
}
