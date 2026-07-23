#pragma once

#include <filesystem>
#include <vector>

#if defined(_WIN32)
#include <windows.h>
#endif

namespace Referencer {

	inline std::filesystem::path resourcePath(const std::filesystem::path& relativePath)
	{
		std::vector<std::filesystem::path> roots = {
			std::filesystem::current_path() / "Referencer" / "resources",
			std::filesystem::current_path() / "resources",
			std::filesystem::path("Referencer") / "resources",
			std::filesystem::path("resources")
		};

#if defined(_WIN32)
		std::vector<wchar_t> executablePathBuffer(32768);
		const DWORD pathLength = GetModuleFileNameW(
			nullptr,
			executablePathBuffer.data(),
			static_cast<DWORD>(executablePathBuffer.size()));
		if (pathLength > 0 && pathLength < executablePathBuffer.size())
		{
			const std::filesystem::path executablePath(
				std::wstring(executablePathBuffer.data(), pathLength));
			roots.insert(roots.begin(), executablePath.parent_path() / "resources");
		}
#elif defined(__linux__)
		std::error_code error;
		const auto executablePath = std::filesystem::read_symlink("/proc/self/exe", error);
		if (!error)
			roots.insert(roots.begin(), executablePath.parent_path() / "resources");
#endif

		for (const auto& root : roots)
		{
			const auto candidate = root / relativePath;
			if (std::filesystem::exists(candidate))
				return candidate;
		}

		return std::filesystem::path("Referencer") / "resources" / relativePath;
	}

}
