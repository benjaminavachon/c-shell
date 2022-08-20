#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h> 

int main(void){
	int status;
	char *args[10];

	args[0] = "/bin/sh";
	args[1] = "-c";
	args[2] = "ls";
	args[3] = "-a";
	args[4] = NULL;

	if(fork() == 0){
		execv("/bin/sh",args);
	}else{
		wait(&status);
	}

	return 0;
}
