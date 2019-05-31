#pragma once
#ifndef HUFFMANCODE_H_
#define HUFFMANCODE_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<sys/stat.h>              
#pragma warning(disable:4996)
#define MAX_TREE_HT 101           
#define MAXCHAR 50                
#define CHARSIZE 128             
#define INPUT(fp,filename) if(!fp){printf("Can't open file %s\n",filename);exit(EXIT_FAILURE);}
#define OUTPUT(fp,filename) if(!fp){printf("Can't create file %s\n",filename);exit(EXIT_FAILURE);}

struct HuffNode {
	char data;                     
	unsigned freq;                
	struct HuffNode *lc, *rc;      
};

struct HuffCode {
	unsigned size;                  
	unsigned capacity;            
	struct HuffNode **arr;          
};

struct Character {
	char ch;
	char code[MAXCHAR - 1];
};

void HuffSort(struct HuffCode* code);
void SwapNode(struct HuffNode** a, struct HuffNode** b);
void InsertCode(struct HuffCode* code, struct HuffNode* node);
void ReadCodesFile(const char* filename, struct Character** ch);
void Statistics(const char* filename, char** data, unsigned** freq);
void PrintCode(FILE **fp, struct HuffNode* root, int arr[], int top);
void ReadInputFile(const char* filename, char** data, unsigned** freq);
void Compression(const char* input_filename, const char* output_filename, struct Character* ch, int length);
void Decompression(const char* input_filename, const char* output_filename, struct Character* ch, struct HuffNode** tree);
struct HuffNode* NewNode(char data, unsigned freq);
struct HuffNode* ExtractMinNode(struct HuffCode* code);
struct HuffNode* BuildTree(char data[], unsigned freq[], unsigned size);
struct HuffNode* Huffman(const char* filename, char data[], unsigned freq[], unsigned size);
struct HuffCode* InitializeCode(unsigned capacity);
struct HuffCode* BuildCode(char data[], unsigned freq[], unsigned size);

#endif // !HUFFMANCODE_H_