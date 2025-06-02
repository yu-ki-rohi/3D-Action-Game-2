#pragma once

#define DEBUG

class WindowSettings
{
public:
	static float constexpr Magnification = 0.8f;
	static int constexpr WindowWidth = (int)(1920 * Magnification);
	static int constexpr WindowHeight = (int)(1080 * Magnification);
};