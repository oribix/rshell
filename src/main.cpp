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
#include <linux/limits.h>

using namespace std;


//prints prompt
void printPrompt(char* login, char* hostname);
void printPrompt(char* login);

//allows comments
void truncate_comment(char* input);

//executes commands
int execute(vector<char*> argv);

// i/o redirection
int output_redirect(vector<char*> argv);
int output_append(vector<char*> argv);
int input_redirect(vector<char*> argv);

//signal handler
void shell_handler(int i);
void process_handler(int i);

int main()
{
	if (SIG_ERR == signal(SIGINT, shell_handler)) perror("SIGINT");
	//signal(SIGTSTP, shell_handler);
	//signal(SIGCONT, NULL);
	
	//getting login
	char* login = getlogin();
	if(NULL == login) perror("getlogin");
	
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
		char input[BUFSIZ];
		cin.getline(input, BUFSIZ);
		
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
	char cwdir[PATH_MAX];
	if(NULL == getcwd(cwdir, PATH_MAX)) perror("getwd");
	
	if(login != NULL) cout << login << flush;

	cout << "@" << hostname << ":"
		<< cwdir << " $ " << flush;
	
	return;
}

void printPrompt(char* login)
{
	char cwdir[PATH_MAX];
	if(NULL == getcwd(cwdir, PATH_MAX)) perror("getwd");
	
	if(login != NULL) cout << login << flush;
	
	cout << ":" << cwdir << " $ " << flush;
	
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
	//FIXME: Make this into switch statements

	//allows the shell to exit
	if(argv.size() == 2 && string(argv[0]) == "exit") exit(0);
	if(argv.size() == 3 && string(argv[0]) == "cd")
	{
		if(-1 == chdir(argv[1]))
		{
			perror("cd");
			return 1;
		}
		return 0;
	}
	if(argv.size() == 2 && string(argv[0]) == "fg")
	{
		if (0 != raise(SIGCONT))
		{
			perror("SIGCONT");
			exit(EXIT_FAILURE);
		}
		return 0;
	}
	if(argv.front() == NULL) return 0;
	
	int status;
	int pid = fork();
	
	if(pid == -1)
	{
		perror("fork");
		exit(1);
	}
	else if(pid == 0) //child process
	{		
		if (SIG_ERR == signal(SIGINT, NULL)) perror("SIGINT");
		//signal(SIGTSTP, NULL);
		//signal(SIGCONT, NULL);
		
		//getting list of paths
		char* pathlist = getenv("PATH");
		if (pathlist == NULL)
		{
			perror("getenv");
			exit(EXIT_FAILURE);
		}
		
		//DONT DELETE THIS. strtok skrews up the pathlist
		//if passed directly
		char pathtemp[BUFSIZ];
		strcpy(pathtemp, pathlist);
		
		//intializing queue of pathnames
		queue<char*> pathnames;
		for (pathnames.push(strtok(pathtemp, ":"))
			;pathnames.back() != NULL
			;pathnames.push(strtok(NULL, ":")));
		
		//go through every possible path
		while(pathnames.front() != NULL)
		{
			//creating path for function call
			char path[BUFSIZ];
			strcpy(path, pathnames.front());
			if(argv[0][0] == '/') strcat(path, argv[0]);
			else
			{
				strcat(path, "/");
				strcat(path, argv[0]);
			}
			
			//executing function
			if(-1 == execv(path, argv.data()))
			pathnames.pop();
		}
		perror("exec");
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
	argv.push_back(NULL);
	
	int savestdout = dup(1);
	if(-1 == savestdout)
	{
		perror("dup");
		exit(-1);
	}
	
	int fd = open(filename.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0777); //fixme: filename conversion to c string is redundant
	if (-1 == fd) perror("open");
	
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
	argv.push_back(NULL);

	int savestdout = dup(1);
	if(-1 == savestdout)
	{
		perror("dup");
		exit(-1);
	}
	
	int fd = open(filename.c_str(), O_CREAT | O_APPEND | O_WRONLY, 0777); //fixme: filename conversion to c string is redundant
	if (-1 == fd) perror("open");
	
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
	argv.push_back(NULL);
	
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

void process_handler(int i)
{
	switch(i)
	{
		case SIGINT:
		{
			exit(EXIT_SUCCESS);
		}
		
		//case SIGTSTP:
		//{
		//	break;
		//}
	}
	return;
}

void shell_handler(int i)
{
	switch(i)
	{
		case SIGINT:
		{
			break;
		}
		
		//case SIGTSTP:
		//{
		//	break;
		//}
	}
	return;
}
