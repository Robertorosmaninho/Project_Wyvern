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

int  foo2(int n, int *v){
  int s = 0;
  for(int i = 0; i < n ; i++)
    s += v[i];
  return s;
}

int main(int argc, char **argv){
	int a, b=4;
	a = 0;
  a += 20;
  a += atoi(argv[argc]);
	int c = foo(a, argc, bar);
  int v[5] = {0, 9, 2, 7, 134};

  foo2(5, &v);

	printf("%d \n", c);
	return 0;
}
