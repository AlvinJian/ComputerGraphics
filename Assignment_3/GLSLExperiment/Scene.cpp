#include "Scene.h"
#include "Node.h"

using namespace assignment3;

Scene::Scene():
	pRootNode(nullptr), curModelMatrix(Angel::identity()),
	curColor(Angel::vec4(1.0, 0.0, 0.0, 1.0))
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

void Scene::startRender()
{
	if (pRootNode != nullptr)
	{
		curModelMatrix = Angel::identity();
		curModelMatrix *= Angel::Translate(rootPos);
		pushModelMatrix();
		pushColorStack();
		pRootNode->action(*this);
		popModelMatrix();
		popColorStack();
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

void Scene::pushColorStack()
{
	colorStack.push(curColor);
}

void Scene::popColorStack()
{
	curColor = Angel::vec4(colorStack.top());
	colorStack.pop();
}