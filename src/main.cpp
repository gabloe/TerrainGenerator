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

// Literally the same as a list of integers. Not just formalized.
enum ExitCode {
  ExitSuccess = 0,
  ExitOnException = 1,
  ExitOnUnknownException = 2,
};

int main(int argc, const char* argv[]) {
  std::string configPath = asset::Asset::Instance().CONFIG_PATH;
  if (argc == 2) {
    // For testing purposes we want to be able 
    // to provide an alternative configuration path.
    configPath = asset::Asset::Instance().RESOURCE_DIR + "/config/" + argv[1];
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
    return ExitCode::ExitOnException;
  } catch (...) {
    // I am honestly not sure how to handle these, you would think that there
    // would be better things to do here other than "There was an error,
    // sorry!".
    logging::Logger::LogError("An untyped exception was thrown");
    return ExitCode::ExitOnUnknownException;
  }

  return ExitCode::ExitSuccess;
}
