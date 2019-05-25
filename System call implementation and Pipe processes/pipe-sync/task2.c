#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

#define print_usage(cmd) printf("Usage: %s 1|2|3\n", cmd)

int main(int argc, char * argv[]) 
{
    int option = 0;

    setbuf(stdout, NULL);

    if (argc < 2)
    {
        print_usage(argv[0]);
        return 1;
    }

    option = atoi(argv[1]);
    if (option != 1 && option != 2 && option != 3)
    {
        print_usage(argv[0]);
        return 1;        
    }

    int fd[2],fd1[2],fd2[2] ;
    char c = 0;
    pid_t parent_pid, pid1, pid2, pid3;

    pipe(fd);
    pipe(fd1);
    pipe(fd2);
    
    if(fork() == 0)
    {
	read(fd[0], &c, sizeof(c));
	printf("Child-1 (PID=%d) is running.\n", getpid());
	exit(3);
    }
    
    if(fork() == 0)
    {
	 read(fd1[0], &c, sizeof(c));
	 printf("Child-2 (PID=%d) is running.\n", getpid());
	 exit(2);
    }

    if(fork() == 0)
    {
	 read(fd2[0], &c, sizeof(c));
	 printf("Child-3 (PID=%d) is running.\n", getpid());
	 exit(1);
    }
	
    switch(option)
    {
	case 1:
		write(fd2[1], &c, sizeof(c));
		close(fd2[1]);
		pid3 = wait(NULL);
		write(fd1[1], &c, sizeof(c));
		close(fd1[1]);
		pid2 = wait(NULL);
		write(fd[1], &c, sizeof(c));
		close(fd[1]);
		pid1 = wait(NULL);
		break;

	case 2:
		write(fd1[1], &c, sizeof(c));
		close(fd1[1]);
		pid2 = wait(NULL);
		write(fd[1], &c, sizeof(c));
		close(fd[1]);
		pid1 = wait(NULL);
		write(fd2[1], &c, sizeof(c));
		close(fd2[1]);
		pid3 = wait(NULL);
		break;

	case 3:
		write(fd2[1], &c, sizeof(c));
		close(fd2[1]);
		pid3 = wait(NULL);
		write(fd[1], &c, sizeof(c));
		close(fd[1]);
		pid1 = wait(NULL);
		write(fd1[1], &c, sizeof(c));
		close(fd1[1]);
		pid2 = wait(NULL);
		break;

	default:
		printf("Execution pattern unavailable!\n");

    }

    printf("In PARENT (PID = %d): successfully reaped child %d\n", parent_pid, pid1);
    printf("In PARENT (PID = %d): successfully reaped child %d\n", parent_pid, pid2);
    printf("In PARENT (PID = %d): successfully reaped child %d\n", parent_pid, pid3);
    
    return 0;
}
