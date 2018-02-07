#pragma once

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 640

#define WINDOW_POS_X 150
#define WINDOW_POS_Y 150

#define DEFAULT_KOCH_ITER 3
#define BASE_KOCH_ITER 0
#define MAX_KOCH_ITER 6

#define TILING_NUM 4

namespace assignment1 {
	class ViewportConfig
	{
	public:
		static void Init();
		static void SetSize(int w, int h);
		static void SetPos(int x, int y);
		static int GetWidth();
		static int GetHeight();
		static int GetPosX();
		static int GetPosY();
		~ViewportConfig()
		{
		}

	private:
		ViewportConfig():
			width(WINDOW_WIDTH),
			height(WINDOW_HEIGHT),
			x(0), y(0)
		{
		}

		int width;
		int height;
		int x, y;
	};
}