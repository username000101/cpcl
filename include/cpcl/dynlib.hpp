#pragma once

#ifdef _WIN32
#include <windows.h>
#elifdef __linux__
#include <dlfcn.h>
#endif

#include <expected>
#include <filesystem>
#include <optional>
#include <string>

namespace cpcl::dynlib {
	class dynamic_library_handle {
		void* native_handle;

		dynamic_library_handle(void* native_handle) : native_handle(native_handle) {}

	public:
		~dynamic_library_handle() {
			if (this->native_handle)
#ifdef _WIN32
				FreeLibrary(static_cast<HMODULE>(this->native_handle);
#elifdef __linux__
				dlclose(this->native_module);
#endif
		}

		static std::expected<dynamic_library_handle, std::errc> open_library(std::string&& filename) {
			if (!std::filesystem::exists(filename)) {
#ifdef _WIN32
				if (!filename.ends_with(".dll")) filename.append(".dll");
#elifdef __linux__
				if (!filename.ends_with(".so")) filename.append(".so");
#endif
				if (!std::filesystem::exists(filename)) return std::unexpected(std::errc::no_such_file_or_directory);
			}

#ifdef _WIN32
			return dynamic_library_handle(LoadLibrary(filename.c_str()));
#elifdef __linux__
			return dynamic_library_handle(dlopen(filename.c_str(), RTLD_LAZY));
#endif
		}

		void close() {
			if (this->native_handle)
#ifdef _WIN32
				FreeLibrary(static_cast<HMODULE>(this->native_handle));
#elifdef __linux__
				dlclose(this->native_handle);
#endif
		}

		template <typename symbol_signature_t>
		std::optional<symbol_signature_t> get_symbol(const std::string& name) {
			if (!this->native_handle) return std::nullopt;
#ifdef _WIN32
			if (auto symbol = reinterpret_cast<symbol_signature_t>(GetProcAddress(static_cast<HMODULE>(this->native_handle), name.c_str())); symbol) return symbol;
#elifdef __linux__
			if (auto symbol = reinterpret_cast<symbol_signature_t>(dlsym(this->native_handle, name.c_str())); symbol) return symbol;
#endif
			return std::nullopt;
		}
	};

} // namespace cpcl::dynlib
