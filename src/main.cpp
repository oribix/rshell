#include <iostream>
#include <cstdio>

using namespace std;

void printPrompt();

char* getCommand();

void execute(char* command);

int main()
{
	printPrompt();
	char *input = new char;
	cin >> input;
	//cout << "maybe here?" << endl;
	execute(input);
	
	return 0;
}

//prints the prompt
void printPrompt()
{
	cout << "$ " << flush;
	return;
}

//executes the commands
void execute(char* command)
{
	
	return;
}
