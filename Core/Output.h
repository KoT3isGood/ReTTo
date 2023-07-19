#pragma once
#include <iostream>
#include <string>
#include <Windows.h>

enum OutPutType { info,ok, warning, error };

template<typename T>

int print(OutPutType type, T string) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (type == OutPutType::info) {
		SetConsoleTextAttribute(hConsole, 23);
		std::cout << " INFO ";
	}
	if (type == OutPutType::error) {
		SetConsoleTextAttribute(hConsole, 64);
		std::cout << " ERROR ";
	}
	if (type == OutPutType::warning) {
		SetConsoleTextAttribute(hConsole, 96);
		std::cout << " WARNING ";
	}
	if (type == OutPutType::ok) {
		SetConsoleTextAttribute(hConsole, 32);
		std::cout << " OK ";
	}
	SetConsoleTextAttribute(hConsole, 7);
	std::cout << " " << string << "\n";

	return 0;

};


