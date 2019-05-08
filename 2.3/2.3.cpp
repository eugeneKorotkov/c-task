#include <iostream>
#include <iomanip>

int main() {
	
	//simple
	std::cout << "just usual cout: " << 42 << " " << 35.91 << std::endl;

	//numbers
	std::cout << "convert to hex: " << std::hex << 2019 << std::endl;
	std::cout << "convert to hex with uppercase: " << std::hex << std::uppercase << 2019 << std::endl;
	std::cout << "show +: " << std::dec << std::showpos << 10 << std::noshowpos << std::endl;
	//bools
	std::cout << "convert to bool: " << std::boolalpha << true << " " << false << std::endl;
	//width
	std::cout << "make output be of constant width: " << std::setw(10) << 5 << std::endl;
	//width+left align
	std::cout << "make output be of constant width: " << std::setw(10) << std::left << "abcd" << 1 << std::endl;
	//width+right align
	std::cout << "make output be of constant width: " << std::setw(10) << std::right << .432 << std::endl;
	//fill width with char
	std::cout << "make output be of constant width and fill left with certain char: " << std::setw(10) << std::setfill('0') << 50 << std::endl;
	//doubles
	std::cout << "fixed number of digits after comma: " << std::fixed << std::setprecision(5) << 5.743543678 << std::endl;
	return 0;
}