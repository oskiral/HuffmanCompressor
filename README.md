# Huffman Compressor

**A simple text file compressor using the Huffman coding algorithm written in C++**

---

## Project Description

This program implements the classic Huffman algorithm for text compression.  
It reads an input text file, analyzes the frequency of each character,  
builds a Huffman tree, encodes the data into a binary format, and saves it to a `.huff` file.

Additionally, the program can read the `.huff` file, decompress it, and restore the original text.

---

## Features

- Reads a text file for compression
- Builds a Huffman tree based on character frequencies
- Encodes text into a bitstream and writes it to a binary file
- Saves the code map for decompression purposes
- Decompresses `.huff` files back to original text

---

## Usage

### Compression:

1. Place your input text file named `input.txt` in the program directory.
2. Run the program, which will automatically read `input.txt`, compress its contents, and save it as `compressed.huff`.

### Decompression:

After compression, the program automatically reads `compressed.huff`, decompresses the data, and outputs the original text to the console.

---

## File Structure

- `input.txt` — the source text file to compress  
- `compressed.huff` — the output file containing encoded data and code map for decompression  
- `main.cpp` — the source code of the compressor and decompressor  

---

## Requirements

- C++ compiler (e.g. g++ or clang++)  
- C++11 standard or newer  

---

## Build and Run

```bash
g++ -std=c++11 -o huffman main.cpp
./huffman
