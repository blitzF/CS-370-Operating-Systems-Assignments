/**
 * Simple shell interface program.
 *
 * Operating System Concepts - Ninth Edition
 * Copyright John Wiley & Sons - 2013
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>





#define MAX_LINE		80 /* 80 chars per line, per command */
/*
int inputfunc (char *args[], int *value, int *flag)
{

	const char delim[4] = " \n";
	char input[MAX_LINE];
	int max_size = (MAX_LINE/2 + 1);
	int arg_in;
	int loop_count;
	int counter = 0;
	int wait_flag =0;
	//read(STDIN_FILENO, input, MAX_LINE);
	if(!fgets(input, MAX_LINE, stdin)) 
	{
	return 0;
	exit(1);
	}
	char *line_ptr = strchar(input, '\n');
	if(line_ptr) *line_ptr = 0;
	/*
	args[0] = strtok(input, delim);
	arg_in = 0;
	while(args[arg_in] != NULL)
	{
	arg_in++;
	args[arg_in] = strtok(NULL, delim);
	}
	
	for(loop_count =0; loop_count < max_size; loop_count++)
	{
	//printf("content %s", args[loop_count]);
	if(args[loop_count] != NULL)
		{counter++;}
	else
	{break;}
	}

	for(loop_count = 0; loop_count < counter; loop_count++)
	{
	//printf("%s", args[loop_count]);
	if(memcmp(args[loop_count], "&" , 1) == 0)
	{
	wait_flag = 1;
	}	
	
	}
	*value = counter;
	*flag = wait_flag;
return 1;

}
*/

int makehist(char *h[], int p)
{
	int tmp =p ;
	int h_num =1;
	do{
	if(h[tmp])
	{
	if(memcmp(h[tmp],"history",7) != 0)
	{
	printf("%d   %s\n", h_num, h[tmp]);
	h_num++;
	}
	}
	tmp = (tmp + 1) % 10;
	} while(tmp !=p);
return 0;
}


int main(void)
{
	int max_size = (MAX_LINE/2 + 1);
	char *args[max_size];	/* command line (of 80) has max of 40 arguments */
	char input[MAX_LINE];
	char *exit_ptr = "exit";
	char *wait_ptr = "&";
    	int should_run = 1;
	char *history_cmds[10];
	int present = 0;
	const char delim[4] = " \n";
	
	//int i , upper;
	int arg_in;
	int user_in = 0;
	int loop_count;
	int h_count = 0;
	
	for(loop_count =0; loop_count <10;loop_count++)
	{
	history_cmds[loop_count] = NULL;
	}
		
    while (should_run){   
	
	printf("osh>");
	fflush(stdout);
	if(!fgets(input,MAX_LINE,stdin))
	break;
	char *new_line = strchr(input,'\n');
	if(new_line)
	*new_line = 0;
	//
	char *h = strchr(input,'!');
	if(h)
	{
	if(*(input+1) == '!')
	{
	printf("char: %c", *(input+1));
	printf("asts: %s", history_cmds[0]);
	printf("ssts: %s", args[0]);
	args[0] = strdup(history_cmds[0]);
	printf("sstsssss: %s", args[0]);	
	}
	else
	{
	int a_num = (*(input+1)) - '0';
	if(history_cmds[a_num] != NULL)
	args[0] = strdup(history_cmds[a_num - 1]);
	else
	continue;
	if(a_num >9)
	{printf("Invalid Number.\n");continue;}
	}
	}
	if(!h)//	
	{
	free(history_cmds[present]);
	history_cmds[present] = strdup(input);
	present = (present + 1) % 10;
	if(memcmp(input,exit_ptr,4)==0)
	{
	break;
	exit(1);
	}
	if(memcmp(input,"history",4)==0)
	{
	if(user_in < 1)
	{
	printf("error: No history available\n");
	continue;
	}
	makehist(history_cmds,present);
	continue;
	}
	args[0] = strtok(input, delim);
	arg_in = 0;
	while(args[arg_in] != NULL)
	{
	arg_in++;
	args[arg_in] = strtok(NULL, delim);
	}
	for(loop_count =0; loop_count < max_size; loop_count++)
	{
	//printf("%s", args[loop_count]);
	//if(memcmp(args[loop_count],"\n",1)==0)
	//args[loop_count] = 0;
	if(args[loop_count] != NULL)
		{user_in++;}
	else
	{break;}
	} 
	//printf("user_in: %d : \n" , user_in);
	}//else wali bracket
	pid_t pid;
	pid = fork();     	
	if(pid == 0)
	{
	
	if(should_run == 0)
	{
	exit(1);
	}
	//printf("child");
	if(!execvp(args[0],args)) exit(1);
	perror("error: no child");
	exit(1);
	}

	else if (pid > 0)  
	{
	if(memcmp(input,wait_ptr,1)==0)
	{
	wait(NULL);
	}	
        }
	else //if (pid < 0)
	{
	printf("error: process not initialized. ");
	exit(1);
	
	} 
	

	
        /**
         * After reading user input, the steps are:
         * (1) fork a child process
         * (2) the child process will invoke execvp()
         * (3) if command included &, parent will invoke wait()
         */
    } // while bracket
    
	return 0;
}
