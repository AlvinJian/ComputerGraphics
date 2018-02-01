#include "Angel.h"
#include "SierpinskiGasket.h"

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // single framebuffer, colors in RGB
	glutInitWindowSize(640, 480);                // Window size: 640 wide X 480 high
	glutInitWindowPosition(100, 150);               // Top left corner at (100, 150)
	glutCreateWindow("CS543 Computer Graphics");

	glewInit();

	assignment1::SierpinskiGasket gasket;
	gasket.generateTriangles(0.08f);
	auto points = gasket.getVertexData();

	// init GPU buffer
	// Create a vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and initialize a buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Angel::vec2) * points.size(), 
		points.data(), GL_STATIC_DRAW);

	// setup shader
	// Load shaders and use the resulting shader program
	auto program = Angel::InitShader("vshader1.glsl", "fshader1.glsl");
	glUseProgram(program);

	// Initialize the vertex position attribute from the vertex shader
	GLuint loc = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glClearColor(1.0, 1.0, 1.0, 1.0);        // sets white as color used to clear screen

	
	static int numOfPts = points.size();
	auto display = [](void) {
		// All drawing happens in display function
		glClear(GL_COLOR_BUFFER_BIT);                // clear window
		glDrawArrays(GL_TRIANGLES, 0, numOfPts);    // draw the points
		glFlush();										// force output to graphics hardware
		return;
	};

	auto keyboard = [](unsigned char key, int x, int y) {
		// keyboard handler
		switch (key) {
		case 033:			// 033 is Escape key octal value
			exit(1);		// quit program
			break;
		}
	};

	glutDisplayFunc(display); // Register display callback function
	glutKeyboardFunc(keyboard); // Register keyboard callback function

	// enter the drawing loop
	glutMainLoop();

	return 0;
}