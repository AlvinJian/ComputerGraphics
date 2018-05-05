#include "Scene.h"
#include "Node.h"
#include "Config.h"

using namespace scn;
using namespace utils;

SceneGraph::SceneGraph() :
	pRootNode(nullptr), curModelMatrix(Angel::identity()),
	groundLevel(0.0f), shadowToggle(true)
{
	
	background.loadCubemap();
	background.loadEnvMap();
	background.genPlainCube();
}

SceneGraph::~SceneGraph()
{
}

void SceneGraph::setRoot(Node * root, Angel::vec3 pos)
{
	pRootNode = root;
	rootPos = Angel::vec4(pos, 1.0f);
}

std::pair<const Node *, const Angel::vec4 *> SceneGraph::getRoot() const
{
	return std::make_pair(pRootNode, &rootPos);
}

void SceneGraph::render()
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

void SceneGraph::pushModelMatrix()
{
	matrixStack.push(curModelMatrix);
}

void SceneGraph::popModelMatrix()
{
	curModelMatrix = Angel::mat4( matrixStack.top() );
	matrixStack.pop();
}

SceneGraph * SingleUsage<SceneGraph>::InUse = nullptr;

void Scene::Render()
{
	if (InUse != nullptr)
	{
		InUse->render();
	}
}

Scene::Scene():
	SceneGraph()
{
}

Scene::~Scene()
{
}