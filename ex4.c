#include <stdio.h>
#include <unistd.h>

#define N 256

#define READ  0
#define WRITE 1

int main(int argc, char *argv[0])
{
	int processNumber;
	int n, i, num;
	int p[N][2];

	switch(argc)
	{
		case 1:
			printf("Please enter number of processes to create: ");
			scanf("%d", &n);
			break;
		case 2:
			sscanf(argv[1], "%d", &n);
			break;
		default:
			printf("Illegal number of arguments passed.\nPlease pass number of processes to create");
			return 0;
	}

	if (n < 1 || n >= N)
	{
		printf("Illegal number of processes received.\n");
		return 0;
	}

	for(i = 0; i < n; i++) 
	{
		pipe(p[i]);
	}

	processNumber = 0;

	for(i = 1; i < n; i++)
	{
		switch(fork())
		{
			case -1:
				printf("Unexpected error occured during creating a process.\n");
				return 0;
			case 0: 
				processNumber = i;				
				break;
			default:
				i = n;
				break;
		}
	}

	if (processNumber == 0)
	{
		num = 1;
		close(p[0][READ]);
		write(p[0][WRITE], &num, sizeof(int));
		close(p[n - 1][WRITE]);
		read(p[n - 1][READ], &num, sizeof(int));

		printf("The test is %spassed.\n", num == n ? "" : "not ");
	} 
	else
	{
		close(p[processNumber - 1][WRITE]);
		read(p[processNumber - 1][READ], &num, sizeof(int));
		num++;
		close(p[processNumber][READ]);
		write(p[processNumber][WRITE], &num, sizeof(int));
	}

	return 0;
}
