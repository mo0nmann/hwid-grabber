#include <iostream>
#include <vector>
#include <string>

#include <PortableDeviceApi.h>

#include "gpu_nvml_grabber.hpp"
#include "device_list_grabber.hpp"

int main(void) {

	std::vector<std::string> hardware_IDs;

	// get GPU info from NVML first
	hardware_IDs = getGPUInfo();

	

	// ---------------------------------------- //
	/*
	
	*/

	for (size_t i = 0; i < hardware_IDs.size(); i++) {
		std::cout << hardware_IDs[i] << std::endl;
	}

	std::cout << std::endl;

	getDeviceHWIDs();

	system("pause");
	return 0;

}