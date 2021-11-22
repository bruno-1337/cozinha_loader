#pragma once

#include <functional>

#include <BlackBone/Process/Process.h>

const auto failure = []( std::string_view str_err, const std::pair<HANDLE, HANDLE> handles = {} ) -> bool
{
	const auto [hProcess, hThread] = handles;

	if ( hProcess ) // hProcess
		CloseHandle( hProcess );

	if ( hThread ) // hThread
		CloseHandle( hThread );

	log_err( "%s", str_err );

	return false;
};

const auto get_system_directory = []() -> std::wstring
{
	wchar_t buf[MAX_PATH];
	GetSystemDirectory( buf, sizeof( buf ) / 4 );

	return std::wstring( buf );
};

class c_injector
{
private:
	// Manual maps buffer into process
	bool map( std::string_view str_proc, std::wstring_view wstr_mod_name, std::vector<std::uint8_t> vec_buffer, blackbone::eLoadFlags flags = blackbone::WipeHeader );

	// Close an array of processes
	void close_processes( std::vector<std::string_view> vec_processes );

	// List of AppIDs and process names
	const std::vector<std::pair<int, std::string_view>> vec_app_ids = {
		{ 730, "csgo.exe" } // Counter-Strike: Global Offensive
	};

public:
	c_injector() = default;
	~c_injector() = default;

	// Initialize routine
	bool initiaze( std::string_view str_proc_name, const std::filesystem::path dll_path );
};

inline auto g_injector = std::make_unique<c_injector>();