#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define BUFFER_SIZE 20
#define NUM_X 5


typedef struct MemoryStructure
{
	int size; // if size < 0 \\ unused;
	MemoryStructure* forwardPtr;
	MemoryStructure* backwardPtr;
	char* buff;
} MemStruct;

typedef struct {
	MemStruct* link;
} MemHead;

int checkTotalEmptySpaceSize(MemHead* memHead)
{
	int totalEmptySpaceSize = 0;
	for (MemStruct* ptr = memHead->link; ptr != NULL; ptr = ptr->backwardPtr)
	{
		if (ptr->size > 0)
			continue;
		totalEmptySpaceSize += ptr->size * -1;
	}
	return totalEmptySpaceSize;
}

MemStruct* makeMemSpace(char data, int data_size)
{
	MemStruct* newSpace = (MemStruct*)malloc(sizeof(MemStruct));
	newSpace->size = data_size;
	newSpace->buff = (char *)malloc(sizeof(data_size) + 1);
	for (int i = 0; i < data_size; ++i)
	{
		newSpace->buff[i] = data;
	}
	newSpace->buff[data_size] = '\0';
	return newSpace;
}

void linkNodes(MemHead* memHead, MemStruct* beforeNode, MemStruct* nowNode, MemStruct* newNode)
{
	newNode->forwardPtr = beforeNode;
	newNode->backwardPtr = nowNode;
	nowNode->forwardPtr = newNode;
	if (beforeNode == NULL) // if not before space is null
		memHead->link = newNode;
	else
		beforeNode->backwardPtr = newNode;
}


void status_print_stuct(MemHead* memHead)
{
	printf("\n//////////////////\n");
	puts("  Memory status");
	printf("//////////////////\n");

	int i = 0;
	char memState[BUFFER_SIZE + 1]; // 데이터의 분포를 시각적으로 보여줄 배열
	for (int j = 0; j < BUFFER_SIZE + 1; j++)
		memState[j] = ' '; // 빈공간으로 초기화

	int idx = 0;
	for (MemStruct* ptr = memHead->link; ptr != NULL; ptr = ptr->backwardPtr)
	{
		printf("Space %d\n", i); // 몇번째 공간인지 출력

		int memSize = ptr->size;
		if (memSize <= 0) // 해당 메모리 공간이 할당되어 있지 않는 경우
		{
			printf("It's not used (Size : %d)\n", -memSize);
			for (int j = idx; j < (idx + (-memSize)); j++)
				memState[j] = '.';
			idx += -memSize;
		}
		else // 해당 메모리 공간이 할당되어 있는 공간인 경우
		{
			printf("It is used space (Size : %d)  ", memSize);
			printf("Data : %s\n", ptr->buff);
			for (int j = idx; j < (idx + memSize); j++)
				memState[j] = ptr->buff[0];
			idx += memSize;
		}
		i++;
	}

	memState[BUFFER_SIZE] = '\0';
	printf("\n   Memory : [%s]\n", memState);
}

int first_fit_allocate_listVer(MemHead* memHead, char data, int data_size)
{
	fputs("\nnow allocating....", stdout);

	int totalEmptySpaceSize = checkTotalEmptySpaceSize(memHead);
	if (totalEmptySpaceSize < data_size)
	{
		fputs("no memory space for allocation", stdout);
		return -1;
	}

	int enableFlag = 0;
	for (MemStruct* ptr = memHead->link; ptr != NULL; ptr = ptr->backwardPtr)
	{
		if (ptr->size > 0)
			continue;

		int memSize = ptr->size * -1;
		if (memSize >= data_size)
		{
			enableFlag = 1;

			// allocate new space & set new space
			MemStruct* allocSpace = makeMemSpace(data, data_size);
			ptr->size -= -data_size;

			// Linking
			MemStruct* forwardNode = ptr->forwardPtr;
			linkNodes(memHead, forwardNode, ptr, allocSpace);
		}
	}

	if (!enableFlag) // fail allocating
	{
		fputs("memory space is too small\n", stdout);
		return -1;
	}

	// success
	fputs("success memory allocation!!\n", stdout);
	return 0;
}

int best_fit_allocate_listVer(MemHead* memHead, char data, int data_size)
{
	fputs("\nnow allocating....", stdout);

	int totalEmptySpaceSize = checkTotalEmptySpaceSize(memHead);
	if (totalEmptySpaceSize < data_size)
	{
		fputs("no memory space for allocation", stdout);
		return -1;
	}

	int emptyIdx = 0;
	int minEmptySize = BUFFER_SIZE;
	int i = 0;
	for (MemStruct* ptr = memHead->link; ptr != NULL; ptr = ptr->backwardPtr, i++)
	{
		if (ptr->size > 0)
			continue;

		int memSize = ptr->size * -1;
		if (memSize >= data_size)
		{
			if (memSize < minEmptySize)
			{
				minEmptySize = memSize;
				emptyIdx = i;
			}
		}

	}

	// 공간 위치를 찾았으니 생성 및 입력
	int idx = 0;
	for (MemStruct* ptr = memHead->link; ptr != NULL; ptr = ptr->backwardPtr, idx++)
	{
		if (!(idx == emptyIdx))
			continue;

		MemStruct* newNode = makeMemSpace(data, data_size);
		ptr->size -= data_size * -1;
		MemStruct* forwardNode = ptr->forwardPtr;
		linkNodes(memHead, forwardNode, ptr, newNode);
	}

	// success
	fputs("success memory allocation!!\n", stdout);
	return 0;
}

int worst_fit_allocate_listVer(MemHead* memHead, char data, int data_size)
{
	fputs("\nnow allocating....", stdout);

	int totalEmptySpaceSize = checkTotalEmptySpaceSize(memHead);
	if (totalEmptySpaceSize < data_size)
	{
		fputs("no memory space for allocation", stdout);
		return -1;
	}

	int emptyIdx = 0;
	int maxEmptySize = 0;
	int i = 0;
	for (MemStruct* ptr = memHead->link; ptr != NULL; ptr = ptr->backwardPtr, i++)
	{
		if (ptr->size > 0)
			continue;

		int memSize = ptr->size * -1;
		if (memSize < data_size)
			continue;

		if (memSize > maxEmptySize)
		{
			maxEmptySize = memSize;
			emptyIdx = i;
		}
	}

	// 공간 위치를 찾았으니 생성 및 입력
	int idx = 0;
	for (MemStruct* ptr = memHead->link; ptr != NULL; ptr = ptr->backwardPtr, idx++)
	{
		if (!(idx == emptyIdx))
			continue;

		MemStruct* newNode = makeMemSpace(data, data_size);
		ptr->size -= data_size * -1;
		MemStruct* forwardNode = ptr->forwardPtr;
		linkNodes(memHead, forwardNode, ptr, newNode);
	}
	// success
	fputs("success memory allocation!!\n", stdout);
	return 0;
}

// 같은 글자의 데이터가 연달아 있다고 가정하고 만든 free 함수 입니다.
// ex) AA..BBB..CCCC
//     FREE AA
//     ....BBB..CCCC
int free_listVer(MemHead* memHead, char data, char data_size)
{
	fputs("\nnow freeing.... ", stdout);

	int enableFlag = 0;
	for (MemStruct* ptr = memHead->link; ptr != NULL; ptr = ptr->backwardPtr)
	{
		if (ptr->size < 0)
			continue;

		if (ptr->size >= data_size && ptr->buff[0] == data)
		{
			enableFlag = 1;
			ptr->size *= -1;
			free(ptr->buff);
			break;
		}
	}

	// 첫 인덱스 출력
	if (enableFlag)
		printf("success memory free\n");
	else
		printf("There's no data\n");

	return 0;
}

// 빈 공간들을 공간 리스트의 가장 앞으로 모은다.
// ex) AA..BBB..CCCC
//     MERGE
//     ....AABBBCCCC
// 1. 해당 함수는 앞으로 메모리를 전부 이동시킨 후
// 2. 이동한 빈 메모리들을 합치는 방식으로 구현되어 있습니다.
void merge_empty_memories(MemHead* memHead)
{
	// 맨 앞으로 빈 메모리 공간들을 보내는 작업
	int isDone = 0;
	while (!isDone)
	{
		int isMovable = 0;
		int i = 0;
		for (MemStruct* ptr = memHead->link; ptr != NULL; ptr = ptr->backwardPtr, i++)
		{
			if (ptr->size > 0) {
				isDone = 1;
				isMovable = 1;
				continue;
			}
			if (i == 0)
				continue;
			if (!isMovable)
				continue;

			MemStruct* fwdPtr = ptr->forwardPtr;
			MemStruct* bwdPtr = ptr->backwardPtr;
			MemStruct* firstNode = memHead->link;
			ptr->forwardPtr = NULL;
			fwdPtr->backwardPtr = bwdPtr;
			if (bwdPtr == NULL); // 맨 마지막 노드인 경우
			else
				bwdPtr->forwardPtr = fwdPtr;
			firstNode->forwardPtr = ptr;
			ptr->backwardPtr = firstNode;
			memHead->link = ptr;
			isDone = 0;
			isMovable = 0;
			break;
		}
	}

	// 맨앞으로 보낸 빈 메모리 공간들을 합치는 작업
	// 이 새로운 공간에 모으고 기존 빈 공간들을 free시킨다.
	MemStruct* newBigEmptySpace = (MemStruct*)malloc(sizeof(MemStruct));
	newBigEmptySpace->buff = NULL;
	newBigEmptySpace->backwardPtr = NULL;
	newBigEmptySpace->forwardPtr = NULL;
	newBigEmptySpace->size = 0;

	MemStruct* nextPtr = NULL;
	for (MemStruct* ptr = memHead->link; ptr != NULL; ptr = nextPtr)
	{
		// 할당된 공간이 나타나면 합칠 빈 공간이 없다는 의미
		// 모두 합친 빈 공간을 헤드노드의 끝과 
		// 할당된 공간의 리스트 앞 순서로 연결 
		// ex) head->link = newNode;
		//     allocptr->forwardPtr = newNode;
		//     newNode->backwardPtr = allocPtr;
		if (ptr->size > 0)
		{
			memHead->link = newBigEmptySpace;
			newBigEmptySpace->backwardPtr = ptr;
			break;
		}

		// 다음 노드를 저장하고
		// 새로운 공간에 자신의 크기를 저장
		nextPtr = ptr->backwardPtr;
		newBigEmptySpace->size += ptr->size;

		// 혹시 buff에 gabage 데이터가 남아있다면
		// free 시켜준다.
		if (!ptr->buff) 
			free(ptr->buff);
		
		// 노드를 free
		free(ptr);
	}
}

////////////////////////////////////////////////////////////////
// Memory Display Functions
////////////////////////////////////////////////////////////////

/* --- Display Buffer --- */
void status_print(char * output) {
	puts("Memory status");
	printf("[[%s]]\n", output);
}


////////////////////////////////////////////////////////////////
// Memory Allocation
////////////////////////////////////////////////////////////////

/* --- First Fit --- */
int FIRST_FIT_ALLOCATION(char* memory_test_input, char data, char data_size)
{
	size_t countX = 0;
	for (int i = 0; i < strlen(memory_test_input); ++i)
		countX = memory_test_input[i] == 'X' ? ++countX : countX;

	size_t totalEmptySpaceSize = (BUFFER_SIZE - 1) - countX;
	if (totalEmptySpaceSize < data_size)
	{
		fputs("total memory space is too small\n", stdout);
		return -1;
	}

	int emptyStartIdx = 0;
	int emptySize = 0;
	int isStart = 0;
	int enableFlag = 0;
	for (int i = 0; i < BUFFER_SIZE - 1; ++i)
	{
		if (memory_test_input[i] == ' ')
		{
			// 주소 시작
			if (isStart == 0)
			{
				isStart = 1;
				emptyStartIdx = i;
			}
			emptySize++;
		}
		else
		{
			// 메모리 체크 시작했고 이미 할당된 인덱스에 도착
			if (isStart == 1)
			{
				if (emptySize >= data_size) // 만약 크기 딱맞거나 크다면
				{
					enableFlag = 1;
					break;
				}
				// 아니면 다시 시작
				isStart = 0;
				emptySize = 0;
			}
		}
	}

	if (!enableFlag)
	{
		fputs("memory space is too small\n", stdout);
		return -1;
	}

	// 공간을 찾았으니 입력
	for (int i = 0; i < data_size; ++i)
	{
		memory_test_input[i + emptyStartIdx] = data;
	}
	printf("first memory index : %d\n", emptyStartIdx);
	return 0;
}


/* --- Best Fit --- */
int BEST_FIT_ALLOCATION(char* memory_test_input, char data, char data_size)
{
	size_t countX = 0;
	for (int i = 0; i < strlen(memory_test_input); ++i)
		countX = memory_test_input[i] == 'X' ? ++countX : countX;

	size_t totalEmptySpaceSize = (BUFFER_SIZE - 1) - countX;
	if (totalEmptySpaceSize < data_size)
	{
		fputs("total memory space is too small\n", stdout);
		return -1;
	}

	int emptyIdx = 0;
	int emptyStartIdxArr[BUFFER_SIZE] = { 0 };
	int emptySize = 0;
	int emptySizeArr[BUFFER_SIZE] = { 0 };
	int isStart = 0;
	for (int i = 0; i < BUFFER_SIZE - 1; ++i)
	{
		if (memory_test_input[i] == ' ')
		{
			// 주소 시작
			if (isStart == 0)
			{
				isStart = 1;
				emptyIdx = i;
				emptyStartIdxArr[i] = i;
			}
			emptySizeArr[emptyIdx]++;
		}
		else
		{
			// 메모리 체크 시작했고 이미 할당된 인덱스에 도착
			if (isStart == 1)
				isStart = 0;
		}
	}

	// 데이터의 크기와 가장 크기가 비슷한 공간을 찾는다.
	int allocIdx = 0;
	int minDiff = BUFFER_SIZE;
	for (int i = 0; i < BUFFER_SIZE; ++i)
	{
		if (emptySizeArr[i] == 0)
			continue;
		int tmp = emptySizeArr[i] - data_size;
		if (tmp < 0)
			continue;
		if (minDiff > tmp)
		{
			allocIdx = i;
			minDiff = tmp;
		}
	}

	// 공간을 찾았으니 입력
	for (int i = 0; i < data_size; ++i)
	{
		memory_test_input[i + emptyStartIdxArr[allocIdx]] = data;
	}
	printf("first memory index : %d\n", emptyStartIdxArr[allocIdx]);

	return 0;
}


/* --- Worst Fit --- */
int WORST_FIT_ALLOCATION(char* memory_test_input, char data, char data_size)
{
	// 버퍼의 빈공간 탐색	
	size_t countBlank = 0;
	for (int i = 0; i < strlen(memory_test_input); ++i)
		countBlank = (memory_test_input[i] == ' ') ? ++countBlank : countBlank;

	size_t totalEmptySpaceSize = countBlank;
	if (totalEmptySpaceSize < data_size)
	{
		fputs("total memory space is too small\n", stdout);
		return -1;
	}

	int emptyIdx = 0;
	int emptyStartIdxArr[BUFFER_SIZE] = { 0 };
	int emptySize = 0;
	int emptySizeArr[BUFFER_SIZE] = { 0 };
	int isStart = 0;
	for (int i = 0; i < BUFFER_SIZE - 1; ++i)
	{
		if (memory_test_input[i] == ' ')
		{
			// 주소 시작
			if (isStart == 0)
			{
				isStart = 1;
				emptyIdx = i;
				emptyStartIdxArr[i] = i;
			}
			emptySizeArr[emptyIdx]++;
		}
		else
		{
			// 메모리 체크 시작했고 이미 할당된 인덱스에 도착
			if (isStart == 1)
				isStart = 0;
		}
	}

	// Array를 돌면서 데이터가 들어갈 
	// 가장 큰 공간을 찾는다.
	int allocIdx = -1;
	int maxDiff = -100;
	for (int i = 0; i < BUFFER_SIZE; ++i)
	{
		if (emptySizeArr[i] == 0)
			continue;
		int tmp = emptySizeArr[i] - data_size;
		if (tmp < 0)
			continue;
		if (maxDiff < tmp)
		{
			allocIdx = i;
			maxDiff = tmp;
		}
	}

	// 모두 스캔했지만 데이터가 들어갈 
	// 가장 큰 공간을 찾지 못했을 때
	if (allocIdx < 0)
	{
		printf("Can't allocate\n");
		return -2;
	}

	// 공간을 찾았으니 입력
	for (int i = 0; i < data_size; ++i)
	{
		memory_test_input[i + emptyStartIdxArr[allocIdx]] = data;
	}
	printf("first memory index : %d\n", emptyStartIdxArr[allocIdx]);

	return 0;
}


////////////////////////////////////////////////////////////////
// Memory Free
////////////////////////////////////////////////////////////////

/* --- Free Function --- */
int ALLOC_FREE(char* memory_test_input, char data_index, char data_size)
{
	int freeIdx = 0;
	int isStart = 0;
	int memorySize = 0;
	int enableFlag = 0;
	for (int i = 0; i < BUFFER_SIZE - 1; ++i)
	{
		if (memory_test_input[i] == data_index)
		{
			if (isStart == 0)
			{
				freeIdx = i;
				isStart = 1;
			}
			memorySize++;
		}

		if (memorySize >= data_size) {
			enableFlag = 1;
			break;
		}
	}

	// 빈칸할당
	for (int i = 0; i < data_size; ++i)
	{
		memory_test_input[i + freeIdx] = ' ';
	}

	// 첫 인덱스 출력
	if (enableFlag)
		printf("first memory index : %d\n", freeIdx);
	else
		printf("first memory index : %d\n", -1);

	return 0;
}


////////////////////////////////////////////////////////////////
// Main
////////////////////////////////////////////////////////////////

int main()
{
	//// define buffer
	//srand(time(NULL));
	//char memory_test[BUFFER_SIZE];
	//int i;

	//// initialization
	//for (i = 0; i < BUFFER_SIZE; i++) {
	//	memory_test[i] = ' ';
	//}
	//memory_test[BUFFER_SIZE - 1] = '\0';


	//// print#1
	//status_print(memory_test);

	//// random memory setting
	//for (i = 0; i < NUM_X; i++) {
	//	memory_test[rand() % BUFFER_SIZE] = 'X';
	//}
	//memory_test[BUFFER_SIZE - 1] = '\0';

	//// print#2
	//status_print(memory_test);


	//// First-Fit allocation
	//printf("\n --- FIRST FIT --- \n");
	//printf("\nAfter Allocate AA\n");
	//FIRST_FIT_ALLOCATION(memory_test, 'A', 2);
	//status_print(memory_test);
	//printf("\nAfter Allocate BBB\n");
	//FIRST_FIT_ALLOCATION(memory_test, 'B', 3);
	//status_print(memory_test);
	//printf("\nAfter Allocate CCCC\n");
	//FIRST_FIT_ALLOCATION(memory_test, 'C', 4);
	//status_print(memory_test);
	//// free memory
	//printf("\nAfter free AA\n");
	//ALLOC_FREE(memory_test, 'A', 2);
	//status_print(memory_test);
	//printf("\nAfter free BBB\n");
	//ALLOC_FREE(memory_test, 'B', 3);
	//status_print(memory_test);
	//printf("\nAfter free CCCC\n");
	//ALLOC_FREE(memory_test, 'C', 4);
	//status_print(memory_test);

	//// Best - Fit allocation
	//printf("\n --- Best FIT --- \n");
	//printf("\nAfter Allocate AA\n");
	//BEST_FIT_ALLOCATION(memory_test, 'A', 2);
	//status_print(memory_test);
	//printf("\nAfter Allocate BBB\n");
	//BEST_FIT_ALLOCATION(memory_test, 'B', 3);
	//status_print(memory_test);
	//printf("\nAfter Allocate CCCC\n");
	//BEST_FIT_ALLOCATION(memory_test, 'C', 4);
	//status_print(memory_test);
	//// free memory
	//printf("\nAfter free AA\n");
	//ALLOC_FREE(memory_test, 'A', 2);
	//status_print(memory_test);
	//printf("\nAfter free BBB\n");
	//ALLOC_FREE(memory_test, 'B', 3);
	//status_print(memory_test);
	//printf("\nAfter free CCCC\n");
	//ALLOC_FREE(memory_test, 'C', 4);
	//status_print(memory_test);

	//// Worst - Fit allocation
	//printf("\n --- Worst FIT --- \n");
	//printf("\nAfter Allocate AA\n");
	//WORST_FIT_ALLOCATION(memory_test, 'A', 2);
	//status_print(memory_test);
	//printf("\nAfter Allocate BBB\n");
	//WORST_FIT_ALLOCATION(memory_test, 'B', 3);
	//status_print(memory_test);
	//printf("\nAfter Allocate CCCC\n");
	//WORST_FIT_ALLOCATION(memory_test, 'C', 4);
	//status_print(memory_test);
	//// free memory
	//printf("\nAfter free AA\n");
	//ALLOC_FREE(memory_test, 'A', 2);
	//status_print(memory_test);
	//printf("\nAfter free BBB\n");
	//ALLOC_FREE(memory_test, 'B', 3);
	//status_print(memory_test);
	//printf("\nAfter free CCCC\n");
	//ALLOC_FREE(memory_test, 'C', 4);
	//status_print(memory_test);

	// List ver.
	MemHead* head = (MemHead*)malloc(sizeof(MemHead)); // 리스트의 헤드 노드
	MemStruct* bigSpace = (MemStruct*)malloc(sizeof(MemoryStructure));
	bigSpace->forwardPtr = NULL;
	bigSpace->backwardPtr = NULL;
	bigSpace->buff = NULL;
	bigSpace->size = BUFFER_SIZE; // 총 메모리 공간을 버퍼 사이즈(20)이라고 가정
	bigSpace->size *= -1;         // 사용하지 않으므로 음수로 설정

	// 초기 상태
	head->link = bigSpace;        // 헤드 노드를 메모리와 연결
	status_print_stuct(head);     // 출력


	printf("\n");

	// First fit
	printf("\n--- First Fit ---\n");
	first_fit_allocate_listVer(head, 'A', 2);
	status_print_stuct(head);
	first_fit_allocate_listVer(head, 'B', 3);
	status_print_stuct(head);
	first_fit_allocate_listVer(head, 'C', 4);
	status_print_stuct(head);

	free_listVer(head, 'A', 2);
	status_print_stuct(head);
	free_listVer(head, 'B', 3);
	status_print_stuct(head);

	// Best fit
	printf("\n--- Best Fit ---\n");
	best_fit_allocate_listVer(head, 'C', 4);
	status_print_stuct(head);
	best_fit_allocate_listVer(head, 'A', 2);
	status_print_stuct(head);

	free_listVer(head, 'C', 4);
	status_print_stuct(head);
	free_listVer(head, 'A', 2);
	status_print_stuct(head);

	// Worst fit
	printf("\n--- Worst Fit ---\n");
	worst_fit_allocate_listVer(head, 'D', 5);
	status_print_stuct(head);
	worst_fit_allocate_listVer(head, 'A', 2);
	status_print_stuct(head);

	// 메모리 합치기
	printf("\n--- Merge empty memories ---\n");
	merge_empty_memories(head);
	status_print_stuct(head);

	return 0;
}
