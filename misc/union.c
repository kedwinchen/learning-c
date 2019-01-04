#include <string.h>
#include <unistd.h>
#include <stdio.h>

union some_union {
	// char p_str[3];
	char p_str[128];
	float f;
	int i;
	double d;
};

void printdata(union some_union s)
{
	printf("s.p_str is: %s\n", (const char *)&s.p_str);
	printf("s.f is: %f\n", s.f);
	printf("s.i is: %d\n", s.i);
	printf("s.d is: %f\n", s.d);
}

int main(void)
{
	const char *q = "ab";
	union some_union s;
	// s.p_str = {'a','b','\0'};
	strncpy(s.p_str, q, sizeof(q));
	printdata(s);
	s.f = 124.127231;
	printf("formatted as %%f10.4 is: %10.4f\n", s.f);
	printf("sizeof(float) is %lu\n", sizeof(float));
	printf("sizeof(p_str) is %lu\n", sizeof(s.p_str));
	printf("p_str is: %s\n", (const char *)&s.p_str);
	return 0;
}
