#include "../../include/network/server/compress_data.hpp"

// Compresse les données en utilisant la bibliothèque zlib
// data - la chaîne à compresser
// Return chaîne compressée
std::string Compress_data::compressData(const std::string& data) {
    // taille nécessaire pour stocker les données compressées
    uLongf compressedSize = compressBound(data.size());
    std::vector<char> compressedData(compressedSize);  // Buffer pour stocker les données compressées
    
    if (compress(reinterpret_cast<Bytef*>(compressedData.data()), &compressedSize, reinterpret_cast<const Bytef*>(data.data()), data.size()) != Z_OK) {
        throw std::runtime_error("Erreur de compression des données");
    }
    
    // Retourne la chaîne compressée, redimensionnée à la taille réelle des données compressées
    return std::string(compressedData.data(), compressedSize);
}

std::string Compress_data::decompressData(const std::string& compressedData) {
    uLongf decompressedSize = compressedData.size() * 4;
    std::vector<char> decompressedData(decompressedSize);
    
    while (uncompress(reinterpret_cast<Bytef*>(decompressedData.data()), &decompressedSize, reinterpret_cast<const Bytef*>(compressedData.data()), compressedData.size()) == Z_BUF_ERROR) {
        // Si le buffer est trop petit, double la taille et réessaye
        decompressedSize *= 2;
        decompressedData.resize(decompressedSize);
    }
    
    return std::string(decompressedData.data(), decompressedSize);
}
