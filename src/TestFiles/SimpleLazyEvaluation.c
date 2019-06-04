#include <stdio.h>

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
		return bar(argc);

	return 0;
}

int main(int argc, char **argv){
	int a;
	a = 1;
	int c = foo(a, argc, bar);
	
	printf("%d \n", c);
	return 0;
}
