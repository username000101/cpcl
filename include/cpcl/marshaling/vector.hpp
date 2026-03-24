#pragma once

#include <span>
#include <vector>

extern "C" struct safety_vector_wrapper {
	void* data_ptr = nullptr;
	size_t element_size = 0, elements = 0;
};