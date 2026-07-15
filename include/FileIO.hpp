#pragma once

#include <string>

void generateMassivefile(const std::string& filepath, size_t targetsizeMB);

size_t parseFile_baseline(const std::string& filepath);

size_t parseFile_mmap(const std::string& filepath);