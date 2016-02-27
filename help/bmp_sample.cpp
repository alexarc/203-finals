#include <iostream>
#include <fstream>
#include <vector>

// We have write() functions, because the streaming operator (operator<<) would 
// write the values as text and we want binary
void write(std::ostream &os, std::uint32_t value) {
	os.write(reinterpret_cast<const char *>(&value), 4);
}

void write(std::ostream &os, std::uint16_t value) {
	os.write(reinterpret_cast<const char *>(&value), 2);
}

// Write the BMP header. See the wikipedia page for more details:
// https://en.wikipedia.org/wiki/BMP_file_format
void write_bmp_header(std::ostream &os) {
	// BMP Header
	os << "BM";							// Signature
	write(os, 512u * 512u * 3u + 54u);	// File size
	write(os, 0u);						// Reserved
	write(os, 54u);						// Image offset

	// DIB Header
	write(os, 40u);						// DIB header size
	write(os, 512u);					// Width
	write(os, 512u);					// Height
	write(os, std::uint16_t(1));		// Number of color planes
	write(os ,std::uint16_t(24));		// Bits per pixel (24 = 3 bytes)
	write(os, 0u);						// Compression method
	write(os, 512u * 512u * 3u);		// Image size (pixel data)
	write(os, 0u);						// Image resolution, horizontal
	write(os, 0u);						// Image resolution, vertical
	write(os, 0u);						// Size of palette plane
	write(os, 0u);						// All colors are important
}

void set_black_point(std::vector<char> &buffer, int x, int y) {
	// We have to start from the end because BMP files are upside-down
	int index = ((511 - y) * 512 + x) * 3;
	buffer[index] = 0;
	buffer[index + 1] = 0;
	buffer[index + 2] = 0;
}

int main() {
	// Allocate a pixel buffer of 3 bytes per pixel (red, blue, green)
	std::vector<char> pixel_buffer(512 * 512 * 3, 0xff);

	// Draw a line between (30, 200) and (60, 200)
	for (int x = 30; x < 60; ++x)
		set_black_point(pixel_buffer, x, 200);

	// Enable throwing exceptions, and open the file as binary
	std::ofstream ofs;
	ofs.exceptions(std::ios::badbit);
	ofs.open("art.bmp", std::ios::binary);

	// Write header then pixel buffer
	write_bmp_header(ofs);
	ofs.write(&pixel_buffer[0], pixel_buffer.size());

	return 0;
}
