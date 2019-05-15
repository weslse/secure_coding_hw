#include <stdio.h>
#include <stdlib.h>
#include <string>

typedef struct point
{
	int x;
	struct point* before;
	struct point* after;
} Point;

void printList(Point* list)
{
	for (Point* p = list; p != NULL; p = p->after)
	{
		printf("[%d]", p->x);
		if (p->after == NULL)
			break;
		printf(" -> ");
	}
	printf("\n");
}

void addList(Point* list, int target)
{
	Point* newNode = (Point*)malloc(sizeof(Point));
	newNode->x = target;
	newNode->before = NULL;
	newNode->after = NULL;

	for (Point* p = list; p != NULL; p = p->after)
	{
		if (p->x == target)
		{
			printf("이미 추가됨\n");
			free(newNode);
			return;
		}

		if (p->after->x > target)
		{
			Point* next = p->after;
			newNode->before = p;
			newNode->after = next;
			p->after = newNode;
			next->before = newNode;
			return;
		}
	}
}

void deleteList(Point* list, int target)
{
	Point* b = NULL;
	Point* a = NULL;

	for (Point* p = list; p != NULL; p = a)
	{
		a = p->after;
		// target이면
		if (p->x == target)
		{
			b = p->before;
			b->after = a;
			a->before = b;
			free(p);
			return;
		}
	}
	printf("없는 좌표입니다\n");
}

int main()
{
	Point* point1 = (Point*)malloc(sizeof(Point));
	point1->x = 1;
	Point* point3 = (Point*)malloc(sizeof(Point));
	point3->x = 3;
	Point* point10 = (Point*)malloc(sizeof(Point));
	point10->x = 10;

	point1->before = NULL;
	point1->after = point3;
	point3->before = point1;
	point3->after = point10;
	point10->before = point3;
	point10->after = NULL;

	// 1 - 3 - 10 출력
	printList(point1);

	// 3 free
	deleteList(point1, 3);
	printList(point1);

	// 키보드 입력
	char inst[4] = { 0 };
	int input = 0;
	printf("\n명령어 ADD(추가), DEL(삭제)가 있습니다.\n");
	printf("--- 초기 상태 ---\n");
	printList(point1);
	while (1)
	{
		printf("입력 >> ");
		scanf(" %s %d", inst, &input);
		if (strcmp(inst, "ADD") == 0)
		{
			addList(point1, input);
			printList(point1);
		}
		else if (strcmp(inst, "DEL") == 0)
		{
			deleteList(point1, input);
			printList(point1);
		}
		else
			printf("입력 오류 다시 입력하세요.\n");
	}
}