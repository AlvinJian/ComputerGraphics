#include "Angel.h"
#include "SierpinskiGasket.h"
#include "GRS.h"
#include "Config.h"

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // single framebuffer, colors in RGB
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);                // Window size: 640 wide X 480 high
	glutInitWindowPosition(WINDOW_POS_X, WINDOW_POS_Y);               // Top left corner at (100, 150)
	glutCreateWindow("CS543 Computer Graphics");

	glewInit();

	//assignment1::SierpinskiGasket::Draw();
	std::string f = "dragon.dat";
	assignment1::GRSData::Draw(f);

	return 0;
}