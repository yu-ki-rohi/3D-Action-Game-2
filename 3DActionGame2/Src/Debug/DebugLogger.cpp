#include "DebugLogger.h"
#include <cstring>
#include "DxLib.h"

DebugLogger::DebugLogger() :
    messageColor(GetColor( 0, 255, 0)),
    warningColor(GetColor(255, 255, 0)),
    errorColor(GetColor(255, 0, 0)),
    characterColor(GetColor(0, 0, 0))
{

}
void DebugLogger::AddDebugLog(DebugLog::Type type_, const char* message_, const std::source_location& loc_)
{
    logs[nextIndex] = std::make_unique<DebugLog>(type_, DuplicateString(message_), DuplicateString(loc_.file_name()), loc_.line(), DuplicateString(loc_.function_name()));
    ++nextIndex;
    if (nextIndex >= maxNum)
    {
        nextIndex = 0;
    }
}

char* DebugLogger::DuplicateString(const char* src)
{
    if (src == nullptr) { return nullptr; }

    size_t len = std::strlen(src) + 1;
    char* dst = new char[len];
    strcpy_s(dst, len, src);

    return dst;
}

void DebugLogger::Render()
{
    int num = 0;

    for (int i = nextIndex - 1; i >= 0; --i)
    {
        if (logs[i] == nullptr) { continue; }
        RenderLog(num++, *logs[i]);
    }

    for (int i = maxNum - 1; i >= nextIndex; --i)
    {
        if (logs[i] == nullptr) { break; }
        RenderLog(num++, *logs[i]);
    }
}

void DebugLogger::RenderLog(int num_, const DebugLog& log_)
{
    int color = 0;
    switch (log_.LogType)
    {
    case DebugLog::Type::Message:
        color = messageColor;
        break;
    case DebugLog::Type::Warning:
        color = warningColor;
        break;
    case DebugLog::Type::Error:
        color = errorColor;
        break;
    }

    if (CheckHitKey(KEY_INPUT_L))
    {
        int height = line * 2 + padding;
        int this_up = up + (height + space) * num_;
        DrawBox(leftOnDetail, this_up, leftOnDetail + widthOnDetail, this_up + height, color, TRUE);
        DrawFormatString(leftOnDetail + padding, this_up + padding, characterColor, "%s ( %d ) : %s", log_.FileName, log_.Line, log_.FuncName);
        DrawString(leftOnDetail + padding, this_up + padding + line, log_.Message, characterColor);
    }
    else
    {
        int height = line + padding;
        int this_up = up + (height + space) * num_;
        DrawBox(leftOnSimple, this_up, leftOnSimple + widthOnSimple, this_up + height, color, TRUE);
        DrawString(leftOnSimple + padding, this_up + padding, log_.Message, characterColor);
    }
}