#include <Asset.hpp>
#include <filesystem>
#include <iostream>

#include <Logger.hpp>

#if defined(__APPLE__) || defined(__MACH__)
#include <mach-o/dyld.h>
#elif defined(__WINDOWS__) || defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#elif defined(__linux__)
#include <unistd.h>
#include <cerrno>
#endif

using namespace asset;
namespace fs = std::filesystem;

std::unique_ptr<Asset> Asset::instance;

#if defined(__APPLE__) || defined(__MACH__)
const uint32_t path_length = PATH_MAX;
#elif defined(__linux__)
const uint32_t path_length = PATH_MAX;
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
  auto result = _NSGetExecutablePath(&path_buf[0], &path_length);
  if (error != 0) {
    auto msg = "Failed call: _NSGetExecutablePath" + std::to_string(error);
    logging::Logger::LogError(msg);
    throw new std::runtime_error{msg};
  }

  executable_path = path_buf;
#elif defined(__WINDOWS__) || defined(_WIN32) || defined(_WIN64)

  auto error = GetModuleFileName(NULL, &path_buf[0], path_length);
  if (IS_ERROR(error)) {
    auto msg = "Failed call: GetModuleFileName" + std::to_string(error);
    logging::Logger::LogError(msg);
    throw new std::runtime_error{msg};
  }

  executable_path = path_buf;
  // Windows includes the executable in the name
  // so we need to remove it.
  executable_path = executable_path.parent_path();
#elif defined(__linux__)
  auto bytes = readlink("/proc/self/exe", &path_buf[0], path_length);
  if (bytes == -1) {
    auto msg = "Failed call: readlink" + std::to_string(errno);
    logging::Logger::LogError(msg);
    throw new std::runtime_error{msg};
  }

  executable_path = path_buf;
#else
  throw new std::runtime_error{"Invalid operating system"};
#endif

  if (!executable_path.empty()) {
    return executable_path.parent_path().string();
  } else {
    auto msg = "Could not determine current executable path";
    logging::Logger::LogError(msg);
    throw new std::runtime_error{msg};
  }
}

Asset::Asset()
    : RESOURCE_DIR{getExecutablePath() + "/resources"},
      SHADERS_DIR{RESOURCE_DIR + "/shaders"},
      MODELS_DIR{RESOURCE_DIR + "/models"},
      CONFIG_PATH{RESOURCE_DIR + "/config/config.json"},
      FONT_DIR{RESOURCE_DIR + "/fonts"} {
  /* Empty */
}
