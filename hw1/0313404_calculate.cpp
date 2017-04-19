// 0313404 NCTU OS HW1-2_calculate

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
	
	fprintf(stderr, "I'm the child %d.\n", getpid());
	fprintf(stderr, "I can receive input! Please enter a number.\n\n");
	
	int n, sum = 1;
	
	scanf("%d",&n);
	
	for(int i = n; i > 1; i--) 
		sum *= i;

	write(STDOUT_FILENO, &sum, sizeof(int));

	fprintf(stderr, "\nThe output number is %d.\n", sum);
	fprintf(stderr, "----------------------------------------------------------\n");
	
	return EXIT_SUCCESS;
}