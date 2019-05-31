#include"ShortestPath.h"
#include"Stack.h"

void s_gets(char str[], int length, FILE* stream) {
	fgets(str, length, stream);
	for (int i = length - 1;i >= 0;i--) {
		if (str[i] == '\n') {
			str[i] = '\0'; break;
		}
	}
}

struct ListDemo NewListDemo(unsigned ID, char country[], char city[], double longitude, double latitude) {
	struct ListDemo demo;
	memset(&demo, 0, sizeof(demo));
	demo.ID = ID;
	strncpy(demo.country, country, 64);
	strncpy(demo.city, city, 64);
	demo.longitude = longitude;
	demo.latitude = latitude;
	return demo;
}

int DemoCompare(const void* a, const void* b) {
	const struct ListDemo* a1 = (const struct ListDemo *)a;
	const struct ListDemo* b1 = (const struct ListDemo *)b;
	if (strcmp(a1->city, b1->city) < 0)return -1;
	else if (strcmp(a1->city, b1->city) > 0)return 1;
	else return (strcmp(a1->country, b1->country) < 0) ? -1 : 1;
}

struct ListNode* NewListNode(struct ListDemo demo, char tool[], double time_weight, double cost_weight, char domain[]) {
	struct ListNode* NewNode = (struct ListNode *)calloc(1, sizeof(struct ListNode));
	NewNode->demo = demo;
	NewNode->dest_ID = demo.ID;
	strncpy(NewNode->tool, tool, 8);
	NewNode->time_weight = time_weight;
	NewNode->cost_weight = cost_weight;
	strncpy(NewNode->domain, domain, 1024);
	NewNode->next = NULL;
	return NewNode;
}

struct Graph* CreateGraph(unsigned V) {
	struct Graph* graph = (struct Graph *)calloc(1, sizeof(struct Graph));
	graph->V = V;
	graph->array = (struct List *)calloc(V, sizeof(struct List));
	for (unsigned i = 0;i < V;i++)
		graph->array[i].head = NULL;
	return graph;
}

void AddEdge(struct Graph* graph, struct ListDemo demo, unsigned src_ID, char tool[], double time_weight, double cost_weight, char domain[]) {
	struct ListNode* NewNode = NewListNode(demo, tool, time_weight, cost_weight, domain);
	NewNode->next = graph->array[src_ID].head;
	graph->array[src_ID].head = NewNode;
}

struct MinHeapNode* NewMinHeapNode(unsigned V, double dist) {
	struct MinHeapNode* node = (struct MinHeapNode *)calloc(1, sizeof(struct MinHeapNode));
	node->V = V;
	node->dist = dist;
	return node;
}

struct MinHeap* CreateMinHeap(unsigned capacity) {
	struct MinHeap* heap = (struct MinHeap *)calloc(1, sizeof(struct MinHeap));
	heap->size = 0;
	heap->capacity = capacity;
	heap->pos = (unsigned *)calloc(capacity, sizeof(unsigned));
	heap->heap_array = (struct MinHeapNode **)calloc(capacity, sizeof(struct MinHeapNode *));
	return heap;
}

void SwapMinNode(struct MinHeapNode** a, struct MinHeapNode** b) {
	struct MinHeapNode* t = *a;*a = *b;*b = t;
}

void MinHeapify(struct MinHeap* heap, unsigned idx) {
	unsigned smallest, left, right;
	smallest = idx;
	left = 2 * idx + 1;
	right = 2 * idx + 2;
	if (left < heap->size
		&& heap->heap_array[left]->dist < heap->heap_array[smallest]->dist)
		smallest = left;
	if (right < heap->size
		&& heap->heap_array[right]->dist < heap->heap_array[smallest]->dist)
		smallest = right;
	if (smallest != idx) {
		struct MinHeapNode* smallestNode = heap->heap_array[smallest];
		struct MinHeapNode* idxNode = heap->heap_array[idx];
		heap->pos[smallestNode->V] = idx;
		heap->pos[idxNode->V] = smallest;
		SwapMinNode(&heap->heap_array[smallest], &heap->heap_array[idx]);
		MinHeapify(heap, smallest);
	}
}

bool isEmpty(struct MinHeap* heap) {
	return heap->size == 0;
}

struct MinHeapNode* ExtractMin(struct MinHeap* heap) {
	if (isEmpty(heap))return NULL;
	struct MinHeapNode* root = heap->heap_array[0];
	struct MinHeapNode* lastNode = heap->heap_array[heap->size - 1];
	heap->heap_array[0] = lastNode;
	heap->pos[root->V] = heap->size - 1;
	heap->pos[lastNode->V] = 0;
	--heap->size;
	MinHeapify(heap, 0);
	return root;
}

void DecreaseKey(struct MinHeap* heap, unsigned v, double dist) {
	unsigned i = heap->pos[v];
	heap->heap_array[i]->dist = dist;
	while (i && heap->heap_array[i]->dist < heap->heap_array[(i - 1) / 2]->dist) {
		heap->pos[heap->heap_array[i]->V] = (i - 1) / 2;
		heap->pos[heap->heap_array[(i - 1) / 2]->V] = i;
		SwapMinNode(&heap->heap_array[i], &heap->heap_array[(i - 1) / 2]);
		i = (i - 1) / 2;
	}
}

bool isInMinHeap(struct MinHeap* heap, unsigned V) {
	return (heap->pos[V] < heap->size);
}

void DijkstraByTime(const char* filename, struct Graph* graph, struct ListDemo* src_demo, struct ListDemo* dest_demo) {
	unsigned src = src_demo->ID;
	unsigned dest = dest_demo->ID;
	unsigned V = graph->V;
	double dist[MAXV];    
	unsigned path[MAXV];   
	memset(path, INT_MAX, sizeof(path));
	struct MinHeap* heap = CreateMinHeap(V);
	for (unsigned v = 0; v < V; v++) {
		dist[v] = (double)INT_MAX;
		heap->heap_array[v] = NewMinHeapNode(v, dist[v]);
		heap->pos[v] = v;
	}
	dist[src] = 0.0;
	heap->heap_array[src] = NewMinHeapNode(src, dist[src]);
	heap->pos[src] = src;
	DecreaseKey(heap, src, dist[src]);
	heap->size = V;
	while (!isEmpty(heap)) {
		struct MinHeapNode* node = ExtractMin(heap);
		unsigned u = node->V;
		struct ListNode* pCrawl = graph->array[u].head;
		while (pCrawl) {
			unsigned v = pCrawl->dest_ID;
			if (isInMinHeap(heap, v) && dist[u] != (double)INT_MAX
				&& pCrawl->time_weight + dist[u] < dist[v]) {
				dist[v] = dist[u] + pCrawl->time_weight;
				DecreaseKey(heap, v, dist[v]);
				path[v] = u;
			}
			pCrawl = pCrawl->next;
		}
	}
	WriteConsole(graph, src_demo, dest_demo, dist, path);
	WriteHtmlFile(filename, graph, src_demo, dest_demo, dist, path);
}

void DijkstraByCost(const char* filename, struct Graph* graph, struct ListDemo* src_demo, struct ListDemo* dest_demo) {
	unsigned src = src_demo->ID;
	unsigned dest = dest_demo->ID;
	unsigned V = graph->V;
	double dist[MAXV];       
	unsigned path[MAXV];      
	memset(path, INT_MAX, sizeof(path));
	struct MinHeap* heap = CreateMinHeap(V);
	for (unsigned v = 0; v < V; v++) {
		dist[v] = (double)INT_MAX;
		heap->heap_array[v] = NewMinHeapNode(v, dist[v]);
		heap->pos[v] = v;
	}
	dist[src] = 0.0;
	heap->heap_array[src] = NewMinHeapNode(src, dist[src]);
	heap->pos[src] = src;
	DecreaseKey(heap, src, dist[src]);
	heap->size = V;
	while (!isEmpty(heap)) {
		struct MinHeapNode* node = ExtractMin(heap);
		unsigned u = node->V;
		struct ListNode* pCrawl = graph->array[u].head;
		while (pCrawl) {
			unsigned v = pCrawl->dest_ID;
			if (isInMinHeap(heap, v) && dist[u] != (double)INT_MAX
				&& pCrawl->cost_weight + dist[u] < dist[v]) {
				dist[v] = dist[u] + pCrawl->cost_weight;
				DecreaseKey(heap, v, dist[v]);
				path[v] = u;
			}
			pCrawl = pCrawl->next;
		}
	}
	WriteConsole(graph, src_demo, dest_demo, dist, path);
	WriteHtmlFile(filename, graph, src_demo, dest_demo, dist, path);
}

struct ListDemoArray* CreateDemoArray(const char* filename,unsigned data[], unsigned* offset) {
	struct ListDemoArray* list_demo_array = (struct ListDemoArray *)calloc(1, sizeof(struct ListDemoArray));
	list_demo_array->demo_array = (struct ListDemo **)calloc(26, sizeof(struct ListDemo *));
	memset(data, 0, sizeof(data));
	unsigned V = 0;
	unsigned temp_ID = 0;
	char temp_country[64] = { '\0' };
	char temp_city[64] = { '\0' };
	double temp_longitude = 0.0;
	double temp_latitude = 0.0;
	unsigned count[26] = { 0 };
	FILE *fp = fopen(filename, "rt");
	if (!fp)exit(EXIT_FAILURE);
	fscanf(fp, "%[^,],%[^,],%lf,%lf\n", temp_country, temp_city, &temp_longitude, &temp_latitude);
	*offset = (temp_city[0] >= 65 && temp_city[0] <= 90) ? 65 : 97;
	++data[temp_city[0] - *offset];
	memset(temp_country, 0, sizeof(temp_country));
	memset(temp_city, 0, sizeof(temp_city));
	while (fscanf(fp, "%[^,],%[^,],%lf,%lf\n", temp_country, temp_city, &temp_longitude, &temp_latitude) == 4) {
		++data[temp_city[0] - *offset];
		memset(temp_country, 0, sizeof(temp_country));
		memset(temp_city, 0, sizeof(temp_city));
	}
	for (unsigned i = 0;i < 26;i++) {
		V += data[i];
		list_demo_array->demo_array[i] = (struct ListDemo *)calloc(data[i], sizeof(struct ListDemo));
	}
	list_demo_array->V = V;
	rewind(fp);
	while (fscanf(fp, "%[^,],%[^,],%lf,%lf\n", temp_country, temp_city, &temp_longitude, &temp_latitude) == 4) {
		struct ListDemo temp_demo = NewListDemo(temp_ID, temp_country, temp_city, temp_longitude, temp_latitude);
		list_demo_array->demo_array[temp_city[0] - *offset][count[temp_city[0] - *offset]] = temp_demo;
		++temp_ID;++count[temp_city[0] - *offset];
		memset(temp_country, 0, sizeof(temp_country));
		memset(temp_city, 0, sizeof(temp_city));
	}
	fclose(fp);
	for (unsigned i = 0;i < 26;i++)
		qsort(list_demo_array->demo_array[i], data[i], sizeof(struct ListDemo), DemoCompare);
	return list_demo_array;
}

struct ListDemo* BinarySearch(const struct ListDemoArray* array, char key[], unsigned data[], unsigned offset) {
	int i = key[0] - offset;
	if (i < 0 || i > 25)return NULL;
	unsigned size = data[i];
	unsigned lo = 0, hi = size - 1;
	int not_found = 0;
	while (lo <= hi) {
		unsigned mi = (lo + hi) >> 1;
		if (strcmp(key, array->demo_array[i][mi].city) < 0)hi = mi;
		else if (strcmp(key, array->demo_array[i][mi].city) > 0)lo = mi + 1;
		else return &array->demo_array[i][mi];
		if (lo == hi)++not_found;
		if (not_found >= 2)break;
	}
	return NULL;
}

struct Graph* GenerateGraph(const char* filename, const struct ListDemoArray* array, unsigned data[], unsigned V, unsigned offset) {
	struct Graph* graph = CreateGraph(V);
	FILE *fp = fopen(filename, "rt");
	if (!fp)exit(EXIT_FAILURE);
	char temp_src_city[64] = { '\0' };
	char temp_dest_city[64] = { '\0' };
	char temp_tool[8] = { '\0' };
	double temp_time_weight = 0.0;
	double temp_cost_weight = 0.0;
	char temp_domain[1024] = { '\0' };
	while (fscanf(fp, "%[^,],%[^,],%[^,],%lf,%lf,%[^\n]\n", 
		temp_src_city, temp_dest_city, temp_tool, &temp_time_weight, &temp_cost_weight, temp_domain) == 6) {
		struct ListDemo* src_city = BinarySearch(array, temp_src_city, data, offset);
		struct ListDemo* dest_city = BinarySearch(array, temp_dest_city, data, offset);
		AddEdge(graph, *dest_city, src_city->ID, temp_tool, temp_time_weight, temp_cost_weight, temp_domain);
		memset(temp_src_city, 0, sizeof(temp_src_city));
		memset(temp_dest_city, 0, sizeof(temp_dest_city));
		memset(temp_tool, 0, sizeof(temp_tool));
		memset(temp_domain, 0, sizeof(temp_domain));
	}
	fclose(fp);
	return graph;
}

void WriteConsole(struct Graph* graph, struct ListDemo* src_demo, struct ListDemo* dest_demo, double dist[], unsigned path[]) {
	unsigned V = graph->V;
	unsigned src = src_demo->ID;
	unsigned dest = dest_demo->ID;
	struct Stack st;
	CreateStack(&st, V);
	if (dist[dest] < (double)(INT_MAX - 1)) {
		unsigned j = dest;
		while (path[j] <= V) {
			push(&st, j);
			j = path[j];
		}
		push(&st, j);
		printf("%s=>%s, length:%g, path:\n%s\n", src_demo->city, dest_demo->city, dist[dest], src_demo->city);
		unsigned pre, temp;
		top(&st, &pre);
		pop(&st);
		while (!isempty(&st)) {
			top(&st, &temp);
			struct ListNode* pCrawl = graph->array[pre].head;
			while (1) {
				if (temp == pCrawl->dest_ID) {
					printf("%s\n", pCrawl->demo.city);
					break;
				}
				pCrawl = pCrawl->next;
			}
			pop(&st);
			pre = temp;
		}
	}
	else printf("No available routes.\n");
}

void WriteHtmlFile(const char* filename, struct Graph* graph, struct ListDemo* src_demo, struct ListDemo* dest_demo, double dist[], unsigned path[]) {
	FILE *fp = fopen(filename, "wt");
	unsigned count_marker = 0, count_string = 0;
	fprintf(fp, "<HTML><HEAD><TITLE>Shortest path from %s to %s</TITLE></HEAD>\n", src_demo->country, dest_demo->country);
	fprintf(fp, "<script type='text/javascript' src='https://maps.google.com/maps/api/js?sensor=false'></script>\n");
	fprintf(fp, "<script>function initialize() { var myOptions = { zoom: 3, center: new google.maps.LatLng(0, 0), mapTypeId: google.maps.MapTypeId.ROADMAP};\n");
	fprintf(fp, "var map=new google.maps.Map(document.getElementById('map'), myOptions);\n");
	unsigned V = graph->V;
	unsigned dest = dest_demo->ID;
	struct Stack st;
	CreateStack(&st, V);
	unsigned j = dest;
	while (path[j] <= V) {
		push(&st, j);
		j = path[j];
	}
	push(&st, j);
	unsigned temp;
	struct ListNode temp_node;
	top(&st, &temp);
	pop(&st);
	fprintf(fp, "var marker%u = new google.maps.Marker({ position: new google.maps.LatLng(%g, %g), map: map, title:'%s, %s'});\n", 
		count_marker++, src_demo->longitude, src_demo->latitude, src_demo->city, src_demo->country);
	top(&st, &temp);
	struct ListNode* pCrawl = graph->array[src_demo->ID].head;
	while (1) {
		if (temp == pCrawl->dest_ID) {
			fprintf(fp, "var marker%u = new google.maps.Marker({ position: new google.maps.LatLng(%g, %g), map: map, title:'%s, %s'});\n", 
				count_marker++, pCrawl->demo.longitude, pCrawl->demo.latitude, pCrawl->demo.city, pCrawl->demo.country);
			fprintf(fp, "var contentString%u = \"%s, %s--> %s, %s(%s - %g hours - $%g)\";\n", 
				count_string, src_demo->city, src_demo->country, pCrawl->demo.city, pCrawl->demo.country, pCrawl->tool, pCrawl->time_weight, pCrawl->cost_weight);
			fprintf(fp, "var path%u = new google.maps.Polyline({ path: [new google.maps.LatLng(%g, %g), new google.maps.LatLng(%g, %g)], strokeColor: '#0000FF', strokeOpacity: 1.0, strokeWeight: 2});\n", 
				count_string, src_demo->longitude, src_demo->latitude, pCrawl->demo.longitude, pCrawl->demo.latitude);
			fprintf(fp, "path%u.setMap(map); google.maps.event.addListener(path%u, 'click', function(event) { alert(contentString%u); });\n", 
				count_string, count_string, count_string);
			++count_string;
			temp_node = *pCrawl;
			break;
		}
		pCrawl = pCrawl->next;
	}
	pop(&st);
	while (!isempty(&st)) {
		top(&st, &temp);
		pop(&st);
		fprintf(fp, "var marker%u = new google.maps.Marker({ position: new google.maps.LatLng(%g, %g), map: map, title:'%s, %s'});\n",
			count_marker++, temp_node.demo.longitude, temp_node.demo.latitude, temp_node.demo.city, temp_node.demo.country);
		struct ListNode* pCrawl = graph->array[temp_node.demo.ID].head;
		while (1) {
			if (temp == pCrawl->dest_ID) {
				fprintf(fp, "var marker%u = new google.maps.Marker({ position: new google.maps.LatLng(%g, %g), map: map, title:'%s, %s'});\n",
					count_marker++, pCrawl->demo.longitude, pCrawl->demo.latitude, pCrawl->demo.city, pCrawl->demo.country);
				fprintf(fp, "var contentString%u = \"%s, %s--> %s, %s(%s - %g hours - $%g)\";\n",
					count_string, temp_node.demo.city, temp_node.demo.country, pCrawl->demo.city, pCrawl->demo.country, pCrawl->tool, pCrawl->time_weight, pCrawl->cost_weight);
				fprintf(fp, "var path%u = new google.maps.Polyline({ path: [new google.maps.LatLng(%g, %g), new google.maps.LatLng(%g, %g)], strokeColor: '#0000FF', strokeOpacity: 1.0, strokeWeight: 2});\n",
					count_string, temp_node.demo.longitude, temp_node.demo.latitude, pCrawl->demo.longitude, pCrawl->demo.latitude);
				fprintf(fp, "path%u.setMap(map); google.maps.event.addListener(path%u, 'click', function(event) { alert(contentString%u); });\n",
					count_string, count_string, count_string);
				++count_string;
				temp_node = *pCrawl;
				break;
			}
			pCrawl = pCrawl->next;
		}
	}
	fprintf(fp, "} google.maps.event.addDomListener(window, 'load', initialize); </script></HEAD><BODY><div id='map' style='width:100%%;height:100%%;'></div></BODY></HTML>\n");
	fclose(fp);
}