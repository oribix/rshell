#include <fcntl.h>
#include <map>
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
int execute(vector<char*> argv);
int output_redirect(vector<char*> argv);
int output_append(vector<char*> argv);
int input_redirect(vector<char*> argv);

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
		if (hostnamecheck != -1) printPrompt(login, hostname);
		else printPrompt(login);
		
		//getting input
		char input[BUFFER_SIZE];
		cin.getline(input, BUFFER_SIZE);
		
		//enable just in case we need original input
		//char inputbackup[BUFFERSIZE];
		//strcpy(inputbackup, input);

		truncate_comment(input);
		
		//splices commands at the semicolons 
		queue<char*> semic_splice;
		for(semic_splice.push(strtok(input, ";"))
			; semic_splice.back() != NULL
			; semic_splice.push(strtok(NULL, ";")));
		
		map<string, int> operator_map;
		operator_map["<"] = 1;
		operator_map[">"] = 2;
		operator_map[">>"] = 3;
		operator_map["|"] = 4;
		
		
		//executing commands
		while (semic_splice.front() != NULL)
		{
			//initializing argv
			vector<char*> argv;
			for (argv.push_back(strtok (semic_splice.front(), " 	"))
				; argv.back() != NULL
				; argv.push_back(strtok(NULL, " ")))
			{
				
				
				
				
				switch(operator_map[string(argv.back())])
				{
					case 1:// <
						{
						argv.push_back(strtok(NULL, " "));
						input_redirect(argv);
						argv.clear();
						break;
						}
					case 2:// >
						{
						//we assume that argv only has lhs and rhs
						argv.push_back(strtok(NULL, " "));
						output_redirect(argv);
						argv.clear();
						break;
						}
					case 3:// >>
						{
						argv.push_back(strtok(NULL, " "));
						output_append(argv);
						argv.clear();
						}
						break;
					case 4:// |
						
						break;
					default:
						{
						operator_map.erase(string(argv.back()));
						break;
						}
				}
				
			}
			
			
			execute(argv);
			
			semic_splice.pop();
		}
	}

	return 0;
}

//prints the prompt
void printPrompt(char* login, char* hostname)
{	
	if(login != NULL) cout << login << flush;

	cout << "@" << hostname << "$ " << flush;
	
	return;
}

void printPrompt(char* login)
{
	if(login != NULL) cout << login << flush;
	
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
int execute(vector<char*> argv)
{
	//allows the shell to exit
	if(argv.size() == 2 && string(argv[0]) == "exit") exit(0);
	
	int status;
	int pid = fork();
	
	if(pid == -1)
	{
		perror("fork");
		exit(1);
	}
	else if(pid == 0) //child process
	{		
		if(-1 == execvp(argv[0], argv.data())) perror("exec");
		
		exit(-1);
	}
	else //parent
	{
		if (-1 == wait(&status))//wait until child is done
		{
			perror("wait");
			exit(1);
		}
	}

	return status;
}

int output_redirect(vector<char*> argv)
{
	string filename = string(argv.back());
	argv.pop_back();
	argv.pop_back();
	
	int savestdout = dup(1);
	if(-1 == savestdout)
	{
		perror("dup");
		exit(-1);
	}
	
	int fd = open(filename.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0777); //fixme: filename conversion to c string is redundant
	
	if (-1 == dup2(fd, 1))
	{
		perror("dup2");
	}
	
	execute(argv);
	
	//restore stdout
	if(-1 == dup2(savestdout, 1))
	{
		perror("restore stdout");
		exit(EXIT_FAILURE);
	}
	
	if (-1 == close(fd))
	{
		perror("close");
		exit(EXIT_FAILURE);
	}
	
	return 0;
}

int output_append(vector<char*> argv)
{
	string filename = string(argv.back());
	argv.pop_back();
	argv.pop_back();
	
	int savestdout = dup(1);
	if(-1 == savestdout)
	{
		perror("dup");
		exit(-1);
	}
	
	int fd = open(filename.c_str(), O_CREAT | O_APPEND | O_WRONLY, 0777); //fixme: filename conversion to c string is redundant
	
	if (-1 == dup2(fd, 1)) perror("dup2");
	
	execute(argv);
	
	//restore stdout
	if(-1 == dup2(savestdout, 1))
	{
		perror("restore stdout");
		exit(EXIT_FAILURE);
	}
	
	if (-1 == close(fd))
	{
		perror("close");
		exit(EXIT_FAILURE);
	}
	
	return 0;
}

int input_redirect(vector<char*> argv)
{
	string filename = string(argv.back());
	int fd = open(argv.back(), O_RDONLY);
	int savestdin = dup(0);//duplicating stdin
	
	if (-1 == savestdin)
	{
		perror("dup");
	}
	
	argv.pop_back();
	argv.pop_back();
	
	if(-1 == dup2(fd, 0)) perror("dup2");
	
	execute(argv);
	
	if(-1 == dup2(savestdin, 0))
	{
		perror("restore stdin");
		exit (EXIT_FAILURE);
	}
	
	if (-1 == close(fd))
	{
		perror("close");
		exit(EXIT_FAILURE);
	}
	
	return 0;
}
