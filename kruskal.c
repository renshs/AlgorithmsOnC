#include <stdio.h>
#include <stdlib.h>
/* Ребра */
typedef struct edge {
	int begin;
	int end;
	int len;
} Edge;

int count_edge(int **adj, int n);
Edge* create_edge_vector(int **adj, int n, int size);
void print_edges(int** adj, int n);
int** solve(Edge* vector, int size, int size_vertex);
int vector_compare(const void *a, const void *b);
void paint_vertices(Edge* vector, int size, int need_vertix, int saved_color, int *paint); 

int main() {
	/* n - количество вершин, size - количество ребер */

	int n;
	scanf("%d", &n);
	int **adj = (int**)calloc(n, sizeof(int*));
	for (int i = 0; i < n; i++) {
		adj[i] = (int*)calloc(n, sizeof(int));
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			scanf("%d", &adj[i][j]);
		}
	}
	/* Подсчет количества ребер в исходной матрице смежности */	
	int size = count_edge(adj, n);
	/* Создаем массив с ребрами */
	Edge *vector = create_edge_vector(adj, n, size);
	/* Сортируем ребра по длине, сначала самые короткие */
	qsort(vector, size, sizeof(Edge), vector_compare);

	int **res = solve(vector, size, n);
	/* Вывод списка имеющихся ребер по матрице смежности */
	print_edges(res, n);

	/* Освобождаем память */
	for (int i = 0; i < n; i++) {
		free(adj[i]);
	}
	free(adj);	
	for (int i = 0; i < n; i++) {
		free(res[i]);
	}
	free(res);
	free(vector);
}

int **solve(Edge* vector, int size, int v) {
	/* size - ребра, v - вершины */
	/* Результирующее остовное дерево в виде матрицы смежности */
	int** res = (int**)calloc(v, sizeof(int*));
	for (int i = 0; i < v; i++) {
		res[i] = (int*)calloc(v, sizeof(int));
	}
	/* Массив с окраской каждой вершины */
	int *paint = (int*)calloc(v, sizeof(int));
	/* Количество рассмотренных вершин */
	int n = 0;
	/* Текущий цвет */
	int color = 1;
	/* Сохраненный цвет для перекрашивания (объединение двух компонент) */
	int saved_color;
	/* Пока не раскрашены все вершины */
	while (n != size ) {
		/* Проверяем, соединяет ли очередное ребро вершины из уже имеющейся компоненты одного цвета (цвет 0 - отсутствие цвета) */
		if ((paint[vector[n].begin] == paint[vector[n].end]) && (paint[vector[n].begin] != 0 || paint[vector[n].end] != 0)) {
			n++;
			continue;
		}
		else {
			/* Создание новой компоненты связности и добавление результата в результирующую матрицу смежности */
			/* Обе вершины бесцветные */
			if (paint[vector[n].begin] == 0 && paint[vector[n].end] == 0) {
				paint[vector[n].begin] = color;
				paint[vector[n].end] = color;

				res[vector[n].begin][vector[n].end] = vector[n].len;
				/* Создаем новый цвет */
				color++;
			}
			/* Присоединение неокрашенной вершины к существующей компоненте связности */
			else if (paint[vector[n].begin] == 0) {
				paint[vector[n].begin] = paint[vector[n].end];
				
				res[vector[n].begin][vector[n].end] = vector[n].len;
			}
			else if (paint[vector[n].end] == 0) {
				paint[vector[n].end] = paint[vector[n].begin];
				
				res[vector[n].begin][vector[n].end] = vector[n].len;
			}
			/* Объединение компонент связности */
			else {
				/* Сохраняем цвет той компоненты, которую будем перекрашивать */
				saved_color = paint[vector[n].end];
				paint[vector[n].end] = paint[vector[n].begin];
				
				res[vector[n].begin][vector[n].end] = vector[n].len;
				
				/* Перекрашиваем вторую компоненту в цвет первой */
				paint_vertices(vector, size, vector[n].end, saved_color, paint);
			}
			n++;
		}
	}
	free(paint);
	return res;
}
void print_edges(int **adj, int n) {
	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			if (adj[i][j] != 0) {
				printf("%d-%d: %d\n", i, j, adj[i][j]);
			}
		}
	}
}

Edge* create_edge_vector(int **adj, int n, int size) {
	Edge* vector = (Edge*)calloc(size, sizeof(Edge));
	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			if (adj[i][j] != 0) {
				/* Заполняем с конца */
				size--;
				vector[size].begin = i;
				vector[size].end = j;
				vector[size].len = adj[i][j];
			}
		}
	}
	return vector;
}

int count_edge(int **adj, int n) {
	int res = 0;
	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			if (adj[i][j] != 0) {
				res++;
			}
		}
	}
	return res;
}

int vector_compare(const void *a, const void *b) {
	return ((const Edge *)a)->len - ((const Edge *)b)->len;
}

void paint_vertices(Edge* vector, int size, int need_vertix, int saved_color, int *paint) {
	for (int i = 0; i < size; i++) {
		/* Если текущее ребро выходит из нужной вершины */
		if (vector[i].begin == need_vertix) {
			/* Если другой конец ребра - нужный по цвету (старый), то красим в цвет исходной вершины */
			if (paint[vector[i].end] == saved_color) {
				paint[vector[i].end] = paint[need_vertix];
				/* Рекурсивно вызываем покраску для конца ребра */
				paint_vertices(vector, size, vector[i].end, saved_color, paint);
			}
		}
		/* Если текущее ребро заканчивается в нужной вершине */
		else if (vector[i].end == need_vertix) {
			/* Аналогично случаю выше, но вызываем для начала ребра */
			if (paint[vector[i].begin] == saved_color) {
				paint[vector[i].begin] = paint[need_vertix];				
				paint_vertices(vector, size, vector[i].begin, saved_color, paint);
			}
		}
	}
}