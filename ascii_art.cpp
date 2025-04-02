#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <string>

#ifdef _WIN32
#include <windows.h>  // Windows specific header for terminal size
#else
#include <sys/ioctl.h>  // Linux/Mac specific header for terminal size
#include <unistd.h>
#endif

const std::string ASCII_CHARS = "@%#*+=-:. ";  // Dark to light ASCII characters

// Function to get terminal width and height on Windows
#ifdef _WIN32
int getTerminalWidth() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
    return 80;  // Default to 80 if we fail to retrieve
}

int getTerminalHeight() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    }
    return 25;  // Default to 25 if we fail to retrieve
}

#else  // Linux or macOS
int getTerminalWidth() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_col;  // Number of columns
}

int getTerminalHeight() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_row;  // Number of rows
}
#endif

// Function to map pixel intensity to ASCII character
char getAsciiChar(unsigned char pixel) {
    int index = (pixel * (ASCII_CHARS.size() - 1)) / 255;
    return ASCII_CHARS[index];
}

int main(int argc, char* argv[]) {
    if (argc < 3 || std::string(argv[1]) != "--input") {
        std::cerr << "Usage: " << argv[0] << " --input /path/to/image\n";
        return 1;
    }

    std::string inputFile = argv[2];

    // Load image using stb_image
    int width, height, channels;
    unsigned char* img = stbi_load(inputFile.c_str(), &width, &height, &channels, 1);
    if (!img) {
        std::cerr << "Error: Failed to load image\n";
        return 1;
    }

    // Get terminal dimensions
    int terminalWidth = getTerminalWidth();
    int terminalHeight = getTerminalHeight();

    // Calculate new image size based on terminal size
    float aspectRatio = (float)width / (float)height;
    int newWidth = terminalWidth;
    int newHeight = static_cast<int>(newWidth / aspectRatio);

    // Resize image to fit terminal (simple nearest neighbor scaling)
    unsigned char* resizedImg = new unsigned char[newWidth * newHeight];
    for (int y = 0; y < newHeight; ++y) {
        for (int x = 0; x < newWidth; ++x) {
            int oldX = x * width / newWidth;
            int oldY = y * height / newHeight;
            resizedImg[y * newWidth + x] = img[oldY * width + oldX];
        }
    }

    // Adjust newHeight if it exceeds terminal height
    if (newHeight > terminalHeight) {
        newHeight = terminalHeight;
        newWidth = static_cast<int>(newHeight * aspectRatio);

        // Resize image to fit terminal (simple nearest neighbor scaling)
        delete[] resizedImg;
        resizedImg = new unsigned char[newWidth * newHeight];
        for (int y = 0; y < newHeight; ++y) {
            for (int x = 0; x < newWidth; ++x) {
                int oldX = x * width / newWidth;
                int oldY = y * height / newHeight;
                resizedImg[y * newWidth + x] = img[oldY * width + oldX];
            }
        }
    }   

    // Print ASCII art based on resized image
    for (int y = 0; y < newHeight; y++) {
        for (int x = 0; x < newWidth; x++) {
            unsigned char pixel = resizedImg[y * newWidth + x];
            std::cout << getAsciiChar(pixel);
        }
        std::cout << "\n";
    }

    // Clean up
    delete[] resizedImg;
    stbi_image_free(img);
    return 0;
}