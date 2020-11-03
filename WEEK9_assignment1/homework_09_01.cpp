#include <iostream>

class MyData {
	int number;
	std::string strNumber;
public:
	MyData(int data, std::string str) : number(data), strNumber(str) {}
	//Operator conversion
	operator int() { return number; }
	operator std::string() { return strNumber; }

	//Unary operator(단항 연산자)
	//후위
	int operator++(int) { //postfix operation (indicated by dummy 'int')
		MyData mydata(number,strNumber);
		number += 1;
		return mydata;
	}
	//전위
	int operator++() { //prefix operation
		number +=1; 
		return number;
	}

	friend std::ostream& operator<<(std::ostream&, MyData&);
};

// non-member operator<< function
std::ostream& operator<<(std::ostream& os, MyData& md) {
	return os << "This number is: " << md.strNumber << "\n";
}

int main() {
	MyData mydata(1, "one"); //객체 생성

	std::string strNum = mydata;
	int intNum = mydata;

	std::cout << strNum << std::endl; // one
	std::cout << intNum << std::endl; // 1
	std::cout << mydata++ << std::endl; // 1
	std::cout << ++mydata << std::endl; // 3
	std::cout << mydata;

}