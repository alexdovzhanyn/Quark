#include "FileHelper.hpp"
#include <filesystem>

const std::unordered_map<std::string, std::string> Quark::FileHelper::mimeMapping = {
  {".html", "text/html; charset=utf-8"},
  {".css", "text/css"},
  {".js", "application/javascript"},
  {".json", "application/json"},
  {".png", "image/png"},
  {".jpg", "image/jpeg"},
  {".jpeg", "image/jpeg"},
  {".svg", "image/svg+xml"},
  {".txt", "text/plain"}
};

std::string Quark::FileHelper::getMimeType(const std::string &path) {
  size_t dot = path.rfind('.');
  
  // If no extension, treat it as binary
  if (dot == std::string::npos) return "application/octet-stream";

  std::string extension = path.substr(dot);
  auto it = mimeMapping.find(extension);

  // If we don't know what the mime type is, treat it as binary
  if (it == mimeMapping.end()) return "application/octet-stream";

  return it->second;
}

bool Quark::FileHelper::isValidFilePath(const std::string &path) {
  return std::filesystem::exists(path);
}
