#include "Angel.h"
#include "SierpinskiGasket.h"

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // single framebuffer, colors in RGB
	glutInitWindowSize(640, 480);                // Window size: 640 wide X 480 high
	glutInitWindowPosition(100, 150);               // Top left corner at (100, 150)
	glutCreateWindow("CS543 Computer Graphics");

	glewInit();

	assignment1::SierpinskiGasket::Draw();

	return 0;
}