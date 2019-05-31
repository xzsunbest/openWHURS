#include"Huffmancode.h"

void SwapNode(struct HuffNode** a, struct HuffNode** b) {
	struct HuffNode* temp = *a;*a = *b;*b = temp;
}

struct HuffNode* NewNode(char data, unsigned freq) {
	struct HuffNode* temp = (struct HuffNode*)calloc(1, sizeof(struct HuffNode));
	temp->data = data;
	temp->freq = freq;
	temp->lc = temp->rc = NULL;
	return temp;
}

struct HuffCode* InitializeCode(unsigned capacity) {
	struct HuffCode* temp = (struct HuffCode*)calloc(1, sizeof(struct HuffCode));
	temp->size = 0;
	temp->capacity = capacity;
	temp->arr = (struct HuffNode**)calloc(temp->capacity, sizeof(struct HuffNode*));
	return temp;
}

void HuffSort(struct HuffCode* code) {
	bool sorted = false;
	unsigned size = code->size;
	while (!sorted) {
		sorted = true;
		for (unsigned i = 1;i < size;i++) {
			if (code->arr[i - 1]->freq < code->arr[i]->freq) {
				SwapNode(&code->arr[i - 1], &code->arr[i]);
				sorted = false;
			}
		}
		size--;
	}
}

struct HuffNode* ExtractMinNode(struct HuffCode* code) {
	HuffSort(code);
	struct HuffNode* temp = code->arr[code->size - 1];
	--code->size;
	return temp;
}

void InsertCode(struct HuffCode* code, struct HuffNode* node) {
	++code->size;
	code->arr[code->size - 1] = node;
	HuffSort(code);
}

struct HuffCode* BuildCode(char data[], unsigned freq[], unsigned size) {
	struct HuffCode* code = InitializeCode(size);
	unsigned j = 0;
	for (unsigned i = 0;i < size;i++)
		if (data[i] != '\0')code->arr[j++] = NewNode(data[i], freq[i]);
	code->size = j;
	HuffSort(code);
	return code;
}

struct HuffNode* BuildTree(char data[], unsigned freq[], unsigned size) {
	struct HuffNode *lc, *rc, *top;
	struct HuffCode *code = BuildCode(data, freq, size);
	while (code->size != 1) {
		lc = ExtractMinNode(code);
		rc = ExtractMinNode(code);
		top = NewNode('^', lc->freq + rc->freq);
		top->lc = lc;
		top->rc = rc;
		InsertCode(code, top);
	}
	return ExtractMinNode(code);
}

void PrintCode(FILE **fp, struct HuffNode* root, int arr[], int top) {
	FILE *fout = *fp;
	if (root->lc) {
		arr[top] = 0;
		PrintCode(&fout, root->lc, arr, top + 1);
	}
	if (root->rc) {
		arr[top] = 1;
		PrintCode(&fout, root->rc, arr, top + 1);
	}
	if ((!root->lc) && (!root->rc)) {
		fprintf(fout, "%c: ", root->data);
		for (int i = 0;i < top;i++)
			fprintf(fout, "%d", arr[i]);
		fputc('\n', fout);
	}
}

struct HuffNode* Huffman(const char* filename, char data[], unsigned freq[], unsigned size) {
	FILE *fp = fopen(filename, "wt");
	OUTPUT(fp, filename);
	struct HuffNode* root = BuildTree(data, freq, size);
	int arr[MAX_TREE_HT], top = 0;
	PrintCode(&fp, root, arr, top);
	fclose(fp);
	return root;
}

void Statistics(const char* filename, char** data, unsigned** freq) {
	FILE *fp = fopen(filename, "rt");
	INPUT(fp, filename);
	char* tmp_data = *data;
	unsigned* tmp_freq = *freq;
	while (!feof(fp)) {
		char ch = fgetc(fp);
		if (ch >= 32)++tmp_freq[ch - 32];
	}
	for (int i = 0;i < 128;i++)
		if (tmp_freq[i] > 0)tmp_data[i] = i + 32;
	fclose(fp);
}

void ReadInputFile(const char* filename, char** data, unsigned** freq) {
	FILE *fp = fopen(filename, "rt");
	INPUT(fp, filename);
	char* tmp_data = *data;
	unsigned* tmp_freq = *freq;
	for(int i = 0;!feof(fp);i++)
		fscanf(fp, "%c %u\n", &tmp_data[i], &tmp_freq[i]);
	fclose(fp);
}

void ReadCodesFile(const char* filename, struct Character** ch) {
	FILE *fp = fopen(filename, "rt");
	INPUT(fp, filename);
	struct Character* tmp_ch = *ch;
	char temp[MAXCHAR + 5] = { '\0' };
	for (int i = 0;!feof(fp);i++) {
		fgets(temp, MAXCHAR, fp);
		if (strlen(temp) == 0)break;
		tmp_ch[i].ch = temp[0];
		strncpy(tmp_ch[i].code, temp + 3, strlen(temp) - 4);
		memset(temp, 0, sizeof(temp));
	}
	fclose(fp);
}

void Compression(const char* input_filename, const char* output_filename, struct Character* ch, int length) {
	FILE *fin = fopen(input_filename, "rt");
	FILE *fout = fopen(output_filename, "wb");
	INPUT(fin, input_filename);
	OUTPUT(fout, output_filename);
	struct stat buf;
	stat(input_filename, &buf);
	fwrite(&buf.st_size, sizeof(long), 1, fout);              
	int bin;
	char tmp;
	char str[MAX_TREE_HT] = { '\0' };
	char tstr[8] = { '\0' };
	while (!feof(fin)) {
		tmp = fgetc(fin);
		for (int i = 0;i < length;i++) {
			if (ch[i].ch == tmp) {
				strcat(str, ch[i].code);
				break;
			}
		}
		if (strlen(str) >= 8) {                    
			strncpy(tstr, str, 8);
			bin = strtol(tstr, NULL, 2);
			fwrite(&bin, 1, 1, fout);
			bin = 0;
			memset(tstr, 0, 8);
			unsigned len = strlen(str);
			for (unsigned i = 0;i < len;i++)
				str[i] = str[i + 8];
		}
	}
	strncpy(tstr, str, 8);
	for (int i = 0;i < 8;i++)
		if (tstr[i] == '\0')tstr[i] = '0';
	bin = strtol(tstr, NULL, 2);
	fwrite(&bin, 1, 1, fout);
	fclose(fin);
	fclose(fout);
}

void Decompression(const char* input_filename, const char* output_filename, struct Character* ch, struct HuffNode** tree) {
	FILE *fin = fopen(input_filename, "rb");
	FILE *fout = fopen(output_filename,"wt");
	INPUT(fin, input_filename);
	OUTPUT(fout, output_filename);
	long file_size;
	fread(&file_size, sizeof(long), 1, fin);
	struct HuffNode* point = *tree;
	char str[2*CHARSIZE] = { '\0' }, tmp = '\0';
	char tstr[2*CHARSIZE] = { '\0' };
	int j = 0;
	while (1) {
		if (strlen(str) <= MAXCHAR) {
			fread(&tmp, 1, 1, fin);
			for (int i = 7;i >= 0;i--) {
				if (tmp & 1)tstr[i] = '1';
				else tstr[i] = '0';
				tmp >>= 1;
			}
			strcat(str, tstr);
		}
		for (int i = 0;i < MAXCHAR && str[i] != '\0';i++) {        
			if (point->data == '^') {
				if (str[i] == '0')point = point->lc;
				else if (str[i] == '1')point = point->rc;
			}
			else {
				fputc(point->data, fout);
				point = *tree;
				--file_size;
				if (file_size == 0) {
					fclose(fin);
					fclose(fout);
					return;
				}
				for (;j < i;j++) str[j] = '\0';
				j = i--;
			}
		}
		point = *tree;
		for (int i = 0;i < 2*CHARSIZE - j;i++)str[i] = str[i + j];
		memset(tstr, 0, sizeof(tstr));
		j = 0;
	}
}