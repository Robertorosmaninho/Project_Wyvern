#include <stdio.h>

void func(int a, int sum){
  if(a > 0)
    printf("%d\n", sum);
}

int bar(int argc){
	int sum = 0;
	if (argc > 0){
		for (int i = 0; i < argc; i++) {
    	sum += i;
  	}
	}
	return sum;
}

int foo(int a, int argc,int (*bar)(int)){
	if(a > 0)
		a = a + bar(argc);

	return a;
}

int main(int argc, char **argv){
	int a;
	a = 0;
	int c = foo(a, argc, bar);
	
	printf("%d \n", c);
	return 0;
}
