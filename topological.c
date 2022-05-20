#include <stdio.h>
#include <stdlib.h>
/* O(n) - алгоритм Тарьяна */

typedef struct vertex_s {
	int val;
	/* 0 - белый, 1 - серый, 2 - черный */
	int color;
} vertex;

vertex *solve(int **adj, int n);
void dfs(int **adj, int size, int v, vertex *res, int *len);

int main(){
	int n;

	/* Считываем матрицу смежности */
	scanf("%d", &n);
	int **adj = (int **)calloc(n, sizeof(int *));
	for (int i = 0; i < n; i++) {
		adj[i] = (int *)calloc(n, sizeof(int));
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			scanf("%d", &adj[i][j]);
		}
	}

	vertex *res = solve(adj, n);

	/* Выводим только номера вершин */
	for (int i = 0; i < n; ++i) {
		printf("%d ", res[i].val);
	}

	/* Освобождаем память */
	for (int i = 0; i < n; i++) {
		free(adj[i]);
	}
	free(adj);
	free(res);
	
}

vertex *solve(int **adj, int n) {
	vertex *res = (vertex *)calloc(n, sizeof(vertex));
	
	/* len можно использовать в качестве индекса, начинающегося с нуля (в отличие от n) */
	int len = n - 1;
	for (int i = 0; i < n; i++) {
		/* Запускаем обход в глубину, если вершина - белая, i - номер текущей вершины */
		if (res[i].color == 0) {
			dfs(adj, n, i, res, &len);
		}
		/* Если нашлась серая вершина на данном шаге, то граф цикличен */
		else if (res[i].color == 1) {
			printf("Граф цикличен, решения не существует:\n");
			exit(0);
		}
	}

	return res;
}
void dfs(int **adj, int size, int v, vertex* res, int* len) {
#ifdef _DEBUG
	printf("Current vertex is %d with color %d\n", v, res[v].color);
#endif
	if (res[v].color == 0) {
		/* Красим белую вершину в серую */
		res[v].color = 1;
		/* вызываем поиск в глубину для каждой вершины, в которую можем дойти из данной, рекурсивно */
		for (int i = 0; i < size; i++) {
			if (adj[v][i] == 1) {
				dfs(adj, size, i, res, len);
			}
		}
		/* Заносим вершину в список для вывода */
		res[(*len)--].val = v;
#ifdef _DEBUG
		printf("Added vertex %d\n", v);
#endif
		/* Красим вершину в черный, она обработана */
		res[v].color = 2;

	}
	/* Если нашлась серая вершина - граф цикличен */
	else if (res[v].color == 1) {
		printf("Граф цикличен, решения не существует:\n");
		exit(0);
	}
}