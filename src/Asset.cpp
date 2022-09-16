#include <Asset.hpp>
#include <filesystem>
#include <iostream>

#include <Logger.hpp>

using namespace asset;
namespace fs = std::filesystem;

std::unique_ptr<Asset> Asset::instance;


#if defined(__APPLE__) || defined(__MACH__)
uint32_t path_length = PATH_MAX;
#elif defined(__linux__)
constexpr uint32_t path_length = PATH_MAX;
#else
constexpr uint32_t path_length = 255;
#endif

const std::string asset::getExecutablePath() {
    logging::Logger::LogInfo("getExecutablePath");

    // buffer the path data
    std::string path_buf(path_length, std::string::value_type());

    // construct a std::filesystem::path to get the parent
    fs::path executable_path;

    #if defined(__APPLE__) || defined(__MACH__)
    if (_NSGetExecutablePath(&path_buf[0], &path_length) == 0) {
        executable_path = path_buf;
    }
    #elif defined(__WINDOWS__) || defined(_WIN32) || defined(_WIN64)
    logging::Logger::LogInfo("WINDOWS");
    
    auto error =GetModuleFileName(NULL, &path_buf[0], path_length);
    if (IS_ERROR(error)) {
        logging::Logger::LogInfo("ERROR");
        throw new std::runtime_error{"Failed call: GetModuleFileName" + std::to_string(error)};
    }

    logging::Logger::LogInfo("SUCCESS");
    executable_path = path_buf;
    executable_path = executable_path.parent_path();

    #elif defined(__linux__)
    if (readlink("/proc/self/exe", &path_buf[0], path_length) > 0) {
        executable_path = path_buf;
    }
    #else
    throw new std::runtime_error{"Invalid operating system"};
    #endif

    if (!executable_path.empty()) {
        return executable_path.parent_path().string();
    } else {
        throw new std::runtime_error{"Could not determine current executable path"};
    }
}

Asset::Asset(): RESOURCE_DIR{ getExecutablePath() + "/resources" },
        SHADERS_DIR{ RESOURCE_DIR + "/shaders"},
        MODELS_DIR{ RESOURCE_DIR + "/models"},
        CONFIG_PATH{RESOURCE_DIR + "/config/config.json"}
{
}
