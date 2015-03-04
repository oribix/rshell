#include <fstream>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <string>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/stat.h>
#include "Timer.h"

using namespace std;

void getput(const string src, string dst);
void rwchar(string src, string dst);
void rwbuff(string in, string out);

int main() {
	
	string srcfile;
	cout << "enter source file: " << flush;
	cin >> srcfile;
	cout << endl;
	
	string dstfile;
	cout << "enter destination file: " << flush;
	cin >> dstfile;
	cout << endl;
	
	string choice;
	cout << "which function?: ";
	cin >> choice;
	cout << endl;

	if(choice == "1") getput(srcfile, dstfile);
	else if (choice == "2") rwchar(srcfile, dstfile);
	else if (choice == "3") rwbuff(srcfile, dstfile);
	else {
		cout << "invalid choice of function." << endl;
	}
}

void getput(const string src, string dst){
	//opening file to read
	ifstream is;
	is.open(src.c_str());
	//error checking input
	if(!is.is_open()) {
		cerr << "problem opening input file" << endl;
		exit(EXIT_FAILURE);
	}
	
	//opening output file
	ofstream outfile;
	outfile.open(dst.c_str());
	//error checking output
	if(!outfile.is_open()) {
		cerr << "problem opening output file" << endl;
		exit(EXIT_FAILURE);
	}	
	
	char c;
	Timer t;
	double eTime;
	t.start();
	
	while (!is.eof()){
		is.get(c);
		outfile.put(c);
	};
	
	t.elapsedUserTime(eTime);
	cout <<  "Elapsed time: " << eTime << endl;
	
	is.close();
	outfile.close();
	return;
}

void rwchar(string src, string dst) {
	//opening file to read
	int fd_src = open(src.c_str(), O_RDONLY);
	if (fd_src == -1) {
		perror ("open src file");
	}
	
	int fd_dst = open(dst.c_str(), O_WRONLY | O_CREAT | O_EXCL);
	
	if(fd_dst == -1) {
		perror("open dst file");
	}
	
	char buf[1];
	int ret;
	Timer t;
	double eTime;
	t.start();
	do{
		ret = read(fd_src, buf, 1);
		if(ret == -1) {
			perror("read");
		}
			
		if(-1 == write(fd_dst, buf, ret)) {
			perror("write");
		}
	}while(ret != 0);
	t.elapsedUserTime(eTime);
	cout << "elapsed time: " << eTime << endl;
	
	if(close(fd_src) == -1) perror("close fd_src");
	if(close(fd_dst) == -1) perror("close fd_dst");
	
	return;
}

void rwbuff(string in, string out) {
     	int fd = open(in.c_str(), O_RDONLY);
	if (fd == -1)
        {
		perror("open src file");
	}

 	int ofd = open(out.c_str(), O_WRONLY | O_CREAT | O_EXCL);
 	if(ofd == -1) 
        {
		perror("open dst file");
	}

	char buf[BUFSIZ];
        Timer t;
	double eTime;
	t.start();	
	int bytes_read = read(fd, buf, BUFSIZ);
 	if(bytes_read == -1)
        {               
        	perror("read");
        }
        
        int bytes_written = write(ofd, buf, bytes_read);
	if(bytes_written == -1)
        {            
        	perror("write");
        }
        t.elapsedUserTime(eTime);
	cout << "Elapsed time: " << eTime << endl;
	
        if(-1 == close(fd)) perror("close src");
	if(-1 == close(ofd)) perror("close dest");

	return;
}	
