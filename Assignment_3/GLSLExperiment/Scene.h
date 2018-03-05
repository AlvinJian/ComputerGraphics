#pragma once
#include "Angel.h"
#include <stack>

namespace assignment3
{
	class Node;
	class Scene
	{
	public:
		Scene();
		~Scene();

		Angel::mat4 curModelMatrix;
		Angel::vec4 curColor;

		void pushModelMatrix();
		void popModelMatrix();
		void pushColorStack();
		void popColorStack();

		void setRoot(Node * root, Angel::vec3 pos);
		void startRender();

	private:
		std::stack<Angel::mat4> matrixStack;
		std::stack<Angel::vec4> colorStack;
		Node * pRootNode;
		Angel::vec4 rootPos;
	};
}
