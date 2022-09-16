/**
 * Main.cpp skeleton
 * Contributors:
 *      * Gabriel Loewen
 * Licence:
 *      * MIT
 */

#include <TerrainGenerator.hpp>

#include <Asset.hpp>
#include <Logger.hpp>

#include <exception>
#include <stdexcept>

constexpr int ExitSuccess = 0;
constexpr int ExitOnException = ExitSuccess + 1;
constexpr int ExitOnUnknownException = ExitOnException + 1;

int main(int argc, const char* argv[]) {
  std::string configPath = asset::Asset::Instance().CONFIG_PATH;
  if (argc == 2) {
    configPath = argv[1];
  }

  config::ConfigReader configReader{configPath};

  // Check this setting first before logging configuration being loadded.
  if (configReader.ContainsKey("infoLoggingEnabled")) {
    logging::Logger::GetInstance().SetEnabled(
        logging::INF, configReader.ReadBool("infoLoggingEnabled"));
  }

  logging::Logger::LogInfo("Config loaded from " + configPath);

  if (configReader.ContainsKey("debugLoggingEnabled")) {
    auto value = configReader.ReadBool("debugLoggingEnabled");
    logging::Logger::GetInstance().SetEnabled(
        logging::DBG, configReader.ReadBool("debugLoggingEnabled"));
    logging::Logger::LogInfo(
        "Overriding default debug logging enabled value: " +
        std::to_string(value));
  }

  TerrainGenerator app{configReader};

  try {
    app.run();
  } catch (const std::exception& ex) {
    logging::Logger::LogError("An exception was thrown: " +
                              std::string{ex.what()});
    return ExitOnException;
  } catch (...) {
    // I am honestly not sure how to handle these, you would think that there would
    // be better things to do here other than "There was an error, sorry!".
    logging::Logger::LogError("An untyped exception was thrown");
    return ExitOnUnknownException;
  }

  return ExitSuccess;
}
