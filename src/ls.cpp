#include <string>
#include <sstream>
#include <math.h>
#include <grp.h>
#include <pwd.h>
#include <iomanip>
#include <ctime>
#include <cstring>
//#include <algorithm>
#include <queue>
#include <list>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <iostream>
#include <unistd.h>

using namespace std;

list<string> printcontents(const char* dirName, bool flaga, bool flagl);

void recursionprint(const char* dirName, bool flaga, bool flagl);

int main(int argc, char *argv[])
{
	char defdir[] = "./";//default directory
	char *dirName = defdir;//lets me change the directory
	
	bool flaga = false;
	bool flagl = false;
	bool flagR = false;
	bool pathfound = false;

	//parsing parameters
	for (int i = 0 ; i < argc ; i++) {
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
				if (string(argv[i]) == "--all") flaga = true;
				if (string(argv[i]) == "--long") flagl = true;
				if (string(argv[i]) == "--recursive") flagR = true;
				if(string(argv[i]) != "--all"
					&& string(argv[i]) != "--long"
					&& string(argv[i]) != "--recursive")
				{
					cout << "error: Unreconized flag: " << argv[i] << endl;
					exit(EXIT_FAILURE);
				}
				
			}
			else {
				//check -a -l -R
				for(int j = 1 ; argv[i][j] != '\0' ; j++) {
					if (argv[i][j] == 'a') flaga = true;
					if (argv[i][j] == 'l') flagl = true;
					if (argv[i][j] == 'R') flagR = true;
					if(argv[i][j] != 'a'
						&& argv[i][j] != 'l'
						&& argv[i][j] != 'R')
					{
						cout << "error: Unreconized flag: " << argv[i] << endl;
						exit(EXIT_FAILURE);
					}
				}
			}
		}
		else if (i != 0){ //skip the first argument
			//take path
			if(!pathfound) {
				dirName = argv[i];
				pathfound = true;
			}
			else {
				cerr << "error: too many path parameters" << endl;
				exit(EXIT_FAILURE);
			}
		}
	}//end parsing parameters
	
	if(flagR) {
		recursionprint(dirName, flaga, flagl);
	}
	else {
		printcontents(dirName, flaga, flagl);
	}
	return 0;
}

list<string> printcontents(const char* dirName, bool flaga, bool flagl) {
	list<string> directorylist;
	
	DIR *dirp = opendir(dirName);
	if(dirp == NULL) {
		perror("opendir");
		exit(EXIT_FAILURE);
	}

	dirent *direntp;
	
	list<string> filenames;//queue of filenames so that we can sort alphabetically 
	
	while ((direntp = readdir(dirp))) {
		if(direntp == NULL) {
			perror("readdir");
			exit(EXIT_FAILURE);
		}
		if(flaga || (direntp->d_name)[0] != '.') {//checks for -a
			filenames.push_back(string(direntp->d_name));
			//cout << direntp->d_name << endl;  // use stat here
		}
	}
	
	filenames.sort();//strict weak sort of the filenames
	
	int nlinkwidth = 0;
	unsigned userwidth = 0;
	unsigned groupwidth = 0;
	int bytewidth = 0;
	
	//initialize the width of the columns
	for(list<string>::iterator it = filenames.begin() ; it != filenames.end() ; it++) {
		struct stat filestats;
		string file = string(dirName) + *it;
		if( -1 == stat(file.c_str(), &filestats)) {
			perror("stat");
			exit(EXIT_FAILURE);
		}
		long nlinktemp = filestats.st_nlink;
		int nlinklength = floor(log10(abs(nlinktemp))) + 1;
		
		struct passwd *username = getpwuid(filestats.st_uid);
		if(username == NULL) {
			perror("getpwuid");
			exit(EXIT_FAILURE);
		}
		
		struct group *groupname = getgrgid(filestats.st_gid);
		if(groupname == NULL) {
			perror("getpwgid");
			exit(EXIT_FAILURE);
		}
		
		int bytelength = floor(log10(abs(filestats.st_size))) + 1;
		
		if(nlinklength > nlinkwidth) nlinkwidth = nlinklength;
		if(string(username->pw_name).size() > userwidth)
			userwidth = string(username->pw_name).size();
		if(string(groupname->gr_name).size() > groupwidth)
			groupwidth = string(groupname->gr_name).size();
		if (bytelength > bytewidth) bytewidth = bytelength;
	}
	
	//print the columns
	while (filenames.size() != 0) {
		struct stat s;
		string file = string(dirName) + filenames.front();
		if( -1 == stat(file.c_str(), &s)) {
			perror("stat");
			exit(EXIT_FAILURE);
		}
		
		if(flagl) {
			//cerr << "files: " << filenames.front() << endl;
			
			
			//takes up 10 spaces
			if(s.st_mode & S_IFDIR) cout << "d";
			else cout << "-";
			cout << ((s.st_mode&S_IRUSR)?"r":"-");
			cout << ((s.st_mode&S_IWUSR)?"w":"-");
			cout << ((s.st_mode&S_IXUSR)?"x":"-");
			cout << ((s.st_mode&S_IXGRP)?"r":"-");
			cout << ((s.st_mode&S_IXGRP)?"w":"-");
			cout << ((s.st_mode&S_IXGRP)?"x":"-");
			cout << ((s.st_mode&S_IXOTH)?"r":"-");
			cout << ((s.st_mode&S_IXOTH)?"w":"-");
			cout << ((s.st_mode&S_IXOTH)?"x":"-") << " " << flush;
			
			char *timestring = ctime(&s.st_mtime);
			//time_t *currenttime = time(NULL);
			
			struct passwd *username = getpwuid(s.st_uid);
			if(username == NULL) {
				perror("getpwuid");
				exit(EXIT_FAILURE);
			}
			
			struct group *groupname = getgrgid(s.st_gid);
			if(groupname == NULL) {
				perror("getpwgid");
				exit(EXIT_FAILURE);
			}
			
			cout << setw(nlinkwidth) << right << s.st_nlink << " ";
			cout << setw(userwidth) << left << username->pw_name << " ";
			cout << setw(groupwidth) << left << groupname->gr_name << " ";
			cout << setw(bytewidth) << right << s.st_size << " ";
			//output month day hour:minute
			for(int i = 4 ; i < 16 ; i++) cout << timestring[i];
			cout << " " << flush;
			
		}
		
		cout << filenames.front() << flush;
		
		if(flagl) cout << endl;
		else cout << " " << flush;
		
		if(s.st_mode & S_IFDIR && filenames.front() != ".." && filenames.front() != ".")
			directorylist.push_back(filenames.front());
		
		filenames.pop_front();
	} 
	if (!flagl) cout << endl;
	
	if (closedir(dirp) == -1) {
		perror("closedir");
		exit(EXIT_FAILURE);
	}
	return directorylist;
}

void recursionprint( const char* dirName, bool flaga, bool flagl) {
	cout << dirName << ": " << endl;
	
	list<string> directorylist = printcontents(dirName, flaga, flagl);
	
	cout << endl;
	
	if(directorylist.empty()) return;
	else {
		while (!directorylist.empty()) {
			string nextdirectory;
			if(string(dirName).at(string(dirName).size() - 1) == '/')
				nextdirectory = string(dirName) + directorylist.front() + "/";
			else {
				nextdirectory = string(dirName) + "/" + directorylist.front() + "/";
			}
			recursionprint(nextdirectory.c_str(), flaga, flagl);
			directorylist.pop_front();
		}
	}
	
	return;
}
