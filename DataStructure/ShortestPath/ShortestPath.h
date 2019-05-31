#ifndef SHORTESTPATH_H_
#define SHORTESTPATH_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#pragma warning(disable:4996)
#define MAXV 10050

struct ListDemo {
	unsigned ID;
	char country[64];
	char city[64];
	double longitude;
	double latitude;
};

struct ListDemoArray {
	unsigned V;
	struct ListDemo** demo_array;
};

struct ListNode {
	struct ListDemo demo;
	unsigned dest_ID;
	char tool[8];
	double time_weight;
	double cost_weight;
	char domain[1024];
	struct ListNode* next;
};

struct List {
	struct ListNode* head;
};

struct Graph {
	unsigned V;
	struct List* array;
};

struct MinHeapNode {
	unsigned V;
	double dist;
};

struct MinHeap {
	unsigned size;
	unsigned capacity;
	unsigned* pos;
	struct MinHeapNode** heap_array;
};

bool isEmpty(struct MinHeap* heap);
bool isInMinHeap(struct MinHeap* heap, unsigned V);
int DemoCompare(const void* a, const void* b);
void s_gets(char str[], int length, FILE* stream);
void AddEdge(struct Graph* graph, struct ListDemo demo, unsigned src_ID, char tool[], double time_weight, double cost_weight, char domain[]);
void SwapMinNode(struct MinHeapNode** a, struct MinHeapNode** b);
void MinHeapify(struct MinHeap* heap, unsigned idx);
void DecreaseKey(struct MinHeap* heap, unsigned v, double dist);
void DijkstraByTime(const char* filename, struct Graph* graph, struct ListDemo* src_demo, struct ListDemo* dest_demo);
void DijkstraByCost(const char* filename, struct Graph* graph, struct ListDemo* src_demo, struct ListDemo* dest_demo);
void WriteConsole(struct Graph* graph, struct ListDemo* src_demo, struct ListDemo* dest_demo, double dist[], unsigned path[]);
void WriteHtmlFile(const char* filename, struct Graph* graph, struct ListDemo* src_demo, struct ListDemo* dest_demo, double dist[], unsigned path[]);
struct ListDemo NewListDemo(unsigned ID, char country[], char city[], double longitude, double latitude);
struct ListDemo* BinarySearch(const struct ListDemoArray* array, char key[], unsigned data[], unsigned offset);
struct ListDemoArray* CreateDemoArray(const char* filename, unsigned data[], unsigned* offset);
struct ListNode* NewListNode(struct ListDemo demo, char tool[], double time_weight, double cost_weight, char domain[]);
struct Graph* CreateGraph(unsigned V);
struct Graph* GenerateGraph(const char* filename, const struct ListDemoArray* array, unsigned data[], unsigned V, unsigned offset);
struct MinHeap* CreateMinHeap(unsigned capacity);
struct MinHeapNode* NewMinHeapNode(unsigned V, double dist);
struct MinHeapNode* ExtractMin(struct MinHeap* heap);

#endif // !SHORTESTPATH_H_