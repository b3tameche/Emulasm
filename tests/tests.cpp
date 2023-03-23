#include <iostream>

using namespace std;

/******************************TEST#1******************************/
int n1=10;
int compute() {
  int a=1;
  int b=0;
  int c=0;
  while (a<=n1) {
    b=a*a;
    c+=b;
    a++;
  }
  return c;
}

int func() {
  return compute();
}

/******************************TEST#2******************************/
int a=3;
int b=3;
int c=4;
int pow() {
  int d = 1; 
  while (d<c) {
    a*=b;
    d++;
  }
  return a;
}

/******************************TEST#3******************************/
int n2=57;
int op() {
  int count = 0;
  while (n2>1) {
    count++;
    if (n2%2 == 0) {
      n2/=2;
    } else {
      n2--;
    }
  }
  return count;
}

/******************************TEST#4******************************/
int fib(int n) {
  if (n<=1) {
    return n;
  }
  int f=0;
  int s=1;
  int a=2;
  int ans;
  while (a<=n) {
    ans=f+s;
    f=s;
    s=ans;
    a++;
  }
  return ans;
}

/******************************TEST#5******************************/
int basic() {
  int a=5;
  int b=2;
  int c;
  if (a<=b) {
    c=(b-a)*2;
  } else {
    c=(a-b)*2;
  }
  return c;
}

/******************************TEST#6******************************/
int fib_rec(int n) {
  if(n<=1) {
    return n;
  }
  int a=fib_rec(n-1);
  int b=fib_rec(n-2);
  return a+b;
}

/******************************TEST#7******************************/
int SQRMLT(int a, int b, int c) {
  int d=a*a;
  int e=b*b;
  int f=c*c;
  e=d*e;
  f=e*f;
  return f;
}

/******************************TEST#8******************************/
int ALU(){
  int a;
  int b = 5;
  a = 6;
  int ans = a * b;
  ans = a / b;
  ans = b - a;
  ans = a + b;
  return ans;
}

/******************************TEST#9******************************/
int FORCYCLE(){
  int a = 0;
  for(int i = 0; i < 4; i++){
    a++;
  }
  return a;
}

/******************************TEST#10******************************/
int IFCLAUSE(){
  int a = 2;
  int b = 3;
  if(a > b){
    int c = 3;
    return c;
  }
  if(a < b){
    int c = 4;
    return c;
  }
  return 5;
}

/******************************TEST#11******************************/
int LOAD(){
  int a = 5;
  int* b = &a;
  int ans = *b;
  return ans;
}

/******************************TEST#12******************************/
int FIBO(int a){
  if(a <= 1) return a;
  int x = FIBO(a - 1);
  int y = FIBO(a - 2);
  return  x + y;
}

int MAIN(){
  int a = 3;
  return FIBO(a);
}

/******************************TEST#13******************************/
int STORE(){
  int a = 5;
  int* b = &a;
  *b = 7;
  return a;
}

int main() { // uncomment
  // cout << func() << endl;
  // cout << pow() << endl;
  // cout << op() << endl;
  // cout << fib(12) << endl;
  // cout << basic() << endl;
  // cout << fib_rec(7) << endl;
  // cout << SQRMLT(4, 7, 13) << endl;
  // cout << ALU() << endl;
  // cout << FORCYCLE() << endl;
  // cout << IFCLAUSE() << endl;
  // cout << LOAD() << endl;
  // cout << MAIN() << endl;
  // cout << STORE() << endl;
}