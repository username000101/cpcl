# CPCL - C++ library for plugins and marshaling

## Example
```c++
#include <cpcl/dynlib.hpp>
#include <cpcl/marshaling.hpp>

int main() {
	auto dll = cpcl::dynlib::dynamic_library_handle::open_library("path/to/library"); // You don't reed to speicfy extension, but You can!
	if (!dll) {
		// Handle library opening error. CPCL currently doesn't provides error messages/something other
	}

	auto process_fn = dll->get_symbol<void(*)(safety_vector_wrapper*)>("process");
	if (!process_fn) {
		// Handle symbol getting error. CPCL currently doesn't provides error messages/something other
	}
	
	std::string a("Hello"), b("World");
	std::vector strings = { cpcl::marshaling::make_safe(a), cpcl::marshaling::make_safe(b) };
	auto safety_strings = cpcl::marshaling::make_safe<safety_string_wrapper>(strings);
	process_fn.value()(&safety_strings);
}
```
Then.. on the other side
```c++
#include <iostream>

#include <cpcl/marshaling.hpp>

void process(safety_vector_wrapper* strings) {
  	for (const auto& string : cpcl::marshaling::from_safe<safety_string_wrapper>(*strings))
		  std::cout << cpcl::marshaling::from_safe(string) << std::endl;
}
```
So.. what we just did?
We create ABI-safety array of ABI-safety strings

To put it simply:

std::string ==> safety_string_wrapper { char* data_ptr, size_t size }

std::vector<safety_string_wrapper> ==> safety_vector_wrapper { void* data_ptr, size_t element_size, size_t elements }

But safety_vector_wrapper ==> only in std::span, and safety_string_wrapper ==> only in std::string_view because we don't touch ownership. If you want to change data - just copy
