#include <algorithm>
#include "ModelNode.h"
#include "Scene.h"
#include "Angel.h"
#include "Config.h"
#include "Utils.h"

using namespace scn;
using namespace common;
using color4 = Angel::vec4;

int ModelNode::ShadingMode = ModelNode::PER_PIXEL;

ModelNode::ModelNode(Ply & model, const Angel::vec4 & color):
	plyModel(model), vao(0), 
	vbo(0), ebo(0), 
	program(0), elementNum(0),
	color(color), shadowVao(0),
	shadowVbo(0), shadowEbo(0),
	shadowProgram(0)
{
}


ModelNode::~ModelNode()
{
}

void ModelNode::setup()
{
	const std::vector<Angel::vec4> colors(plyModel.getVertices().size(),
		color);
	const std::vector<point4>& points = plyModel.getVertices();
	const std::vector<Angel::vec3>& normals = plyModel.getNormals();
	const std::vector<GLuint>& elements = plyModel.getFlattenIndexesOfFaces();

	// Create a vertex array object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create vbo for points and colors
	size_t pointsByteSize = sizeof(point4) * points.size();
	size_t colorsByteSize = sizeof(color4) * colors.size();
	size_t normalsByteSize = sizeof(Angel::vec3) * normals.size();
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, pointsByteSize + colorsByteSize + normalsByteSize,
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, pointsByteSize,
		points.data());
	glBufferSubData(GL_ARRAY_BUFFER, pointsByteSize, colorsByteSize,
		colors.data());
	glBufferSubData(GL_ARRAY_BUFFER, pointsByteSize + colorsByteSize, normalsByteSize,
		normals.data());

	// Create ebo for faces
	size_t elemByteSize = sizeof(int) * elements.size();
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, elemByteSize, elements.data(),
		GL_STATIC_DRAW);
	elementNum = elements.size();

	// Load shaders and use the resulting shader program
	program = Angel::InitShader("vshader1.glsl", "fshader1.glsl");
	glUseProgram(program);
	// set up vertex arrays
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(pointsByteSize));

	GLuint vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(pointsByteSize + colorsByteSize));

	// disable all buffer and shader program
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glUseProgram(0);

	// setup shadow shader
	glGenVertexArrays(1, &shadowVao);
	glBindVertexArray(shadowVao);
	glGenBuffers(1, &shadowVbo);
	glBindBuffer(GL_ARRAY_BUFFER, shadowVbo);
	glBufferData(GL_ARRAY_BUFFER, pointsByteSize, points.data(), GL_STATIC_DRAW);
	glGenBuffers(1, &shadowEbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shadowEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, elemByteSize, elements.data(),
		GL_STATIC_DRAW);
	shadowProgram = Angel::InitShader("shadow_vs.glsl", "shadow_fs.glsl");
	glUseProgram(shadowProgram);
	GLuint vPos = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPos);
	glVertexAttribPointer(vPos, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	// disable all buffer and shader program
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glUseProgram(0);
}

void ModelNode::action(SceneGraph & scene)
{
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
	const Ply& m = getPlyModel();
	Angel::mat4 orthoMat = m.createOrthoMat();
	auto orthoMatT = Angel::transpose(orthoMat);
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

	// light attributes
	float lightPos[4] = { scene.LightPosition.x, scene.LightPosition.y,
		scene.LightPosition.z, scene.LightPosition.w };
	GLuint lightPositionLoc = glGetUniformLocation(program, "lightPosition");
	glUniform4fv(lightPositionLoc, 1, lightPos);

	Angel::vec4 lightDirection = scene.LightPositionEnd - scene.LightPosition;
	float lightDir[3] = { lightDirection.x, lightDirection.y, lightDirection.z };
	GLuint lightDirectionLoc = glGetUniformLocation(program, "lightDirection");
	glUniform3fv(lightDirectionLoc, 1, lightDir);

	GLuint lightAngleLoc = glGetUniformLocation(program, "lightAngle");
	glUniform1f(lightAngleLoc, scene.LightAngle);

	color4 light_ambient(0.2, 0.2, 0.2, 1.0);
	color4 light_diffuse(0.5, 0.5, 0.5, 1.0);
	color4 light_specular(1.0, 1.0, 1.0, 1.0);

	color4 material_ambient = color;
	color4 material_diffuse = color;
	color4 material_specular = color;
	float  material_shininess = scene.Shininess;

	color4 ambient_product = light_ambient * material_ambient;
	color4 diffuse_product = light_diffuse * material_diffuse;
	color4 specular_product = light_specular * material_specular;

	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 
		1, ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"),
		1, diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"),
		1, specular_product);
	glUniform1f(glGetUniformLocation(program, "Shininess"),
		material_shininess);
	glUniform1i(glGetUniformLocation(program, "shadingMode"), 
		ModelNode::ShadingMode);
	glUniform1i(glGetUniformLocation(program, "skybox"), 0);
	glUniform1i(glGetUniformLocation(program, "skyboxMode"), 
		scene.background.currentMode);

	// drawing
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);
	glDrawElements(GL_TRIANGLES, elementNum, GL_UNSIGNED_INT, 0);

	// disable all buffer and shader program
	glBindVertexArray(0);
	// glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glUseProgram(0);

	if (scene.shadowToggle)
	{
		// draw shadow
		glBindVertexArray(shadowVao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shadowEbo);
		glUseProgram(shadowProgram);

		modelMatrixLoc = glGetUniformLocationARB(shadowProgram, "modelMatrix");
		glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, modelMatrixf.data());
		viewMatrixLoc = glGetUniformLocationARB(shadowProgram, "viewMatrix");
		glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, viewMatrixf.data());
		projMatrixLoc = glGetUniformLocationARB(shadowProgram, "projectionMatrix");
		glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, projMatrixf.data());
		orthMatrixLoc = glGetUniformLocationARB(shadowProgram, "orthoMatrix");
		glUniformMatrix4fv(orthMatrixLoc, 1, GL_FALSE, orthMatf.data());

		// light attributes
		float _lightPos[4] = { scene.LightPosition.x, scene.LightPosition.y,
			scene.LightPosition.z, scene.LightPosition.w };
		lightPositionLoc = glGetUniformLocation(shadowProgram, "lightPosition");
		glUniform4fv(lightPositionLoc, 1, _lightPos);

		glUniform1f(glGetUniformLocation(shadowProgram, "ground"), scene.groundLevel);

		glDrawElements(GL_TRIANGLES, elementNum, GL_UNSIGNED_INT, 0);

		// disable all buffer and shader program
		glBindVertexArray(0);
		// glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glUseProgram(0);
	}

	glDisable(GL_DEPTH_TEST);
	glFlush();
}
