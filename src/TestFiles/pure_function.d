import std.stdio;

struct S { double x; }
pure ulong foo(immutable(int)[] arr, int num, S val)
{
    //arr[num] = 1; // compile error
    num = 2;        // has no side effect to the caller side
    val.x = 3.14;   // ditto
    return arr.length;
}

void main(){
  S val;
  val.x = 2.5;
  ulong a = foo([0, 1, 2], 3, val);
}
