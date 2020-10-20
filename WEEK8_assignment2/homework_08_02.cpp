#include <iostream>

using namespace std;

class Strategy { //인터페이스 
public:
	virtual int doOperation(int num1, int num2) =0;
};

class OperationAdd : public Strategy {
public:
	int doOperation(int num1, int num2) {
		return num1 + num2;
	}
};

class OperationSubstract : public Strategy {
public:
	int doOperation(int num1, int num2) {
		return num1 - num2;
	}
};

class OperationMultiply : public Strategy {
public:
	int doOperation(int num1, int num2) {
		return num1 * num2;
	}
};
class Context {
private:
	Strategy *strategy;
public:
	Context(Strategy *strategy) {
		this->strategy = strategy;
	}
	int executeStrategy(int num1, int num2) {
		return strategy->doOperation(num1, num2);
	}

};
int main() {
	Context *context = new Context(new OperationAdd());
	cout << "10 + 5 = " << context->executeStrategy(10, 5) << endl; 

	context = new Context(new OperationSubstract());
	cout << "10 - 5 = " << context->executeStrategy(10, 5) << endl;

	context = new Context(new OperationMultiply());
	cout << "10 * 5 = " << context->executeStrategy(10, 5) << endl;
}