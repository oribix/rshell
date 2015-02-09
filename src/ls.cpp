#include <cstdlib>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
	char dirName[] = ".";//default directory
	
	bool flaga = false;
	//bool flagl = false;
	//bool flagR = false;

	//parsing parameters
	for (int i = 0 ; i < argc ; i++) {
		//I might need this. I dont know yet
		//int arglen;
		//for (arglen = 0 ; argv[i][arglen] != '\0' ; arglen++)

		if(argv[i][0] == '\0') { //should never trigger. It's here for protection
			cerr << "error: empty argument found" << endl;
		}
		else if(argv[i][0] == '-') {
			if(argv[i][1] == '\0') {
				//FIXME: Print a GOOD error message
				cerr << "error: '-' has no parameters"
					<< endl;
				exit(EXIT_FAILURE);
			}
			else if(argv[i][1] == '-') {
				//check -all -long -recursive
				
			}
			else {
				//check -a -l -R
				for(int j = 0 ; argv[i][j] != '\0' ; j++) {
					if (argv[i][j] == 'a') flaga = true;
					//if (argv[i][j] == 'l') flagl = true;
					//if (argv[i][j] == 'R') flagR = true;
				}
			}
		}
		else {
			//take path
		}
	}
	
	DIR *dirp = opendir(dirName);
	if(dirp == NULL) {
		perror("opendir");
		exit(EXIT_FAILURE);
	}

	dirent *direntp;
	while ((direntp = readdir(dirp))) {
		if(direntp == NULL) {
			perror("readdir");
			exit(EXIT_FAILURE);
		}
		if(flaga || (direntp->d_name)[0] != '.') {
			cout << direntp->d_name << endl;  // use stat here
		}
	}
	if (closedir(dirp) == -1) {
		perror("closedir");
		exit(EXIT_FAILURE);
	}
}