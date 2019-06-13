
int bar(ulong Size){
	int soma = 0;
	if(Size > 0)
		for(int i = 0; i < Size; i++)
			soma+=i;

	return soma;
}

int foo(int a, lazy int bar ){
  if(a > 0){
    a = a + bar();
		a = bar();
	}
  return a;
}


void main(string[] args){

  int a = 1, soma = 0;
	ulong Size = args.length;
  int c = foo(a, bar(Size));
}
