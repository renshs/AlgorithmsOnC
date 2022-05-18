#include <stdlib.h>
#include <assert.h>
typedef int Data;

typedef struct bhnode_s {
	/* Приоритет */
	int priority;
	/* Значение */
	Data val;
	Data to;

} bhnode;

/* Структура бинарной кучи */
typedef struct binary_heap_s {
	bhnode *body;
	/* Максимальны размер */
	int bodysize;
	/* Текущий размер */
	int numnodes;
} binary_heap;

/* Создание бинарной кучи */
binary_heap *binary_heap_new(int maxsize) {
	binary_heap *bh = malloc(sizeof(binary_heap));
	bh->body = calloc(sizeof(bhnode), maxsize + 1);
	bh->bodysize  = maxsize;
	bh->numnodes = 0;
	return bh;
}

void print_heap(binary_heap *bh) {
	printf("Numnodes: %d\n", bh->numnodes);
	for (int i = 1; i < bh->numnodes + 1; ++i) {
		printf("%d to %d (priority: %d)\n", bh->body[i].val, bh->body[i].to, bh->body[i].priority);
	}
}
void binary_heap_destroy(binary_heap *bh) {
	free(bh->body);
	free(bh);
}

void binary_heap_swap(binary_heap *bh, int a, int b) {
#ifdef DEBUG
	printf("Swapping %d and %d\n", a, b);
#endif	
	bhnode tmp = bh->body[a];
	bh->body[a] = bh->body[b];
	bh->body[b] = tmp;
}

bhnode binary_heap_fetch(binary_heap *bh) {
	assert(bh->numnodes > 0);
	return bh->body[1];
}

int binary_heap_insert(binary_heap *bh, bhnode node) {
	if (bh->numnodes > bh->bodysize) {
		return -1;
	}
	bh->body[++bh->numnodes] = node;
	for (size_t i = bh->numnodes;
		i> 1 && bh->body[i].priority < bh->body[i/2].priority;
		i /= 2) {
		binary_heap_swap(bh, i, i/2);
	}
	return 0;
}

void binary_heap_erase(binary_heap *bh) {
	assert(bh->numnodes > 0);
	bh->body[1] = bh->body[bh->numnodes--];
#ifdef DEBUG
	printf("Now root is %d (priority %d)\n", bh->body[1].val, bh->body[1].priority);
#endif
	size_t index = 1;
	for (;;) {
		size_t left = 2 * index;
		size_t right = left + 1;	
		size_t largest  = index;
		if ((int) left <= bh->numnodes && bh->body[left].priority < bh->body[index].priority)
			largest = left;
		if ((int) right <= bh->numnodes && bh->body[right].priority < bh->body[largest].priority)
			largest = right;
		if (largest == index) break;
		binary_heap_swap(bh, index, largest);
		index = largest;
#ifdef DEBUG
		printf("###\n");
		print_heap(bh);
#endif
	}
}

void binary_heap_recreate(binary_heap *bh) {
	size_t index = 1;
	for (;;) {
		size_t left = 2 * index;
		size_t right = left + 1;	
		size_t largest  = index;
		if ((int) left <= bh->numnodes && bh->body[left].priority < bh->body[index].priority)
			largest = left;
		if ((int) right <= bh->numnodes && bh->body[right].priority < bh->body[largest].priority)
			largest = right;
		if (largest == index) break;
		binary_heap_swap(bh, index, largest);
		index = largest;
#ifdef DEBUG
		printf("###\n");
		print_heap(bh);
#endif
	}
}