#include <stdio.h>
#include <stdlib.h>
/* O(n^3) */

void solve(int **adj, int n);
/* Поэлементный ИЛИ над двумя строками, заменяет строку a */
void vector_or(int **adj, int n, int a, int b);

int main() {
	int n;
	scanf("%d", &n);
	int **adj = (int **)calloc(n, sizeof(int*));
	for (int i = 0; i < n; i++) {
		adj[i] = (int*)calloc(n, sizeof(int));
	}
	
	/* Считываем матрицу смежности */
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			scanf("%d", &adj[i][j]);
		}
	}	

	solve(adj, n);

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			printf("%d ", adj[i][j]);
		}
		printf("\n");
	}

	/* Освобождаем память */
	for (int i = 0; i < n; i++) {
		free(adj[i]);
	}
	free(adj);

}

void solve(int **adj, int n) {
	int i, j;
	/* Идем по матрице, рассматриваем только ненулевые элементы и не на главной диагонали */
	for (i = 0; i < n; ++i) {
		for (j = 0; j < n; ++j) {
			if (adj[i][j] && (i != j)) {
				/* Поэлементно "ИЛИм" строки i и j, перезаписывая строку i */
				vector_or(adj, n, i, j);
			}
		}
	}
}

void vector_or(int **adj, int n, int a, int b) {
	for (int i = 0; i < n; ++i)
		adj[a][i] = adj[a][i] | adj[b][i];
}