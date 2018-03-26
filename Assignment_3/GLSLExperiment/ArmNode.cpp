#include <vector>
#include "Scene.h"
#include "Config.h"
#include "ArmNode.h"

using namespace assignment3;

ArmNode::ArmNode(const Angel::vec3 & directedShape,
	TransformNode::Side s):
	TransformNode(Angel::Translate(directedShape), s),
	directedShapeVec(directedShape),
	vao(0), vbo(0), program(0), sinuAnimator(nullptr)
{
	armMat = Angel::mat4(TransformNode::transformMat);
}


ArmNode::~ArmNode()
{
}

void ArmNode::action(SceneGraph & scene)
{
	auto num = setupArms();
	Angel::mat4 modelMat(scene.curModelMatrix);
	// projection matrix
	GLfloat ratio = (GLfloat)config::ViewportConfig::GetWidth() /
		(GLfloat)config::ViewportConfig::GetHeight();
	Angel::mat4 perspectiveMat = Angel::Perspective((GLfloat)45.0f, ratio, (
		GLfloat)0.1, (GLfloat) 100.0);
	auto perspectiveMatT = Angel::transpose(perspectiveMat);
	std::vector<float> projMatrixf = utils::FlattenMat4(perspectiveMatT);

	// view matrix
	Angel::mat4 viewMatrix = scene.camera.createViewMat();
	auto viewMatrixT = Angel::transpose(viewMatrix);
	std::vector<float> viewMatrixf = utils::FlattenMat4(viewMatrixT);

	// model matrix
	auto modelMatrixT = Angel::transpose(modelMat);
	std::vector<float> modelMatrixf = utils::FlattenMat4(modelMatrixT);

	// ortho matrix
	auto orthoMatT = Angel::transpose(Angel::identity());
	std::vector<float> orthMatf = utils::FlattenMat4(orthoMatT);

	glBindVertexArray(vao);
	// glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glUseProgram(program);

	GLuint modelMatrixLoc = glGetUniformLocationARB(program, "modelMatrix");
	glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, modelMatrixf.data());
	GLuint viewMatrixLoc = glGetUniformLocationARB(program, "viewMatrix");
	glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, viewMatrixf.data());
	GLuint projMatrixLoc = glGetUniformLocationARB(program, "projectionMatrix");
	glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, projMatrixf.data());
	GLuint orthMatrixLoc = glGetUniformLocationARB(program, "orthoMatrix");
	glUniformMatrix4fv(orthMatrixLoc, 1, GL_FALSE, orthMatf.data());

	glEnable(GL_DEPTH_TEST);
	glDrawArrays(GL_LINE_STRIP, 0, num);
	glDisable(GL_DEPTH_TEST);
	glFlush();
	// disable all buffer and shader program
	glBindVertexArray(0);
	// glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(0);

	// glDeleteBuffers(1, &vbo);
	// glDeleteVertexArrays(1, &vao);

	// append new matrix to CTM
	TransformNode::action(scene);
}

void ArmNode::linkSinusoidAnimator(const SinusoidAnimator * animator)
{
	sinuAnimator = animator;
}

size_t ArmNode::setupArms()
{
	std::vector<Angel::vec4> arms(4, Angel::vec4(Angel::vec3()));
	arms[1].y = directedShapeVec.y;
	arms[2].y = directedShapeVec.y;
	arms[2].x = directedShapeVec.x;
	arms[3].x = directedShapeVec.x;
	arms[3].y = directedShapeVec.y;
	arms[3].z = directedShapeVec.z;

	if (sinuAnimator != nullptr)
	{
		auto movement = sinuAnimator->getMovement();
		// TODO hardcode
		if (movement.first == TransformNode::Y_AXIS)
		{
			arms[3].y += movement.second;
			arms[2].y += movement.second;
			Angel::mat4 sinuMat = Angel::Translate(0.0f, movement.second, 0.0f);
			transformMat = sinuMat * armMat;
		}
	}

	std::vector<Angel::vec4> colors(4, 
		Angel::vec4(1.0f, 0.0f, 0.0f, 1.0f));

	// Create a vertex array object
	if (vao == 0)
	{
		glGenVertexArrays(1, &vao);
	}
	glBindVertexArray(vao);

	// create buffer
	size_t armBufSize = arms.size() * sizeof(Angel::vec4);
	size_t colorsBufSize = colors.size() * sizeof(Angel::vec4);
	size_t bufSize = armBufSize + colorsBufSize;
	if (vbo == 0)
	{
		glGenBuffers(1, &vbo);
	}
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, bufSize, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, armBufSize, arms.data());
	glBufferSubData(GL_ARRAY_BUFFER, armBufSize, colorsBufSize, colors.data());

	if (program == 0)
	{
		// shader program
		program = Angel::InitShader("skel_vshader.glsl", "skel_fshader.glsl");
	}
	glUseProgram(program);

	// set up vertex arrays
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));
	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(armBufSize));

	return arms.size();
}
