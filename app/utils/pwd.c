#include <unistd.h>
#include <stdio.h>
#define SIZE 256

int main(int argc, char const *argv[])
{
	char cwd[SIZE];

	getcwd(cwd, SIZE);

	printf("%s\n", cwd);

	return 1;
}