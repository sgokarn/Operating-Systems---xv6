#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) 
{
	setbuf(stdout, NULL); // disable I/O buffering - all prints are dumped to terminal immediately
	
    	FILE * file = fopen("cmds", "r");
        char line[100];
	char arr[100][100];
	int i = 0;
	char *token;
	char arr2[100][100];


	while (fgets(line, 100, file) != NULL) 
	{
		strcpy(arr[i], line);
		i++;
	}
	fclose(file);

	for(int u = 0; u<i; u++)
	{
		for(char * f = arr[u]; f = strchr(f, '\n'); ++f)
		{
			*f = '\0';
		}	
	}
	
	int x = 0;
	for(int j = 0; j<i; j++)
	{
		token = strtok(arr[j], " ");
		while(token != NULL)
		{
			strcpy(arr2[x], token);
			x++;
			token = strtok(NULL, " ");
		}	
	}

	int fd[2];
	int fd1[2];
	pid_t parent_pid = getpid();

	pipe(fd);
	pipe(fd1);
	
	if(fork() == 0)
	{
	 printf("In CHILD - 1 (PID = %d): executing command cat...\n", getpid());
	 dup2(fd1[1], STDOUT_FILENO);
	 close(fd1[0]);
	 close(fd1[1]);
	 execlp(arr2[0], arr2[0], arr2[1], (char*) NULL);
	 exit(1);
	}

	if(fork() == 0)
	{
	 printf("In CHILD - 2 (PID = %d): executing command head...\n", getpid());
	 dup2(fd1[0], STDIN_FILENO);
	 dup2(fd[1], STDOUT_FILENO);
	 close(fd1[0]);
	 close(fd1[1]);
	 execlp(arr2[2], arr2[2], arr2[3], (char*) NULL);
	 exit(2);
	}
	
	if(fork() == 0)
	{
	 printf("In CHILD - 3 (PID = %d): executing command wc...\n", getpid());
	 dup2(fd[0], STDIN_FILENO);
	 close(fd[1]);
	 close(fd1[1]);
	 close(fd[0]);
	 close(fd1[0]);
	 execlp(arr2[4], arr2[4], arr2[5], (char*) NULL);
	 exit(3);
	}
	else
	{
	 close(fd[1]);
	 close(fd1[1]);
	 close(fd[0]);
	 close(fd1[0]);
	}

	pid_t pid1 = wait(NULL);
	pid_t pid2 = wait(NULL);
	pid_t pid3 = wait(NULL);

	printf("In PARENT (PID = %d): successfully reaped child %d\n", parent_pid, pid1);
	printf("In PARENT (PID = %d): successfully reaped child %d\n", parent_pid, pid2);
	printf("In PARENT (PID = %d): successfully reaped child %d\n", parent_pid, pid3);	
	
	return 0;

}
