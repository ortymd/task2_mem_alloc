#include <func.h>
#include <stdio.h>
#include <string.h>

void test1()
{
	heap_malloc();

	char *a = m_malloc(10);
	char *b = m_malloc(10);
	if(a == NULL || b == NULL)
	{
		printf("FAILED\n");
	}
	else
	{
		sprintf(a, "%s", "123"); 
		sprintf(b, "%s", "123"); 

		if(strcmp(a, "123") == 0 && strcmp(b,"123") == 0)
			printf("SUCCESS\n");
		else
			printf("FAILED\n");
	}

	heap_free();
}

void test2()
{
	heap_malloc();

	char *a = m_malloc(10);
	char *tmp = a;
	m_free(a);

	char *b = m_malloc(10);
	if(tmp != b)
	{
		printf("FAILED\n");
	}
	else
	{
		printf("SUCCESS\n");
	}

	m_free(b);

	heap_free();
}

void test3()
{
	heap_malloc();
	char *a = m_malloc(2);
	char *b = m_malloc(1);
	m_free(a);

	char *c = m_malloc(3);
	if(c == a)
		printf("FAILED\n");
	else
		printf("SUCCESS\n");

	m_free(b);
	m_free(c);
	heap_free();

}

void test4()
{
	heap_malloc();
	char *a = m_malloc(2);
	char *b = m_malloc(3);
	char *c = m_malloc(3);
	m_free(a);
	m_free(c);

	char *d = m_malloc(3);
	if(d == c)
		printf("FAILED\n");
	else
		printf("SUCCESS\n");

	m_free(b);
	m_free(d);
	heap_free();
}

