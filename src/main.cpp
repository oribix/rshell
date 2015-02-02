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
	
		//exectuting commands
		execute(input);
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
		
		//if nothing was inputted, kill child
		if (argv.size() == 0 ) 
		{
			cout << "no commmand detected" << endl;
			exit(0);
		}
		
		//initialize argv	
		while(argv.back() != NULL)
		{
			bool comment_found = false;
			
			//if the token starts with '#', replace the token with NULL
			if(argv.back()[0] == '#')
			{
				argv.pop_back();
				argv.push_back(NULL);
				comment_found = true;
			}
			else //look for any '#'s and replace it with a '\0' 
			{
				//checking for '#'
				for (int i = 0 ; argv.back()[i] != '\0' ; i++)
				{
					if (argv.back()[i] == '#')
					{
						argv.back()[i] = '\0';
						comment_found = true;
					}
				}
			}
			
			if(comment_found && argv.back() != NULL)
			{
				//cerr << "pushing a null" << endl;
				argv.push_back(NULL);
			}
			else if (!comment_found)
			{
				//cerr << "pushing another token" << endl;
				argv.push_back(strtok(NULL, " "));
			}
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



