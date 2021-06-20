#pragma once

#ifndef DIAGNOSTICS
#define DIAGNOSTICS

#include <iostream>
#include <sstream>
#include <format>

namespace Diagnostics { 

	template<class... Args>
	inline auto Throw(const char* fstr, Args... args) -> void {
		std::ostringstream out;
		out << std::format(fstr, args...);
		std::cerr << out.str() << std::endl;
		throw new std::exception(out.str().c_str());
	}
	
	inline auto ErrorCallback(int code, const char* description) -> void {
		Throw("GLFW threw an error (code {}): {}", code, description);
	}

	template<class... Args>
	inline auto Log(const char* fstr, Args... args) noexcept -> void {
		std::ostringstream out;
		out << std::format(fstr, args...);
		std::cout << out.str() << std::endl;
	}
};

#endif