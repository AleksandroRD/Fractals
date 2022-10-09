#include "debug.h"
#include <iostream>

void DebugLog(std::string text) {
	std::cout << text << std::endl;
}
void DebugLog(double text) {
	std::cout << text << std::endl;
}
void DebugLog(float text) {
	std::cout << text << std::endl;
}
void DebugLog(int text) {
	std::cout << text << std::endl;
}