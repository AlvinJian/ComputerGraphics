#include "Scene.h"
#include "Node.h"
#include "Config.h"

using namespace scn;
using namespace utils;

SceneGraph::SceneGraph():
	pRootNode(nullptr), curModelMatrix(Angel::identity())
{
	auto rot = Angel::RotateX(0.0f);
	auto mov = Angel::Translate(0.0f, 0.0f, 0.0f);
	ground.setModelMatrix(mov * rot);
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
		curModelMatrix *= Angel::Translate(rootPos);
		pushModelMatrix();
		pRootNode->action(*this);
		popModelMatrix();

		ground.draw(*this);
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