# driver-steganography
Driver Steganography is a steganographic concealment tool that hides encrypted kernel drivers inside carrier files. <br />
It uses XOR encryption combined with file-append steganography to embed drivers in innocent-looking files. <br />
The hidden bytes remains undetectable until you extract it with the magic marker during the decryption process. <br />

# What Is Steganography?
Steganography conceals data within carrier files:
- Carrier Files - Any legitimate file (EXE, DLL, PNG, PDF, etc.)
- Hidden File - Encrypted file appended after carrier data
- Magic Marker - Signature string to identify payload
- Extraction - Marker search and XOR decryption to recover hidden file

# Configuration
Configure encryption settings in the source:
```cpp
const uint8_t XOR_KEY = 0xAA; // XOR encryption key
const std::string MAGIC_MARKER = "DRIVERMAGIC1234"; // Payload identifier
```

**Encrypt Mode** (Hide driver inside carrier file) <br />
Result: output.exe appears normal but contains hidden driver
```cpp
encrypt_file("carrier.exe", "driver.sys", "output.exe");
```

**Decrypt Mode** (Extract hidden driver from carrier) <br />
Result: Original driver.sys extracted and decrypted
```cpp
decrypt_file("output.exe", "extracted_driver.sys");
```

# Conclusion
Driver Steganography provides a simple but effective mechanism for concealing kernel drivers within legitimate carrier files. <br />
The XOR encryption combined with file-append steganography ensures the payload remains hidden from casual inspection. <br />
