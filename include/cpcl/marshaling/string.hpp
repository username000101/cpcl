#pragma once

#include <string>

extern "C" struct safety_string_wrapper {
	char* data_ptr = nullptr;
	size_t size = 0;
};