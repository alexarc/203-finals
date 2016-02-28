#include <vector>
#include <string>
#include <map>
#include <algorithm>

#define NOMINMAX
#include <Windows.h>

struct screen_buffer {
	COORD buffer_size = { 80, 25 };
	std::vector<CHAR_INFO> buffer;
	HANDLE handle;

	screen_buffer(HANDLE handle)
	 : handle(handle) {
		// Hide the cursor because it's ugly
		CONSOLE_CURSOR_INFO cursor_info;
		cursor_info.bVisible = false;
		cursor_info.dwSize = 1;
		SetConsoleCursorInfo(handle, &cursor_info);

		// Clear the console before the first frame
		clear();
	}
	
	void clear() {
		// Fill the console with bright blue on black, space characters
		CHAR_INFO fill_char;
		fill_char.Char.AsciiChar = ' ';
		fill_char.Attributes = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		std::vector<CHAR_INFO>().swap(buffer);
		buffer.resize(buffer_size.X * buffer_size.Y, fill_char);
	}

	void write_char(int x, int y, char val) {
		buffer[x + y * buffer_size.X].Char.AsciiChar = val;
	}

	void show() {
		// Write buffer to the console
		COORD zero_coords = { 0, 0 };
		SMALL_RECT write_region = { 0, 0, buffer_size.X, buffer_size.Y };
		WriteConsoleOutputA(handle, buffer.data(), buffer_size, zero_coords, &write_region);
	}
};

int main() {
	HANDLE in_handle = GetStdHandle(STD_INPUT_HANDLE);
	screen_buffer screen(GetStdHandle(STD_OUTPUT_HANDLE));
	
	// Time counters for game time tracking
	LARGE_INTEGER last_timestamp;
	LARGE_INTEGER timestamp;
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	LONGLONG frame_ticks = frequency.QuadPart / 24; // 24 fps
	QueryPerformanceCounter(&last_timestamp);

	int x = 5, y = 5;
	std::map<WORD, BOOL> key_states;

	for (;;) {
		// Read keyboard events from the console
		DWORD n_events = 0;
		while (GetNumberOfConsoleInputEvents(in_handle, &n_events), n_events > 0) {
			INPUT_RECORD record;
			ReadConsoleInputA(in_handle, &record, 1, &n_events);
			if (record.EventType == KEY_EVENT) {
				// Record key states in the global key map
				key_states[record.Event.KeyEvent.wVirtualKeyCode] = record.Event.KeyEvent.bKeyDown;
			}
		}

		// Move the character around the screen
		if (key_states[VK_LEFT]) x--;
		if (key_states[VK_RIGHT]) x++;
		if (key_states[VK_UP]) y--;
		if (key_states[VK_DOWN]) y++;

		// Wrap around borders
		y = (y + 25) % 25;
		x = (x + 80) % 80;

		// Update back-buffer state
		screen.clear();
		screen.write_char(x, y, '#');

		// Wait until 1/24th of second has passed
		QueryPerformanceCounter(&timestamp);
		if (last_timestamp.QuadPart + frame_ticks > timestamp.QuadPart) {
			// Calculate how many milliseconds we have to wait
			ULONGLONG wait_time = (last_timestamp.QuadPart + frame_ticks - timestamp.QuadPart) * 1000 / frequency.QuadPart;
			// Assert our confidence by using static_cast
			Sleep(static_cast<DWORD>(wait_time));
		}
		last_timestamp = timestamp;

		// Display the screen
		screen.show();
	}

	return 0;
}
