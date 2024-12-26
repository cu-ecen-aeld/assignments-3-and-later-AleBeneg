#! /bin/bash

# Function to display help message
display_help()
{
	echo "Usage: $0 <directory> <search_string>"
	echo
	echo "Searches for a text string in the files of the specified directory."
	echo
	echo "Arguments:"
	echo "  <directory>     Path to the directory to search in."
	echo "  <search_string> The string to search for in the files."
	echo
	echo "Options:"
	echo "  -h, --help      Display this help message."
	exit 0
}

# Check if the user requested help
if [ "$1" == "-h" ] || [ "$1" == "--help" ]; then
	display_help
fi

# Check if two arguments are passed
if [ $# -ne 2 ]; then
	echo "Error: Two arguments are required."
	echo "Usage: $0 <directory> <search_string>"
	exit 1
fi

# Path to a directory on the filesystem
filesdir=$1

# Check if filesdir is a valid directory
if [ ! -d "$filesdir" ]; then
	echo "Error: '$filesdir' is not a valid directory."
	exit 1
fi

# Text string which will be searched
searchstr=$2

count_words=0
count_files=0

# Iterate over all files in the given directory
for f in $(find "$filesdir" -type f); do

	# Count the number of lines in each file that match the search string
	count_words_i=$(grep -nIw "$searchstr" "$f" | wc -l)

	# If there are any matching lines, increment the file counter
	if [ "$count_words_i" -gt 0 ]; then
		((count_files++))
	fi

	# Add the number of matching lines in this file to the total count of words
	((count_words+=count_words_i))
done

echo "The number of files are $count_files and the number \
of matching lines are $count_words"


