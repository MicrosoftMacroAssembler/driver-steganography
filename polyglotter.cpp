#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

const uint8_t XOR_KEY = 0xAA;
const std::string MAGIC_MARKER = "DRIVERMAGIC1234";

void print_usage() {
    std::cout << "CLI Driver Hider\n"
        << "Interactive usage:\n"
        << "  Choose mode: encrypt or decrypt\n"
        << "  Provide required file paths when prompted.\n";
}

// Encrypt function
bool encrypt_file(const std::string& carrierPath, const std::string& driverPath, const std::string& outputPath) {
    std::ifstream carrierFile(carrierPath, std::ios::binary);
    if (!carrierFile) { std::cerr << "Failed to open carrier file\n"; return false; }
    std::vector<uint8_t> carrierData((std::istreambuf_iterator<char>(carrierFile)), std::istreambuf_iterator<char>());
    carrierFile.close();

    std::ifstream driverFile(driverPath, std::ios::binary);
    if (!driverFile) { std::cerr << "Failed to open driver file\n"; return false; }
    std::vector<uint8_t> driverData((std::istreambuf_iterator<char>(driverFile)), std::istreambuf_iterator<char>());
    driverFile.close();

    for (auto& b : driverData) b ^= XOR_KEY; // simple XOR encryption

    std::ofstream outFile(outputPath, std::ios::binary);
    if (!outFile) { std::cerr << "Failed to create output file\n"; return false; }
    outFile.write(reinterpret_cast<char*>(carrierData.data()), carrierData.size());
    outFile.write(MAGIC_MARKER.c_str(), MAGIC_MARKER.size());
    outFile.write(reinterpret_cast<char*>(driverData.data()), driverData.size());
    outFile.close();

    std::cout << "Driver encrypted and appended to: " << outputPath << "\n";
    return true;
}

// Decrypt function
bool decrypt_file(const std::string& inputPath, const std::string& outputDriverPath) {
    std::ifstream inFile(inputPath, std::ios::binary);
    if (!inFile) { std::cerr << "Failed to open input file\n"; return false; }
    std::vector<uint8_t> fileData((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    inFile.close();

    auto it = std::search(fileData.begin(), fileData.end(), MAGIC_MARKER.begin(), MAGIC_MARKER.end());
    if (it == fileData.end()) { std::cerr << "Marker not found!\n"; return false; }

    size_t payloadStart = std::distance(fileData.begin(), it) + MAGIC_MARKER.size();
    std::vector<uint8_t> driverData(fileData.begin() + payloadStart, fileData.end());

    for (auto& b : driverData) b ^= XOR_KEY; // decrypt

    std::ofstream outDriver(outputDriverPath, std::ios::binary);
    if (!outDriver) { std::cerr << "Failed to create driver output file\n"; return false; }
    outDriver.write(reinterpret_cast<char*>(driverData.data()), driverData.size());
    outDriver.close();

    std::cout << "Driver extracted to: " << outputDriverPath << "\n";
    return true;
}

int main() {
    print_usage();

    std::string mode;
    std::cout << "\nEnter mode (encrypt/decrypt): ";
    std::getline(std::cin, mode);
    std::transform(mode.begin(), mode.end(), mode.begin(), ::tolower);

    if (mode == "encrypt") {
        std::string carrierFile, driverFile, outputFile;
        std::cout << "Carrier file path: ";
        std::getline(std::cin, carrierFile);
        std::cout << "Driver file path: ";
        std::getline(std::cin, driverFile);
        std::cout << "Output file path: ";
        std::getline(std::cin, outputFile);

        if (!encrypt_file(carrierFile, driverFile, outputFile)) {
            std::cerr << "Encryption failed!\n";
            return 1;
        }
    }
    else if (mode == "decrypt") {
        std::string inputFile, outputDriver;
        std::cout << "Input file path: ";
        std::getline(std::cin, inputFile);
        std::cout << "Output driver file path: ";
        std::getline(std::cin, outputDriver);

        if (!decrypt_file(inputFile, outputDriver)) {
            std::cerr << "Decryption failed!\n";
            return 1;
        }
    }
    else {
        std::cerr << "Invalid mode!\n";
        return 1;
    }

    std::cout << "Operation completed successfully.\n";
    return 0;
}
