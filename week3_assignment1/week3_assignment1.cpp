#include <iostream>

class SetValue {
private:
	int x, y;
public:
	int getX() {	return x;	}
	int getY() {	return y;	}
	void setX(int newX) {	x = newX;	}
	void setY(int newY) {	y = newY;	}
};

int main() {
	SetValue obj;
	obj.setX(33);
	obj.setY(44);

	std::cout << "X= " << obj.getX() << ", Y= " << obj.getY() << std::endl;
	system("pause");
	return 0;
}