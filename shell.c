#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

void shell_loop(void);
char * read_line();
char **get_line(char *line);
int exit_shell();
int exec_command(char **args[]);
int cd(char *path);

//calls on shell_loop() thus creating the beginning of the shell...
int main(int argc, char * argv[]){
	shell_loop();
}

/*creates the main loop for the shell that is responsible for printing the current directory followed by a % 
 * and then taking commands and then calling on read_line to take the input and separate the input then caling
 * on exec_command to execute the command*/
void shell_loop(){
	char *line = malloc(1024);
	char **cmd;
	int status = 1;
	char cwd[256];
	getcwd(cwd,256);
	
	do{
		getcwd(cwd,256);
		printf(cwd);
		printf("%% ");

		line = read_line();
		cmd = get_line(line);
		status = exec_command(cmd);
		sleep(1);

		free(line);
		free(cmd);

	}while(status);

}

/*responsible for going character by characer on an input in the shell creating a pointer to a string*/

char * read_line(){
	int buffersize = 1024;
	int pos = 0;
	char * buff = malloc(sizeof(char) * buffersize);
	int c;
	
	if(!buff){
		printf("ALLOCATION ERROR\n");
		exit(EXIT_FAILURE);
	}

	while(1){
		c = getchar();
		if(c == EOF || c == '\n'){
			buff[pos] = '\0';
			return buff;
		}else{
			buff[pos] = c;
		}
		pos++;

		if(pos >= buffersize){
			buffersize += 1024;
			buff = realloc(buff, buffersize);

			if(!buff){
				printf("ALLOCATION ERROR\n");
				exit(EXIT_FAILURE);
			}
		}
	}
}


/* responsible for taking the pointer for a string and separating it at every " " and putting it in a pointer array
 * to be used later to execute commands */
char **get_line(char *line){
	char **cmd = malloc(8*sizeof(char *));
	char *sep = " ";
	char *parsed;
	char *path = "/bin/sh";
	int i = 0;

	parsed = strtok(line, sep);
	
	while(parsed != NULL){
		cmd[i] = parsed;
		parsed = strtok(NULL, sep);
		i++;
	}
	cmd[i]=NULL;

	return cmd;
}


/*responsible for executing the command by taking the input as an array of pointers and creating a child process 
 *to run each command line command. and also checks if the command is quit which then calls on exit_shell()
 *to exit the shell */
int exec_command(char **args[]){
	pid_t pid;
	int status;
	
	if(strcmp(args[0],"quit")==0){
		return exit_shell();
	}

	if(strcmp(args[0],"cd")==0){
		cd(args[1]);
	}
	
	pid = fork();
	if(pid == 0){
		if(execvp(args[0],args) < 0){
			printf("command not found\n");
		}
		exit(EXIT_FAILURE);
	}else if(pid < 0){
		printf("ERROR FORKING\n");
	}
	return 1;
}


// returns 0 thus effectively quitting the shell by terminating the shell loop.
int exit_shell(){
	return 0;
}
//returns a new path based on the argument given.
int cd(char *path){
	return chdir(path);
}
