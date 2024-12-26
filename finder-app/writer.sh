#! /bin/bash

# Function to display help message
display_help()
{
	echo
	echo "Usage: $0 <file> <write_string>"
	echo
	echo "Creates a new file at the specified location and writes the provided string into it."
	echo "If the file already exists, it will be overwritten."
	echo
	echo "Arguments:"
	echo "  <file>         Full path to the file where the string will be written."
	echo "                 This can be an absolute or relative path, including the filename."
	echo "  <write_string> The text string that will be written into the specified file."
	echo
	echo "Options:"
	echo "  -h, --help     Display this help message."
	echo
	exit 0
}

# Check if the user requested help
if [ "$1" == "-h" ] || [ "$1" == "--help" ]; then
	display_help
fi

# Check if two arguments are passed
if [ $# -ne 2 ]; then
	echo "Error: Two arguments are required."
	echo "Usage: $0 <file> <write_string>"
	exit 1
fi

# Full path to a file
writefile=$1

# Ensure the directory for the file exists
dir=$(dirname "$writefile")
if [ ! -d "$dir" ]; then
	mkdir -p "$dir" ||
	{
		echo "Error: Failed to create directory '$dir'."
		exit 1
	}
fi

# Text string to be written in the file
writestr=$2

# Create (or overwrite) the file with the provided string
echo "$writestr" > "$writefile"

if [ $? -eq 0 ]; then
	echo "Successfully wrote to '$writefile'."
else
	echo "Error: Failed to write to '$writefile'."
	exit 1
fi
