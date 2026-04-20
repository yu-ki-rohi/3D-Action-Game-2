#pragma once

#include "../Common.h"

#include <memory>
#include <source_location>

struct DebugLog
{
	enum class Type
	{
		Message,
		Warning,
		Error
	};

	Type LogType;
	const char* Message;
	const char* FileName;
	const int Line;
	const char* FuncName;

	DebugLog(DebugLog::Type type_, const char* message_, const char* file_name_, int line_, const char* func_name_)
		:
		LogType(type_),
		Message(message_),
		FileName(file_name_),
		Line(line_),
		FuncName(func_name_)
	{

	}

	~DebugLog()
	{
		delete[](Message);
		delete[](FileName);
		delete[](FuncName);
	}

};

class DebugLogger
{
public:
	DebugLogger();

public:
	void AddDebugLog(DebugLog::Type type_, const char* message_, const std::source_location& loc);

	void Render();

private:
	// Chat GPTÇégópÇµÇƒê∂ê¨
	// ï∂éöóÒÇï ÇÃÉÅÉÇÉäÇ…ï°êª
	char* DuplicateString(const char* src_);

	void RenderLog(int num_, const DebugLog& log_);

private:
	static constexpr int maxNum = 23;
	static constexpr int messageLeft = 500;
	static constexpr int line = 20;
	static constexpr int padding = 5;
	static constexpr int space = 3;
	static constexpr int widthOnSimple = 300;
	static constexpr int up = 10;
	static constexpr int leftOnSimple = WindowSettings::WindowWidth - widthOnSimple;
	static constexpr int leftOnDetail = 200;
	static constexpr int widthOnDetail = WindowSettings::DefaultWidth - leftOnDetail * 2;

private:
	const unsigned int messageColor;
	const unsigned int warningColor;
	const unsigned int errorColor;
	const unsigned int characterColor;

	int nextIndex = 0;

	std::unique_ptr<DebugLog> logs[maxNum]{};
};
