#include <iostream>

struct MyStruct {
	char a;
	int b;
};

void modifyStruct(MyStruct& s) {
	s.b = 42;
}

int main() {
	int x = 42;
	MyStruct str1;
	std::cout << "Hello world!\n"; 
	std::cout << "Hello " << x << "\n";
	
	str1.a = 1;
	str1.b = 2;
	modifyStruct(str1);
	std::cout << "Hello struct {a=" << str1.a << ", b=" << str1.b << "}\n";
	return 0;
}
