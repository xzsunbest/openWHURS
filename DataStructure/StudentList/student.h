#pragma once
#ifndef STUDENT_H_
#define STUDENT_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#pragma warning(disable:4996)

struct course {
	char name[5];
	int ID;
	char section[5];
};

struct student {
	char firstName[100];
	char lastName[100];
	char SSN[15];
	int course_num;
	struct course arrCourse[10];
	struct student *next;
};

// Global Linked List initially set to zero
struct student *head = NULL;

// Adds The argument student to the LinkList
void AddToList(struct student *addStudent)
{
	// malloc a new student
	struct student *cur = malloc(sizeof(struct student));
	struct student *tail = head;
	// copy contents of argument student to the new student
	strcpy(cur->firstName, addStudent->firstName);
	strcpy(cur->lastName, addStudent->lastName);
	strcpy(cur->SSN, addStudent->SSN);
	cur->course_num = addStudent->course_num;
	memcpy(cur->arrCourse, addStudent->arrCourse, 10 * sizeof(struct course));
	cur->next = NULL;
	// is linked list empty? if so, new student is head
	if (head == NULL) {
		head = malloc(sizeof(struct student));
		head = cur;
	}
	// if not, search through the linked list to find appropriate
	// spot within the linked list
	else {
		// Concerns:
		struct student *prev = head;
		while (prev != NULL) {
			if (strcmp(prev->lastName, cur->lastName) > 0) {
				// - what if new student ends up being the first? change head
				if (prev == head) {
					cur->next = head;
					head = cur;
				}
				// - what if new student ends up in the middle? update pointers
				else {
					while (tail->next != prev) {
						tail = tail->next;
					}
					tail->next = cur;
					cur->next = prev;
				}
				break;
			}
			else {
				prev = prev->next;
			}
		}
		// - what if new student is last?
		if (prev == NULL) {
			while (tail->next != NULL) {
				tail = tail->next;
			}
			tail->next = cur;
		}
	}
}

// Opens up the file for reading
// reads in the data from the file on a student by student basis
// Once the data for a student has been read
// add the student to the link list by calling AddToList
void ReadInputFile(char *filename)
{
	struct student tmp; // temporary student structure
	tmp.next = NULL;
	// open up the input file for read mode
	FILE *fp = fopen(filename, "rt");
	// if it can¡¯t open print error message
	if (!fp) {
		printf("Can't open file %s\n", filename);
		exit(EXIT_FAILURE);
	}
	// while you still have data to read
	while(!feof(fp)){
		// read one students data into temporary structure tmp
		fscanf(fp, "%s", &tmp.firstName);
		if (strcmp(tmp.firstName, "*****") == 0) {
			return;
		}
		fscanf(fp, "%s", &tmp.lastName);
		fscanf(fp, "%s", &tmp.SSN);
		fscanf(fp, "%d", &tmp.course_num);
		for (int i = 0;i < tmp.course_num;i++) {
			fscanf(fp, "%s %d %s\n", &tmp.arrCourse[i].name, &tmp.arrCourse[i].ID, &tmp.arrCourse[i].section);
		}
		// after you have gotten all the data for one student
		// call the function AddToList
		AddToList(&tmp);
	}
	fclose(fp);
}

// Opens up the file for writing
// Goes through the linked list and prints the data to the output file
void PrintToFile(char *filename)
{
	// open up file for write mode
	FILE *fp = fopen(filename, "wt");
	// print the linked list student by student to the
	// output file. Sample output in section 5.1 & 5.2
	struct student *prev = head;
	while (prev != NULL) {
		fprintf(fp, "SSN: %s\n", prev->SSN);
		fprintf(fp, "Name: %s, %s\n", prev->lastName, prev->firstName);
		fprintf(fp, "Registered For %d Classes\n", prev->course_num);
		for (int i = 0;i < prev->course_num;i++) {
			fprintf(fp, "  - %s0%d-%s\n", prev->arrCourse[i].name, prev->arrCourse[i].ID, prev->arrCourse[i].section);
		}
		fputc('\n', fp);
		prev = prev->next;
	}
	fclose(fp);
}

// Looks through the linked list for a student matching the SSN passed in
// if it finds it, deletes it from the linked list
void DeleteStudent(char *SSN)
{
	// search through the linked list to see if SSN exists
	struct student *prev = head;
	while (prev != NULL) {
		// If so, remove it from the linked list
		if (strcmp(SSN, prev->SSN) == 0) {
			struct student *tmp = malloc(sizeof(struct student));
			struct student *tail = head;
			// Concerns:
			// - what if deleted student is the head? update head
			if (prev == head) {
				tmp = head;
				head = head->next;
				tmp->next = NULL;
				free(tmp);
			}
			// - what if deleted student is in the middle? update pointers
			else if (prev->next != NULL) {
				while (tail->next != prev) {
					tail = tail->next;
				}
				tmp = prev;
				tail->next = tail->next->next;
				tmp->next = NULL;
				free(tmp);
			}
			// - what if deleted student is at the tail?
			else {
				while (tail->next != prev) {
					tail = tail->next;
				}
				tmp = prev;
				tail->next = NULL;
				free(tmp);
			}
			break;
		}
		else {
			prev = prev->next;
		}
	}
}

// Opens up the file for reading
// reads each students SSN from the file
// Deletes the student with that SSN from the list by calling DeleteStudent
void ProcessDeleteFile(char *filename)
{
	// create string deleteSSN
	char delete_SSN[15];
	// open up the file for read mode, print error message if not present
	FILE *fp = fopen(filename, "rt");
	if (!fp) {
		printf("Can't open file %s\n", filename);
		exit(EXIT_FAILURE);
	}
	// while you still have data to read
	while (!feof(fp)) {
		// read a string from the file
		fscanf(fp, "%s\n", &delete_SSN);
		if (strcmp(delete_SSN, "*****") == 0) {
			return;
		}
		// call the function DeleteStudent with argument SSN that you read in
		DeleteStudent(delete_SSN);
	}
	fclose(fp);
}

// Deletes the Entire Linked List
void DeleteList()
{
	// start from the head and remove each entry
	// from the list and free up the space via calling the function free
	struct student *tmp = malloc(sizeof(struct student));
	while (head != NULL) {
		tmp = head;
		head = head->next;
		free(tmp);
	}
	tmp = NULL;
}

#endif // !STUDENT_H_