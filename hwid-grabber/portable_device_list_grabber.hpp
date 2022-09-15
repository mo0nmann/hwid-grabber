#include <iostream>
#include <vector>
#include <string>
#include <locale>
#include <codecvt>

#include <PortableDeviceApi.h>
#include <wrl.h>


inline std::string pwstrToBasic(PWSTR string) {
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	return converter.to_bytes(string);
}


// function to obtain the name of a device
// if no name is provided, "Unknown" is returned
inline std::string getDeviceName(_In_ IPortableDeviceManager* device_manager, _In_ PCWSTR pnp_device_id) {
	
	std::string device_name = "Unknown";
	DWORD name_length = 0;

	// pass nullptr as the PWSTR return string parameter to get the total number
	// of characters to allocate for the string value
	HRESULT hr = device_manager->GetDeviceFriendlyName(pnp_device_id, nullptr, &name_length);
	if (FAILED(hr)) {
		std::cout << "Failed to get number of characters for device friendly name, hr = " << std::hex << hr << std::endl;
		system("pause");
		exit(1);
	
	} else if (name_length > 0) {
		
		// allocate the number of characters needed and retrieve the string value
		PWSTR name = new (std::nothrow) WCHAR[name_length];
		if (name != nullptr) {
			
			SecureZeroMemory(name, name_length * sizeof(WCHAR));
			
			hr = device_manager->GetDeviceFriendlyName(pnp_device_id, name, &name_length);
			if (SUCCEEDED(hr)) {
				// convert PWSTR to basic string and store
				device_name = pwstrToBasic(name);
			} 
			
			// delete from memory
			delete[] name;
			name = nullptr;

		}
		else {
			std::cout << "Failed to allocate memory for the device friendly name string" << std::endl;
			system("pause");
			exit(1);
		}

	}
	
	return device_name;
}


inline std::vector<std::string> getPortableDeviceHWIDs() {

	std::vector<std::string> information;

	// Initialize
	HRESULT hr = CoInitialize(nullptr);
	if (FAILED(hr)) {
		std::cout << "Failed to Initialize PortableDeviceManager!, hr = " << std::hex << hr << std::endl;
	}

	// create portable device manager object
	Microsoft::WRL::ComPtr<IPortableDeviceManager> device_manager;
	hr = CoCreateInstance(CLSID_PortableDeviceManager,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&device_manager));

	if (FAILED(hr)) {
		std::cout << "! Failed to CoCreateInstance CLSID_PortableDeviceManager, hr = " << std::hex << hr << std::endl;
	}

	// obtain amount of devices
	DWORD pnp_device_id_count = 0;
	if (SUCCEEDED(hr)) {
		hr = device_manager->GetDevices(nullptr, &pnp_device_id_count);

		if (FAILED(hr)) {
			std::cout << "! Failed to get number of devices on the system, hr = " << std::hex << hr << std::endl;
		}
	}

	// if devices found
	if (SUCCEEDED(hr) && (pnp_device_id_count > 0)) {
		
		PWSTR* pnp_device_ids = new (std::nothrow) PWSTR[pnp_device_id_count];
		if (pnp_device_ids != nullptr) {

			SecureZeroMemory(pnp_device_ids, pnp_device_id_count * sizeof(PWSTR));
			DWORD retrieved_device_id_count = pnp_device_id_count;

			hr = device_manager->GetDevices(pnp_device_ids, &retrieved_device_id_count);
			if (SUCCEEDED(hr)) {

				_Analysis_assume_(retrieved_device_id_count <= pnp_device_id_count);

				for (DWORD index = 0; index < retrieved_device_id_count; index++) {

					std::string identifier = pwstrToBasic(pnp_device_ids[index]);

					std::string partition_one = identifier.substr(identifier.find("vid_") + 4);
					std::string vid = partition_one.substr(0, partition_one.find("&"));

					std::string partition_two = identifier.substr(identifier.find("pid_") + 4);
					std::string pid = partition_two.substr(0, partition_two.find("#"));

					std::string partition_three = identifier.substr(identifier.find(pid + "#") + 5);
					std::string serial_number = partition_three.substr(0, partition_three.find("#"));

					information.push_back(
						getDeviceName(device_manager.Get(), pnp_device_ids[index])
						+ "\n"
						+ "identifier: " + identifier
						+ "\n"
						+ "VID: " + vid
						+ "\n"
						+ "PID: " + pid
						+ "\n"
						+ "Serial#: " + serial_number
					);
				}

			}
		}
	}

	return information;
}