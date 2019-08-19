#include <stdio.h>

int foo(int a, int b){
//	if(a > 0)
	//	 b = b + 1;
//  else
  //   b = b - 1;
  
//  b++;
//  a++;  
  //Temos que pensar no caso de retornar b
      if(a > 0)
          return b;
//      return b;
  
	return 0;
}

int main(int argc, char **argv){

	int a;
	a = 0;
	int sum = 0;
	for (int i = 0; i < argc; i++) {
		sum += i;
	}
	/*int c =*/ foo(a, sum);
 // int d = foo(c, foo(a, sum));

//	printf("%d \n", c);
return 0;
}
