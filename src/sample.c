
#include <stdio.h>
#include <string.h>
int main()
{
	char str[20];
	str[0] = 'a';
	str[1] = 'b';
	str[2] = '\0';
	printf("%d\n", (int)strlen(str));
	printf("%ld\n", sizeof(str));
}