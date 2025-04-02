# ASCII Art Generator

A cross-platform C++ program that converts images to ASCII art, automatically adjusting to your terminal size.

## Features

- Converts any image format supported by stb_image (JPG, PNG, BMP, etc.) to ASCII art
- Automatically adjusts to terminal dimensions
- Cross-platform support (Windows, Linux, macOS)
- Maintains aspect ratio while resizing
- Simple and efficient implementation

## Dependencies

- C++ compiler (GCC, Clang, MSVC, etc.)
- stb_image.h (included in the repository)

## Building

```bash
# On Linux/macOS
g++ -o ascii_art ascii_art.cpp

# On Windows with MinGW
g++ -o ascii_art.exe ascii_art.cpp

# On Windows with MSVC
cl ascii_art.cpp
```

## Usage

```bash
./ascii_art --input path/to/image
```

## Example

```bash
./ascii_art --input sample.jpg
```

The program will convert the image to ASCII art and display it in your terminal, automatically adjusting to your terminal's dimensions while maintaining the aspect ratio.

## How it Works

1. Loads the image using stb_image library
2. Detects terminal dimensions
3. Resizes the image to fit the terminal while maintaining aspect ratio
4. Converts each pixel to an ASCII character based on brightness
5. Outputs the result to the terminal

## License

This project is open source and available under the MIT License.

## Credits

- stb_image.h by Sean Barrett (https://github.com/nothings/stb)