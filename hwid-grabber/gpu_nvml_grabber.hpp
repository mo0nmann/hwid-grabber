#include <iostream>
#include <vector>
#include <string>
#include <nvml.h>

inline void errorHandler(_In_ nvmlReturn_t result) {
	result = nvmlShutdown();
	if (NVML_SUCCESS != result) {
		std::cout << "Failed to shutdown NVML " << nvmlErrorString(result) << std::endl;
	}
	system("pause");
	exit(1);
}

inline std::vector<std::string> getGPUInfo() {

	nvmlReturn_t result;
	unsigned int device_count, i;

	// First initialize NVML library
	result = nvmlInit();
	if (NVML_SUCCESS != result) {
		std::cout << "Failed to initialize NVML: " << nvmlErrorString(result) << std::endl;
		system("pause");
		exit(1);
	}

	// Get GPU count
	result = nvmlDeviceGetCount(&device_count);
	if (NVML_SUCCESS != result) {
		std::cout << "Failed to query device count " << nvmlErrorString(result) << std::endl;
		errorHandler(result);
	}

	std::vector<std::string> information;

	// loop through GPUs
	for (i = 0; i < device_count; i++) {

		nvmlDevice_t device;

		char uuid[128];
		char serial_number[128];

		// get handle to device
		result = nvmlDeviceGetHandleByIndex(i, &device);
		if (NVML_SUCCESS != result) {
			std::cout << "Failed to get handle for device " << nvmlErrorString(result) << std::endl;
			errorHandler(result);
		}

		// get the device UUID
		result = nvmlDeviceGetUUID(device, uuid, sizeof(uuid));
		if (NVML_SUCCESS != result) {
			std::cout << "Failed to get UUID for device " << nvmlErrorString(result) << std::endl;
			errorHandler(result);
		}

		// get the device serial number
		result = nvmlDeviceGetSerial(device, serial_number, sizeof(serial_number));
		if (NVML_SUCCESS != result) {
			strcpy_s(serial_number, nvmlErrorString(result));
		}

		std::string gpu_uuid(uuid);
		std::string gpu_serial_number(serial_number);
		

		information.push_back("GPU UUID: " + gpu_uuid);
		information.push_back("GPU Serial Number: " + gpu_serial_number);
	}

	result = nvmlShutdown();
	if (NVML_SUCCESS != result) {
		std::cout << "Failed to shutdown NVML " << nvmlErrorString(result) << std::endl;
		system("pause");
		exit(1);
	}

	return information;
}