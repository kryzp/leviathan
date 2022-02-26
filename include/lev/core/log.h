#pragma once

namespace Log
{
	bool init();
	void destroy();

	void print(const char* fmt, ...);
	void error(const char* fmt, ...);
	void warn(const char* fmt, ...);

	void file(const char* type, const char *msg, ...);
	void filenewline();

	void directory(const char* dir);
	const char* directory();
}
