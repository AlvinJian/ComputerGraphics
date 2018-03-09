#include "Scene.h"
#include "Node.h"
#include "Config.h"

using namespace assignment3;

Scene * Scene::CurrentScene = nullptr;

Angel::vec4 Scene::LightPosition = Angel::vec4(0.0f, 0.5f, 0.0f, 1.0f);
Angel::vec4 Scene::LightPositionEnd = Angel::vec4(0.0f, -1.0f, 0.0f, 1.0f);
float Scene::LightAngle = 120.0f;
float Scene::Shininess = 7.5f;

void Scene::Use(Scene * scn)
{
	CurrentScene = scn;
}

void Scene::Render()
{
	CurrentScene->render();
}

Scene::Scene():
	pRootNode(nullptr), curModelMatrix(Angel::identity())
{
}


Scene::~Scene()
{
}

void Scene::setRoot(Node * root, Angel::vec3 pos)
{
	pRootNode = root;
	rootPos = Angel::vec4(pos, 1.0f);
}

std::pair<const Node *, const Angel::vec4 *> Scene::getRoot() const
{
	return std::make_pair(pRootNode, &rootPos);
}

void Scene::render()
{
	if (pRootNode != nullptr)
	{
		// sets the default color to clear screen
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f); // background
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, 
			config::ViewportConfig::GetWidth(), 
			config::ViewportConfig::GetHeight()
		);
		curModelMatrix = Angel::identity();
		curModelMatrix *= Angel::Translate(rootPos);
		pushModelMatrix();
		pRootNode->action(*this);
		popModelMatrix();
		glutSwapBuffers();
	}
}

void Scene::pushModelMatrix()
{
	matrixStack.push(curModelMatrix);
}

void Scene::popModelMatrix()
{
	curModelMatrix = Angel::mat4( matrixStack.top() );
	matrixStack.pop();
}
