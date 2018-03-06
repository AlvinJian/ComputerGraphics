#pragma once
#include "Angel.h"
#include <stack>

namespace assignment3
{
	class Node;
	class Scene
	{
	public:
		static void Use(Scene *);
		static void Render();

		Scene();
		~Scene();

		Angel::mat4 curModelMatrix;
		Angel::vec4 curColor;

		void pushModelMatrix();
		void popModelMatrix();

		void setRoot(Node * root, Angel::vec3 pos);

	private:
		static Scene * CurrentScene;

		void render();

		std::stack<Angel::mat4> matrixStack;
		Node * pRootNode;
		Angel::vec4 rootPos;
	};
}
