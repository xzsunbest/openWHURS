#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<time.h>
#define TIMES 1000000
#pragma warning(disable:4996)
typedef struct Character {
	short num;
	char ch;
}Char;
void CreateRandomFile(const char* filename) {
	FILE *fp = fopen(filename, "wt");
	if (!fp)return;
	srand((unsigned)time(NULL));
	int times = TIMES;
	while (times--) {
		char ch = rand() % 26 + 65;
		short num = 1 + rand() % 100;
		char* tmp = (char *)calloc(num, sizeof(char));
		for (int i = 0; i < num; i++)tmp[i] = ch;
		fwrite(tmp, sizeof(char), num, fp);
		free(tmp); tmp = NULL;
	}
	fclose(fp);
}
void EncodeFile(const char* input_filename, const char* output_filename) {
	FILE *fin = fopen(input_filename, "rt");
	FILE *fout = fopen(output_filename, "wb");
	if (!fin || !fout)return;
	Char buffer;
	short tmp_num = 0; char tmp_ch1, tmp_ch2;
	tmp_ch1 = fgetc(fin);
	while (!feof(fin)) {
		tmp_ch2 = fgetc(fin);
		if (tmp_ch2 == tmp_ch1)++tmp_num;
		else {
			buffer.ch = tmp_ch1;
			buffer.num = tmp_num + 1;
			fwrite(&buffer, sizeof(Char), 1, fout);
			tmp_ch1 = tmp_ch2;
			tmp_num = 0;
		}
	}
	fclose(fin);
	fclose(fout);
}
void DecodeFile(const char* input_filename, const char* output_filename) {
	FILE *fin = fopen(input_filename, "rb");
	FILE *fout = fopen(output_filename, "wt");
	if (!fin || !fout)return;
	while (!feof(fin)) {
		Char buffer;
		fread(&buffer, sizeof(Char), 1, fin);
		char* tmp = (char *)calloc(buffer.num, sizeof(char));
		for (int i = 0; i < buffer.num; i++)tmp[i] = buffer.ch;
		fwrite(tmp, sizeof(char), buffer.num, fout);
		free(tmp); tmp = NULL;
		memset(&buffer, 0, sizeof(Char));
	}
	fclose(fin);
	fclose(fout);
}
int main() {
	CreateRandomFile("test.txt");
	EncodeFile("test.txt", "encode.bin");
	DecodeFile("encode.bin", "decode.txt");
	return 0;
}