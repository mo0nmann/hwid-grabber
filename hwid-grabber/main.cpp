#include <iostream>
#include <vector>
#include <string>

#include <PortableDeviceApi.h>

#include "gpu_nvml_grabber.hpp"
#include "portable_device_list_grabber.hpp"

void printVector(std::vector<std::string> vec) {
	for (auto i : vec) {
		std::cout << i << std::endl;
	}
}

// prints horizontal line to the width of the windows terminal
void printHorizontalLine() {

	// get terminal width
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	int terminal_width = csbi.srWindow.Right - csbi.srWindow.Left + 1;

	SetConsoleOutputCP(CP_UTF8);
	for (int i = 0; i < terminal_width; i++) {
		std::cout << u8"\u203e";
	}

}

int main(void) {

	std::vector<std::string> hardware_IDs;

	// get GPU info from NVML first
	hardware_IDs = getGPUInfo();

	std::cout << "GPUs:" << std::endl;
	printHorizontalLine();

	printVector(hardware_IDs);

	std::cout << std::endl;

	std::cout << "Portable Devices:" << std::endl;
	printHorizontalLine();

	std::vector<std::string> portable_IDs = getPortableDeviceHWIDs();
	
	if (portable_IDs.empty()) {
		std::cout << "No Devices Found" << std::endl;
	} else {
		printVector(portable_IDs);
	}

	std::cin.get();
	return 0;

}