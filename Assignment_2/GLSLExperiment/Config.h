#pragma once

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 640

#define WINDOW_POS_X 150
#define WINDOW_POS_Y 150

namespace config {
	class ViewportConfig
	{
	public:
		static void Init();
		static const ViewportConfig * getInstance();
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
		static ViewportConfig * Instance();

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