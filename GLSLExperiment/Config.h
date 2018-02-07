#pragma once

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 640

#define WINDOW_POS_X 150
#define WINDOW_POS_Y 150

#define DEFAULT_KOCH_ITER 3

#define TILING_NUM 4

namespace assignment1 {
	class ViewportConfig
	{
	public:
		static void Init();
		static void SetSize(int w, int h);
		static int GetWidth();
		static int GetHeight();
		~ViewportConfig()
		{
		}

	private:
		ViewportConfig():
			width(WINDOW_WIDTH),
			height(WINDOW_HEIGHT)
		{
		}

		int width;
		int height;
	};
}