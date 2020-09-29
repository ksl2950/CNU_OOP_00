#include <iostream>
using namespace std;

int i; 

class A {
public:
	~A() {
		i = 10;
	}
};
int foo() {
	i = 3;
	A *ob= new A(); //클래스 동적 할당 , delete 하기 전까지 계속 메모리 유지 
	delete ob;
	return i;
}
int main() {
	cout << foo() << endl;
	
	return 0;
}