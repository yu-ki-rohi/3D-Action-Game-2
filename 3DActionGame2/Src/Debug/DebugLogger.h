#pragma once

#include "../Common.h"

#include <memory>
#include <source_location>
#include <string>

struct DebugLog
{
	enum class Type
	{
		Message,
		Warning,
		Error
	};

	Type LogType;
	std::string Message;
	std::string FileName;
	const int Line;
	std::string FuncName;

	DebugLog(DebugLog::Type type_, const char* message_, const char* file_name_, int line_, const char* func_name_)
		:
		LogType(type_),
		Message(message_ ? message_ : ""),
		FileName(file_name_ ? file_name_ : ""),
		Line(line_),
		FuncName(func_name_ ? func_name_ : "")
	{

	}

	~DebugLog() = default;

};

class DebugLogger
{
public:
	DebugLogger();

public:
	void AddDebugLog(DebugLog::Type type_, const char* message_, const std::source_location& loc);

	void Update(float elapsed_time_);
	void Render();

private:
	enum State
	{
		Hidden,
		Simple,
		Detail,
		Max
	};

private:
	void RenderLog(int num_, const DebugLog& log_);

private:
	static constexpr int maxNum = 100;
	static constexpr int messageLeft = 500;
	static constexpr int line = 20;
	static constexpr int padding = 5;
	static constexpr int space = 3;
	static constexpr int widthOnSimple = 300;
	static constexpr int defaultUp = 10;
	static constexpr int leftOnSimple = WindowSettings::WindowWidth - widthOnSimple;
	static constexpr int leftOnDetail = 200;
	static constexpr int widthOnDetail = WindowSettings::DefaultWidth - leftOnDetail * 2;

	static constexpr int pointPosition = 8;
	static constexpr int scrollSpeed = 1500 << pointPosition;

private:
	const unsigned int messageColor;
	const unsigned int warningColor;
	const unsigned int errorColor;
	const unsigned int characterColor;

	int nextIndex = 0;
	// ŒÅ’è¬”“_
	int scroll = 0 << pointPosition;

	State state = State::Simple;
	int previousLKyeState = 0;

	std::unique_ptr<DebugLog> logs[maxNum]{};
};
