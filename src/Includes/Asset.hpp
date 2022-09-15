#pragma once

#include <string>
#include <iostream>
#include <limits.h>

#if defined(__APPLE__) || defined(__MACH__)
#include <mach-o/dyld.h>
#elif defined(__WINDOWS__) || defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#elif defined(__linux__)
#include <unistd.h> 
#endif

namespace asset {

	const std::string getExecutablePath();

	class Asset {
	private:

		static std::unique_ptr<Asset> instance;

	protected:
		Asset();

	public:
		static const Asset& Instance()
		{
			if (instance == nullptr)
			{
				instance = std::unique_ptr<Asset>(new Asset{});
			}
			return *instance;
		}

		const std::string RESOURCE_DIR;
		const std::string SHADERS_DIR;
		const std::string MODELS_DIR;
		const std::string CONFIG_PATH;
	};
} // namespace asset