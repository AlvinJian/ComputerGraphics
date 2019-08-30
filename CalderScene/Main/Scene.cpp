#include "Scene.h"
#include "Node.h"
#include "Config.h"

using namespace scn;
using namespace utils;

Scene::Scene() :
	pRootNode(nullptr), curModelMatrix(Angel::identity()),
	groundLevel(0.0f), shadowToggle(true)
{
	
	background.loadCubemap();
	background.loadEnvMap();
	background.genPlainCube();
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
	if (InUse != this) return;
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
		pushModelMatrix();
		background.draw(*this);
		
		background.bindEnvMap(true);

		curModelMatrix *= Angel::Translate(rootPos);
		pRootNode->action(*this);
		popModelMatrix();

		background.bindEnvMap(false);

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

Scene * SingleUsage<Scene>::InUse = nullptr;

void Scene::Render()
{
	if (InUse != nullptr)
	{
		InUse->render();
	}
}
