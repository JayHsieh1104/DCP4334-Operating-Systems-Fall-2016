// 0313404 NCTU OS HW1-1

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main() {
	
	pid_t p1, p2, p3;

	printf("Main process ID : %d\n\n", getpid());

	p1 = fork();
	
	if (p1 < 0)
		printf("error in fork!\n");
	else if (p1 == 0)
		printf("fork 1. I'm the child %d, my parent is %d\n", getpid(), getppid());
	else {
		wait(NULL);
		printf("fork 1. I'm the parent %d, my child is %d\n", getpid(), p1);
	}

	p2 = fork();
	
	if (p2 < 0)
		printf("error in fork!\n");
	else if (p2 == 0)
		printf("fork 2. I'm the child %d, my parent is %d\n", getpid(), getppid());
	else {
		wait(NULL);
		printf("fork 2. I'm the parent %d, my child is %d\n", getpid(), p2);
	}

	p3 = fork();
	
	if (p3 < 0)
		printf("error in fork!\n");
	else if (p3 == 0)
		printf("fork 3. I'm the child %d, my parent is %d\n", getpid(), getppid());
	else {
		wait(NULL);
		printf("fork 3. I'm the parent %d, my child is %d\n", getpid(), p3);
	}
	
return 0;
}
