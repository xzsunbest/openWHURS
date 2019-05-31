#include"student.h"

int main() 
{
	// Read the input file and create the linked list
	// ReadInputFile calls helper function AddToList to
	// create list in sorted order
	ReadInputFile("student.txt");
	// print the current list to the outputfile output.txt
	PrintToFile("output.txt");
	// Read and process the dropped students file delete.txt
	// It uses helper function DeleteStudent to remove
	// the from the list
	ProcessDeleteFile("delete.txt");
	// Print the updated linked list to the file update.txt
	PrintToFile("update.txt");
	// Delete the Entire Linked List before exiting the program
	DeleteList();
	return 0;
}