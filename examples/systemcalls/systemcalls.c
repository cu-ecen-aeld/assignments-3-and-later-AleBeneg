#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "systemcalls.h"

/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/
bool do_system (const char *cmd)
{
	int err;

	if (cmd == NULL)
	{
		fprintf (stderr, "Error: no command provided\n");
		return false;
	}

	if ((err = system (cmd)) != 0)
	{
		if (err == -1)
			perror ("system");
		else if (err == 127)
			fprintf (stderr, "Error: Shell execution failed\n");
		else
			fprintf (stderr, "Error: Command failed with status %d\n", err);
		return false;
	}

	return true;
}

/**
* @param count -The numbers of variables passed to the function. The variables are command to execute.
*   followed by arguments to pass to the command
*   Since exec() does not perform path expansion, the command to execute needs
*   to be an absolute path.
* @param ... - A list of 1 or more arguments after the @param count argument.
*   The first is always the full path to the command to execute with execv()
*   The remaining arguments are a list of arguments to pass to the command in execv()
* @return true if the command @param ... with arguments @param arguments were executed successfully
*   using the execv() call, false if an error occurred, either in invocation of the
*   fork, waitpid, or execv() command, or if a non-zero return value was returned
*   by the command issued in @param arguments with the specified arguments.
*/
bool do_exec (int count, ...)
{
	va_list args;
	char    *command[count+1];
	int     i, pid, ret;

	va_start (args, count);

	for (i = 0; i < count; i++)
	{
		command[i] = va_arg (args, char *);
	}
	command[count] = NULL;

	ret = true;

	/* Create a new process */
	switch (pid = fork ())
	{
		case -1:
			perror ("fork");
			ret = false;
			break;

		/* Child process */
		case 0:
			if (execv (command[0], command) == -1)
			{
				perror ("execv");
				exit (1);
			}
			exit (0);

		/* Parent process */
		default:
		{
			int status;

			if (waitpid (pid, &status, 0) == -1)
			{
				perror ("waitpid");
				ret = false;
			}

			/* Check if the child process completed successfully */
			ret = (status == 0);
			break;
		}
	}

	va_end (args);
	return ret;
}

/**
* @param outputfile - The full path to the file to write with command output.
*   This file will be closed at completion of the function call.
* All other parameters, see do_exec above
*/
bool do_exec_redirect (const char *outputfile, int count, ...)
{
	va_list args;
	char    *command[count+1];
	int     i, fd, ret, pid;

	va_start (args, count);
	for(i = 0; i < count; i++)
	{
		command[i] = va_arg (args, char *);
	}
	command[count] = NULL;

	ret = true;
	if ((fd = open (outputfile, O_WRONLY|O_TRUNC|O_CREAT,
	                S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)) < 0)
	{
		perror ("open");
		ret = false;
	}
	else
	{
		switch (pid = fork ())
		{
			case -1:
				perror ("fork");
				ret = false;
				break;

			/* Child process */
			case 0:
				/* Redirect stdout to the file */
				if (dup2 (fd, STDOUT_FILENO) < 0)
				{
					perror ("dup2");
					exit (1);
				}
				close (fd);
				if (execv (command[0], command) == -1)
				{
					perror ("execv");
					exit (2);
				}
				exit (0);

			/* Parent process */
			default:
			{
				int status;

				if (waitpid (pid, &status, 0) == -1)
				{
					perror ("waitpid");
					ret = false;
				}
				ret = (status == 0);
				break;
			}
		}
	}

	close (fd);
	va_end (args);
	return ret;
}
