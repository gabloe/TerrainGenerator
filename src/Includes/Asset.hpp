#pragma once

#include <string>
#include <iostream>
#include <limits.h>
#include <memory>

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