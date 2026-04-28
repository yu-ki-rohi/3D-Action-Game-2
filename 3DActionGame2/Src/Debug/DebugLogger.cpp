#include "DebugLogger.h"
#include "DxLib.h"

#include "DebugMessenger.h"

DebugLogger::DebugLogger() :
    messageColor(GetColor( 0, 255, 0)),
    warningColor(GetColor(255, 255, 0)),
    errorColor(GetColor(255, 0, 0)),
    characterColor(GetColor(0, 0, 0))
{

}

void DebugLogger::AddDebugLog(DebugLog::Type type_, const char* message_, const std::source_location& loc_)
{
    logs[nextIndex] = std::make_unique<DebugLog>(type_, message_, loc_.file_name(), loc_.line(), loc_.function_name());
    ++nextIndex;
    if (nextIndex >= maxNum)
    {
        nextIndex = 0;
    }
}


void DebugLogger::Update(float elapsed_time_)
{
    int currentLKeyState = CheckHitKey(KEY_INPUT_L);
    if (currentLKeyState == 1 && previousLKyeState == 0)
    {
        state = (State)((state + 1) % State::Max);
        scroll = 0;
    }
    previousLKyeState = currentLKeyState;

    if (state == State::Detail)
    {
        if (CheckHitKey(KEY_INPUT_UP))
        {
            int new_scroll = scroll + scrollSpeed * elapsed_time_;
            int scroll_max = defaultUp + (line * 2 + padding + space) * (maxNum + 1) - WindowSettings::WindowHeight;
            scroll_max <<= pointPosition;
            scroll = (new_scroll < scroll_max) ? new_scroll : scroll_max;
        }
        else if (CheckHitKey(KEY_INPUT_DOWN))
        {
            int new_scroll = scroll - scrollSpeed * elapsed_time_;
            int scroll_min = 0;
            scroll_min <<= pointPosition;
            scroll = (scroll_min < new_scroll) ? new_scroll : scroll_min;
        }
    }
}

void DebugLogger::Render()
{
    if (state == State::Hidden) { return; }
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
   
    if (state == State::Detail)
    {
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
        int height = line * 2 + padding;
        int this_up = defaultUp - (scroll >> pointPosition) + (height + space) * num_;
        if (this_up > WindowSettings::WindowHeight ||
            this_up + height < 0) { return; }
        DrawBox(leftOnDetail, this_up, leftOnDetail + widthOnDetail, this_up + height, color, TRUE);
        DrawFormatString(leftOnDetail + padding, this_up + padding, characterColor, "%s ( %d ) : %s", log_.FileName.c_str(), log_.Line, log_.FuncName.c_str());
        DrawString(leftOnDetail + padding, this_up + padding + line, log_.Message.c_str(), characterColor);
    }
    else if(state == State::Simple)
    {
        switch (log_.LogType)
        {
        case DebugLog::Type::Message:
            color = GetColor(255, 255, 255);
            break;
        case DebugLog::Type::Warning:
            color = warningColor;
            break;
        case DebugLog::Type::Error:
            color = errorColor;
            break;
        }
        int height = line + padding;
        int this_up = defaultUp + (height + space) * num_;
        if (this_up > WindowSettings::WindowHeight) { return; }
        DrawString(leftOnSimple + padding, this_up + padding, log_.Message.c_str(), color);
    }
}