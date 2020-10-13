#include <fstream>
#include <iostream>
#include <string>

char inputString[1000];

class AnyString {
	std::string anyString;

public:
	AnyString(const std::string& anyString) : anyString(anyString) {}
	std::string getAnyString() {
		return "Stored String :: " + anyString;
	}
	friend std::ostream& operator<<(std::ostream& os, const AnyString& str);
};

std::ostream& operator<<(std::ostream& os, const AnyString& str) {
	os << str.anyString;
	return os;
}

int main() {
	std::ofstream out("testOveroding.txt");
	AnyString a("Hello, this is operator overloading test!!!");
	out << a << std::endl;
	out.close();

	//제대로 들어갔는지 확인해보기 위한 파일 읽기 
	/*std::ifstream in("testOveroding.txt");
	while (!in.eof()) {
		in.getline(inputString, 100);
		std:: cout << inputString << std ::endl;
	}*/
	return 0;
}
