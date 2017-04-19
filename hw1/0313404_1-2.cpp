// 0313404 NCTU OS HW1-2

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main() {
	
	/* --- create pipe; --- */	
	int pipe_fd[2];		
	
	if(pipe(pipe_fd) == -1) {
		fprintf(stderr, "create pipe failed!\n");
		exit(EXIT_FAILURE);
	}
	
	
	/* --- create child process; --- */
	pid_t p1, p2, p3;

	fprintf(stderr, "Main process ID : %d\n", getpid());
	fprintf(stderr, "----------------------------------------------------------\n");
		
	if ((p1 = fork()) < 0) {
		fprintf(stderr, "fork failed!\n");
		exit(EXIT_FAILURE);
	}	
	/* --- child process --- */
	else if (p1 == 0) {				
		/* --- close read end --- */
		close(pipe_fd[0]);
		
		/* --- bind write end to stdout --- */
		dup2(pipe_fd[1], STDOUT_FILENO);
		
		/* --- close pipe_fd[1] --- */
		close(pipe_fd[1]);
		
		/* --- call calculate.out --- */
		execl("calculate.out", "./calculate.out", (char *)0);		
		
		/* --- The process should not go here --- */
		fprintf(stderr, "Unexpected Error!\n");
		exit(EXIT_FAILURE);		
	}
	/* --- parent process --- */
	else {
		wait(NULL);
	}

		
	if ((p2 = fork()) < 0) {
		fprintf(stderr, "fork failed!\n");
		exit(EXIT_FAILURE);
	}
	/* --- child process --- */
	else if (p2 == 0) {			
		/* --- close read end --- */
		close(pipe_fd[0]);		
		
		/* --- bind write end to stdout --- */
		dup2(pipe_fd[1], STDOUT_FILENO);
		
		/* --- close pipe_fd[1] --- */
		close(pipe_fd[1]);
		
		/* --- call calculate.out --- */
		execl("calculate.out", "./calculate.out", (char *)0);		
		
		/* --- The process should not go here --- */
		fprintf(stderr, "Unexpected Error!\n");
		exit(EXIT_FAILURE);
	}
	/* --- parent process --- */
	else {
		wait(NULL);		
	}

	
	
	if ((p3 = fork()) < 0) {
		fprintf(stderr, "fork failed!\n");
		exit(EXIT_FAILURE);
	}
	/* --- child process --- */
	else if (p3 == 0) {			
		/* --- close read end --- */
		close(pipe_fd[0]);		
		
		/* --- bind write end to stdout --- */
		dup2(pipe_fd[1], STDOUT_FILENO);
		
		/* --- close pipe_fd[1] --- */
		close(pipe_fd[1]);
		
		/* --- call calculate.out --- */
		execl("calculate.out", "./calculate.out", (char *)0);		
		
		/* --- The process should not go here --- */
		fprintf(stderr, "Unexpected Error!\n");
		exit(EXIT_FAILURE);
	}
	/* --- parent process --- */
	else {
		wait(NULL);
		close(pipe_fd[1]);
		
		int num, sum = 0;
		for(int i = 1; i <= 3; i++) {
			read(pipe_fd[0], &num, sizeof(int));
			sum += num;
		}	
		
		fprintf(stderr, "I'm the parent %d\n", getpid());
		fprintf(stderr, "Sum of results from three child processes : %d\n", sum);
		fprintf(stderr, "----------------------------------------------------------\n");
	}
return EXIT_SUCCESS;
}
