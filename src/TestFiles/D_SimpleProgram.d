import std.stdio;

int foo(int a, int b){
	if(a > 0)
		b+=1;
  else
    b+=2;

  b++;
  a++;  
	return 0;
}


void main(string[] args){

	int a = 0, soma = 0;
	for(int i = 0; i < args.length; i ++)
		soma+=i;

	int c = foo(a, soma);
  int d = foo(c, foo(a, soma));
	writeln(c);
}
