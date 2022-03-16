#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
int main()
{
	char buffer[256];
	char *name = "/gf2d";
	int exit_status = 2;
	getcwd(buffer,sizeof(buffer));
	strcat(buffer,name);
	int retval = 2;
	do
	{
		pid_t pid = fork();
		if(pid==0)
		{
			execl(buffer,"/gf2d",(char*)NULL);
			printf("exec");
			
		}
		int status;
		 
		waitpid(pid, &status, 0);
	 
		if ( WIFEXITED(status) )
		{
			exit_status = WEXITSTATUS(status);       
		}
    }while(exit_status);
	return 0;
}
