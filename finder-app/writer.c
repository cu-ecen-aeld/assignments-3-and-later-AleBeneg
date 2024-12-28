#include <stdio.h>
#include <string.h>
#include <syslog.h>

static void display_help (void);

int main (int argc, char *argv[])
{
	FILE *file;

	/* Check if the user requested help */
	if (argc == 1 && !(strcmp (argv[0], "-h") && strcmp (argv[0], "--help")))
	{
		display_help ();
		return 0;
	}

	openlog (NULL, 0, LOG_USER);

	/* Check if two arguments are passed */
	if (argc != 3)
	{
		printf ("Error: Two arguments are required\n");
		syslog (LOG_ERR, "Error: Two arguments are required\n");
		printf ("Usage: %s <file> <write_string>\n", argv[0]);
		return 1;
	}

	/* Open the file for writing */
	if ((file = fopen (argv[1], "w")) == NULL)
	{
		syslog (LOG_ERR, "Error: Failed to open '%s' file\n", argv[1]);
		return 1;
	}

	/* Write the string to the file */
	syslog (LOG_USER , "Writing %s to %s\n", argv[2], argv[1]);
	fprintf (file, "%s", argv[2]);

	fclose (file);
	closelog();

	return 0;
}

static void display_help (void)
{
	printf ("\n");
	printf ("Usage: $0 <file> <write_string>\n");
	printf ("\n");
	printf ("Creates a new file at the specified location and writes the provided string into it.\n");
	printf ("If the file already exists, it will be overwritten.\n");
	printf ("\n");
	printf ("Arguments:\n");
	printf ("  <file>         Full path to the file where the string will be written.\n");
	printf ("                 This can be an absolute or relative path, including the filename.\n");
	printf ("  <write_string> The text string that will be written into the specified file.\n");
	printf ("\n");
	printf ("Options:\n");
	printf ("  -h, --help     Display this help message.\n");
	printf ("\n");
}
