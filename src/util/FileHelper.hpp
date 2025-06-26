#pragma once
#include <unordered_map>
#include <string>

namespace Quark {
  class FileHelper {
  public:
    static std::string getMimeType(const std::string &path);

    static bool isValidFilePath(const std::string &path);

  private:
    static const std::unordered_map<std::string, std::string> mimeMapping;
  };
}
