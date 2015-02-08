#include <cstdlib>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <iostream>

using namespace std;

/*
 *  * This is a BARE BONES example of how to use opendir/readdir/closedir.
 *   * Notice that there is no error checking on these functions.
 *    * You MUST add error checking yourself.
 *     */

int main()
{
	char dirName[] = ".";
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
		cout << direntp->d_name << endl;  // use stat here
	}
	if (closedir(dirp) == -1) {
		perror("closedir");
		exit(EXIT_FAILURE);
	}
}
