#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
/* O(n^2) */

typedef struct vertex_s {
	int val;
	/* 0 - непройденная, 1 - пройденная */
	int sel;
} vertex;

vertex *solve(unsigned int **adj, int n, int x);
void route(unsigned int **adj, vertex *res, int n, int x);
int main() {
	/* Количество вершин и номер нужной для отсчета вершины */
	int n;
	int x;

	scanf("%d", &n);
	unsigned int **adj = (unsigned int**)calloc(n, sizeof(unsigned int *));
	for (int i = 0; i < n; i++) {
		adj[i] = (unsigned int*)calloc(n, sizeof(unsigned int));
	}
	/* В нашей матрице смежности 0 обозначает отсутствие пути из одной вершины в другую */
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			scanf("%d", &adj[i][j]);
			if (!adj[i][j])
				adj[i][j] = INT_MAX;
		}
	}

#ifdef _DEBUG
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			printf("%d\t", adj[i][j]);
		}
		printf("\n");
	}
#endif
	scanf("%d", &x);

	vertex *res = solve(adj, n, x);

	/* Вывод результата, расстояние от каждой вершины до данной с путем */
	route(adj, res, n, x);

	/* Освобождаем память */
	for (int i = 0; i < n; i++) {
		free(adj[i]);
	}
	free(adj);

	free(res);
}

vertex *solve(unsigned int **adj, int n, int x) {
	vertex *res = (vertex *)calloc(n, sizeof(vertex));

	/* Количество пройденных вершин */
	int size = 1;
	
	/* Массив с длинами ребер, из которых выбираем для каждой вершины */
	unsigned int *distance = (unsigned int*)calloc(n, sizeof(unsigned int));
	
	/* Начальная вершина */
	int v = x;
	
	/* Делаем начальную вершину выбранной */
	res[v].sel = 1;
	
	/* Смотрим, как выбранная вершина соотносится с другими, заполняем массив расстояний */
	for (int i = 0; i < n; i++) {
		distance[i] = adj[v][i];
		res[i].val = v;
	}

	/* Пока не переберем все вершины */
	while (size != n) {
		/* Минимальное расстояние до вершины, изначально - бесконечность */
		unsigned int min = INT_MAX;
		/* Номер очередной вершины */
		int k;
		for (int i = 0; i < n; i++) {
			/* Пропускаем пройденные вершины */
			if (res[i].sel == 1) {
				continue;
			}
			/* Ищем вершину, до которой ближе всего (с номером k) */
			if (min >= distance[i]) {
				min = distance[i];
				k = i;
			}
		}
		/* Отмечаем найденную вершину "пройденной" */
		res[k].sel = 1;
		/* Смотрим, изменится ли наш массив рёбер, если будем "шагать" из выбранной вершины */
		for (int i = 0; i < n; i++) {
			/* Игнорируем пройденные вершины */
			if (res[i].sel == 1) {
				continue;
			}
			/* Если из вершины k ближе до вершины i, то обновляем знaчение для i */
			if (distance[i] > min + adj[k][i]) {
				distance[i] = min + adj[k][i];
				res[i].val = k;
			}
		}

		size++;
	}

	free(distance);
	return res;
}

void route(unsigned int **adj, vertex *res, int n, int x) {
	for (int i = 0; i < n; i++) {
		/* Игнорируем петли */
		if (i == x) {
			continue;
		}
		int w = i;
		/* Суммарная длина */
		int sum = 0;
		/* Если есть прямой короткий путь, то выводим его */
		if (res[i].val == x) {
			sum = adj[x][i];
			printf("%d: %d-%d\n", i, i, x);
		}
		/* Иначе, последовательно идем по вершинам из res */
		else {
			printf("%d: %d-", i, i);
			while (res[w].val != x) {
				sum += adj[res[w].val][w];
				printf("%d-", res[w].val);
				w = res[w].val;
			}
			sum += adj[x][w];
			printf("%d\n", x);
		}
		printf("Расстояние: %d\n###\n", sum);
	}
}