#pragma once

#define DEBUG

class WindowSettings
{
public:
	static float constexpr Magnification = 1.0f;
	static int constexpr DefaultWidth = 1920;
	static int constexpr DefaultHeight = 1080;
	static int constexpr WindowWidth = (int)(DefaultWidth * Magnification);
	static int constexpr WindowHeight = (int)(DefaultHeight * Magnification);
};