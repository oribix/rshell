#include <queue>
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
void truncate_comment(char* input);
void execute(char* input);

int main()
{
	//getting login
	char* login = getlogin();
	
	//getting hostname
	char hostname[100];
	int hostnamecheck = gethostname(hostname, 100);
	
	//checking hostname
	if(hostnamecheck == -1) 
	{
		cerr << "error while getting host name: error number " 
			<< errno << endl;
	} 
	
	while (true)
	{
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
		
		//enable just in case we need original input
		//char inputbackup[BUFFERSIZE];
		//strcpy(inputbackup, input);

		truncate_comment(input);
		
		queue<char*> semic_splice;
		for(semic_splice.push(strtok(input, ";"))
			; semic_splice.back() != NULL
			; semic_splice.push(strtok(NULL, ";")));
		
		
		//exectuting commands
		while (semic_splice.front() != NULL)
		{
			execute(semic_splice.front());
			semic_splice.pop();
		}
	}
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

void truncate_comment(char* input)
{
	for(int i = 0 ; input[i] != '\0' ; i++)
	{
		bool found_comment = false;
		if(input[i] == '#')
		{
			input[i] = '\0';
			found_comment = true;
		}
		else if (found_comment) input[i] = '\0';
	}
	return;
}

//executes the commands
void execute(char* input)
{
	if(string(input) == "exit")
	{
		//cout << "exiting rshell" << endl; 
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
		
		vector<char*> argv;
		argv.push_back(strtok (input, " 	"));
		
		//initialize argv	
		while(argv.back() != NULL)
		{
			argv.push_back(strtok(NULL, " "));
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
		//cout << "done waiting" << endl;
	}

	return;
}



