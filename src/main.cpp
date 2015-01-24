#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

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
	if(string(input) == "exit")
	{
		cout << "exiting" << endl; 
		exit(0);
	}
	
	int pid = fork();

	if(pid == -1)
	{
		perror("fork");
		exit(1);
	}
	else if(pid == 0) //child process
	{
		cout << "Child working" << endl;
		
		vector<char*> argv;
		argv[0] = strtok (input, " ");
		
		//if nothing was inputted, kill child
		if (argv[0] == NULL ) 
		{
			cout << "no commmand detected" << endl;
			exit(0);
		}
		
		//initialize argv	
		for (int i = 1 ; argv[i] != NULL ; i++)
		{
			argv[i] = strtok(NULL, " ");
		}
		
		if(-1 == execvp(argv[0], argv.data())) perror("exec");
		
		exit(0);
	}
	else //parent
	{
		if (-1 == wait(0))
		{
			perror("wait");
			exit(1);
		}
		cout << "done waiting" << endl;
	}

	/*
	char* token;
	
	token = strtok(input, " ");
		
	while(token != NULL)
	{
		cout << token << endl;
	
		token = strtok(NULL, " ");
	}
	*/
	return;
}



