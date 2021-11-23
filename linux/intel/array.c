//这里首先对linux平台intel 64位架构的c++程序做一个简单的逆向学习，先贴出源代码，使用ubuntu 20.04进行编译

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct array {
	int size;
	int used;
	int *arr;
};

void dump(struct array *array)
{
	int idx;

	for (idx = 0; idx < array->used; idx++)
		printf("[%02d]: %08d\n", idx, array->arr[idx]);
}

void alloc(struct array *array)
{
	array->arr = (int *)malloc(array->size * sizeof(int));
}

int insert(struct array *array, int elem)
{
	int idx;
	if (array->used >= array->size)
		return -1;

	for (idx = 0; idx < array->used; idx++) {
		if (array->arr[idx] > elem)
			break;
	}

	if (idx < array->used)
		memmove(&array->arr[idx+1], &array->arr[idx],
			(array->used - idx) * sizeof(int));

	array->arr[idx] = elem;
	array->used++;
	return idx;
}

int delete(struct array *array, int idx)
{
	if (idx < 0 || idx >= array->used)
		return -1;

	memmove(&array->arr[idx], &array->arr[idx+1],
		(array->used - idx - 1) * sizeof(int));
	array->used--;
	return 0;
}

int search(struct array *array, int elem)
{
	int idx;

	for (idx = 0; idx < array->used; idx++) {
		if (array->arr[idx] == elem)
			return idx;
		if (array->arr[idx] > elem)
			return -1;
	}

	return -1;
}

int main()
{
	int idx;
	struct array ten_int = {10, 0, NULL};

	alloc(&ten_int);
	if (!ten_int.arr)
		return -1;
	insert(&ten_int, 1);
	insert(&ten_int, 3);
	insert(&ten_int, 2);
	printf("=== insert 1, 3, 2\n");
	dump(&ten_int);

	idx = search(&ten_int, 2);
	printf("2 is at position %d\n", idx);
	idx = search(&ten_int, 9);
	printf("9 is at position %d\n", idx);

	printf("=== delete [6] element \n");
	delete(&ten_int, 6);
	dump(&ten_int);
	printf("=== delete [0] element \n");
	delete(&ten_int, 0);
	dump(&ten_int);
	return 0;
}
//下面是反汇编后的伪代码,但是存在很多问题，ida并不能够完整的识别出正确的代码，需要我们手动的修正
int __cdecl main(int argc, const char **argv, const char **envp)
{
  __int64 v3; // rbp
  int result; // eax
  unsigned int v5; // eax
  unsigned int v6; // eax
  unsigned __int64 v7; // rdx
  unsigned __int64 v8; // rt1
  signed int size; // [rsp-28h] [rbp-28h]
  int used; // [rsp-24h] [rbp-24h]
  __int64 arr; // [rsp-20h] [rbp-20h]
  unsigned __int64 v12; // [rsp-10h] [rbp-10h]
  __int64 v13; // [rsp-8h] [rbp-8h]

  __asm { endbr64 }
  v13 = v3;
  v12 = __readfsqword(0x28u);                   // canary保护生成数据
  size = 10;
  used = 0;
  arr = 0LL;
  alloc((__int64)&v13, &size);
  if ( arr )
  {
    insert((__int64)&v13, (__int64)&size, 1);   // 这里的&size是结构体指针
    insert((__int64)&v13, (__int64)&size, 3);
    insert((__int64)&v13, (__int64)&size, 2);
    printf_weishibie();
    dump(&size);
    v5 = search(&size, 2LL);
    sub_10B0("2 is at position %d\n", v5);
    v6 = search(&size, 9LL);
    sub_10B0("9 is at position %d\n", v6);
    printf_weishibie();
    delete(&size, 6LL);
    dump(&size);
    printf_weishibie();
    argv = 0LL;
    delete(&size, 0LL);
    dump(&size);
    result = 0;
  }
  else
  {
    result = -1;
  }
  v8 = __readfsqword(0x28u);
  v7 = v8 ^ v12;
  if ( v8 != v12 )
    result = sub_10A0(&size, argv, v7);
  return result;
}

![image](https://user-images.githubusercontent.com/57678966/142989342-85fd6b13-6cf8-4ffb-a4a2-434176fe27cb.png)
