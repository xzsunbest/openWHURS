#include"Huffmancode.h"

int main() {
	char arr[CHARSIZE] = { '\0' };
	unsigned freq[CHARSIZE] = { 0 };
	struct Character codes[CHARSIZE] = { 0 };
	char* pArr = arr;
	unsigned* pFreq = freq;
	struct Character* pCh = codes;
	Statistics("message.txt", &pArr, &pFreq);
	//ReadInputFile("frequency.txt", &pArr, &pFreq);
	struct HuffNode* HuffTree = Huffman("codes.txt", arr, freq, CHARSIZE);
	ReadCodesFile("codes.txt", &pCh);
	Compression("message.txt", "encode.bin", codes, CHARSIZE);
	Decompression("encode.bin", "decode.txt", codes, &HuffTree);
	return 0;
}