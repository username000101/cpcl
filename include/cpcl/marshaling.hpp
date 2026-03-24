#pragma once

#include <type_traits>

#include "cpcl/marshaling/string.hpp"
#include "cpcl/marshaling/vector.hpp"

namespace cpcl::marshaling {
	template <typename vector_element_t = nullptr_t>
	auto make_safe(auto& value) {
		if constexpr (std::is_same_v<std::decay_t<decltype(value)>, std::string>) return safety_string_wrapper{.data_ptr = value.data(), .size = value.size()};
		if constexpr (std::is_same_v<std::decay_t<decltype(value)>, std::vector<vector_element_t>>) return safety_vector_wrapper{.data_ptr = value.data(), .element_size = sizeof(vector_element_t), .elements = value.size()};
	}

	template <typename vector_element_t = nullptr_t>
	auto from_safe(auto& value) {
		if constexpr (std::is_same_v<std::decay_t<decltype(value)>, safety_string_wrapper>) return std::string_view(value.data_ptr, value.size);
		if constexpr (std::is_same_v<std::decay_t<decltype(value)>, safety_vector_wrapper>) return std::span<vector_element_t>(static_cast<vector_element_t*>(value.data_ptr), value.elements);
	}
} // namespace cpcl::marshaling