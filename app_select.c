#include <stdio.h>
#include <stdlib.h>
/* O(n*log(n) + n), сортировка плюс проход по массиву заявок */

/* Структура для хранения заявки */
typedef struct application {
	int begin;
	int end;
} app;

int vector_compare(const void *a, const void *b);
int solve(app *vector, unsigned int N);

int main() {
	/* Число заявок */
	unsigned int N;
	scanf("%d", &N);
	app* vector = (app*)calloc(N, sizeof(app));

	/* Считываем информацию о заявках, начало-конец */
	for (unsigned int i = 0; i < N; i++) {
		scanf("%d %d", &vector[i].begin, &vector[i].end);
	}

	/* Сортируем заявки по времени окончания */
	qsort(vector, N, sizeof(app), vector_compare);
	
	int result = solve(vector, N); 
	printf("Количество выполненных заявок:%d\n", result);

	free(vector);
}

int vector_compare(const void *a, const void *b) {
	return ((const app *)a)->end - ((const app *)b)->end;
}

int solve(app *vector, unsigned int N) {
	/* Сохраняем начальное значение N */
	unsigned int result = N;
	/* Конец очередной заявки */
	int end = vector[0].end;
	for (unsigned int i = 1; i < N; i++) {
		if (end > vector[i].begin) {
			/* 	Удаляем заявки, время начала которых раньше времени конца
		 		Изменяем счетчик заявок */
			(result)--;
		}
		else {
			/* Иначе, обновляем значение очередного конца */
			end = vector[i].end;
		}
	}
	
	return result;
}