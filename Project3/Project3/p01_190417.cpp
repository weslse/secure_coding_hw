#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define BUFFER_SIZE 20
#define NUM_X 5


typedef struct MemoryStructure
{
	unsigned int size; // first bit is checkBit;
	MemoryStructure* forwardPtr;	
	MemoryStructure* backwardPtr;
	char* buff;
} MemoryStructure;


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
	// define buffer
	srand(time(NULL));
	char memory_test[BUFFER_SIZE];
	int i;

	// initialization
	for (i = 0; i < BUFFER_SIZE; i++) {
		memory_test[i] = ' ';
	}
	memory_test[BUFFER_SIZE - 1] = '\0';


	// print#1
	status_print(memory_test);

	// random memory setting
	for (i = 0; i < NUM_X; i++) {
		memory_test[rand() % BUFFER_SIZE] = 'X';
	}
	memory_test[BUFFER_SIZE - 1] = '\0';

	// print#2
	status_print(memory_test);


	// First-Fit allocation
	printf("\n --- FIRST FIT --- \n");
	printf("\nAfter Allocate AA\n");
	FIRST_FIT_ALLOCATION(memory_test, 'A', 2);
	status_print(memory_test);
	printf("\nAfter Allocate BBB\n");
	FIRST_FIT_ALLOCATION(memory_test, 'B', 3);
	status_print(memory_test);
	printf("\nAfter Allocate CCCC\n");
	FIRST_FIT_ALLOCATION(memory_test, 'C', 4);
	status_print(memory_test);
	// free memory
	printf("\nAfter free AA\n");
	ALLOC_FREE(memory_test, 'A', 2);
	status_print(memory_test);
	printf("\nAfter free BBB\n");
	ALLOC_FREE(memory_test, 'B', 3);
	status_print(memory_test);
	printf("\nAfter free CCCC\n");
	ALLOC_FREE(memory_test, 'C', 4);
	status_print(memory_test);

	// Best - Fit allocation
	printf("\n --- Best FIT --- \n");
	printf("\nAfter Allocate AA\n");
	BEST_FIT_ALLOCATION(memory_test, 'A', 2);
	status_print(memory_test);
	printf("\nAfter Allocate BBB\n");
	BEST_FIT_ALLOCATION(memory_test, 'B', 3);
	status_print(memory_test);
	printf("\nAfter Allocate CCCC\n");
	BEST_FIT_ALLOCATION(memory_test, 'C', 4);
	status_print(memory_test);
	// free memory
	printf("\nAfter free AA\n");
	ALLOC_FREE(memory_test, 'A', 2);
	status_print(memory_test);
	printf("\nAfter free BBB\n");
	ALLOC_FREE(memory_test, 'B', 3);
	status_print(memory_test);
	printf("\nAfter free CCCC\n");
	ALLOC_FREE(memory_test, 'C', 4);
	status_print(memory_test);

	// Worst - Fit allocation
	printf("\n --- Worst FIT --- \n");
	printf("\nAfter Allocate AA\n");
	WORST_FIT_ALLOCATION(memory_test, 'A', 2);
	status_print(memory_test);
	printf("\nAfter Allocate BBB\n");
	WORST_FIT_ALLOCATION(memory_test, 'B', 3);
	status_print(memory_test);
	printf("\nAfter Allocate CCCC\n");
	WORST_FIT_ALLOCATION(memory_test, 'C', 4);
	status_print(memory_test);
	// free memory
	printf("\nAfter free AA\n");
	ALLOC_FREE(memory_test, 'A', 2);
	status_print(memory_test);
	printf("\nAfter free BBB\n");
	ALLOC_FREE(memory_test, 'B', 3);
	status_print(memory_test);
	printf("\nAfter free CCCC\n");
	ALLOC_FREE(memory_test, 'C', 4);
	status_print(memory_test);

	return 0;
}
