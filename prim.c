#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "pqueue.c"
/* O(E*logV), E - количество вершин, V - число ребер, используем матрицу смежности и приоритетную очередь */

bhnode *solve(int **adj, int n);

int main() {
	int n;

	scanf("%d", &n);
	int **adj = (int**)calloc(n, sizeof(int*));
	for (int i = 0; i < n; i++) {
		adj[i] = (int*)calloc(n, sizeof(int));
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			scanf("%d", &adj[i][j]);
			if (!adj[i][j])
				adj[i][j] = INT_MAX;
		}
	}

	bhnode *res = solve(adj, n);

	for (int i = 1; i < n; i++) {
		printf("%d-%d: %d\n", res[i].val, res[i].to, res[i].priority);
	}

	/* Освобождаем память */
	for (int i = 0; i < n; i++) {
		free(adj[i]);
	}
	free(adj);
	free(res);
}

bhnode *solve(int **adj, int n) {
	/* Приоритетная очередь */
	binary_heap *bh = binary_heap_new(n);
	
	int i;

	/* Результирующий массив */
	bhnode *res = (bhnode *)calloc(n, sizeof(bhnode));
	
	/* Начинаем с нулевой вершины */

	/* Количество выбранных вершин */
	int size = 1;

	/* Добавляем в очередь все вершины, кроме нулевой, с которой начали */	
	for (i = 1; i < n; ++i) {
		binary_heap_insert(bh, (bhnode) {adj[0][i], i, 0});
	}

	/* Пока очередь вершин не пуста */
	while (bh->numnodes) {
		bhnode current = binary_heap_fetch(bh);
		binary_heap_erase(bh);
#ifdef _DEBUG
		printf("Extracted #%d to %d (%d)#\n", current.val, current.to, current.priority);	
#endif
		res[size++] = current;

		/* Обновляем приоритеты в очереди */
		for (i = 1; i <= bh->numnodes; ++i) {
			if (adj[bh->body[i].val][current.val] < bh->body[i].priority) {
#ifdef _DEBUG
				printf("Updated priority on %d (prev - to %d (%d)) to %d (%d)\n", bh->body[i].val, bh->body[i].to, bh->body[i].priority, current.val, adj[bh->body[i].val][current.val]);
#endif
				bh->body[i].to = current.val;
				bh->body[i].priority = adj[bh->body[i].val][current.val];
			}
		}
		/* Сортируем заново, с новыми приоритетами */
		binary_heap_recreate(bh);
	}
	binary_heap_destroy(bh);
	return res;
}