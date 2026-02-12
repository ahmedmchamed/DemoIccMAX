#ifndef COMMAND_LINE_UTILITY_H
#define COMMAND_LINE_UTILITY_H

#include <string>
#include <filesystem>
#include <unordered_map>
#include <utility>
#include <vector>
#include "Config.h"

namespace Utils {
    class Util final {
        public:
            using CommandLineArgValue = std::pair<std::string, Config>;
            Util() = default;
            CommandLineArgValue getCommandLineArgValue(std::string const& argKey);
            Config parseCommandLine(std::vector<std::string> const& commandLineArgs);
            static Config readFrom(std::filesystem::path const& filePath);
            static void writeTo(
                std::filesystem::path const& filePath,
                std::vector<Config::Row> const& outputData,
                Config const& colourData);

        private:
            // Command line args - key is option name, value is
            // complex type of option value and value's data
            std::unordered_map<std::string, CommandLineArgValue> mCommandLineArgs;
    };
}

#endif
