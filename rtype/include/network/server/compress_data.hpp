#pragma once
#include <stdexcept>
#include <vector>
#include <zlib.h>

class Compress_data
{
public:
    Compress_data() {};
    ~Compress_data() {};
    std::string compressData(const std::string& data);
    std::string decompressData(const std::string& compressedData);
};
