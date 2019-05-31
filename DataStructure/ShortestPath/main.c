#include"ShortestPath.h"

int main() {
	unsigned offset, data[26] = { 0 };
	char src_city[64] = { '\0' }, dest_city[64] = { '\0' }, choice;
	struct ListDemoArray* array = CreateDemoArray("cities_big.csv", data, &offset);
	struct Graph* graph = GenerateGraph("routes_big.csv", array, data, array->V, offset);
	printf("Enter the name of source city and destination city:\n");
	s_gets(src_city, 63, stdin);
	s_gets(dest_city, 63, stdin);
	struct ListDemo* src_demo = BinarySearch(array, src_city, data, offset);
	struct ListDemo* dest_demo = BinarySearch(array, dest_city, data, offset);
	while (!src_demo) {
		printf("The source city name you entered doesn't exist.Enter again!\n");
		s_gets(src_city, 63, stdin);
		src_demo = BinarySearch(array, src_city, data, offset);
	}
	while (!dest_demo) {
		printf("The destination city name you entered doesn't exist.Enter again!\n");
		s_gets(dest_city, 63, stdin);
		dest_demo = BinarySearch(array, dest_city, data, offset);
	}
	printf("1.Time\t2.Cost\n");
	scanf("%c", &choice);
	while (!strchr("12", choice)) {
		printf("Wrong input.Enter again!\n");
		scanf("%c", &choice);
	}
	choice == '1' ? DijkstraByTime("sample_time.html", graph, src_demo, dest_demo) :
		DijkstraByCost("sample_cost.html", graph, src_demo, dest_demo);
	return 0;
}

//Example input and output:
//Enter the name of source city and destination city:
//mywxzguaeagnfmiwiix
//ufqowpjpvlwobjfmfqz
//1.Time    2.Cost
//1
//mywxzguaeagnfmiwiix=>ufqowpjpvlwobjfmfqz, length:32, path:
//mywxzguaeagnfmiwiix
//lruryimjgmohxoofxwb
//ufqowpjpvlwobjfmfqz