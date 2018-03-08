#pragma once
#include "Angel.h"
#include <stack>
#include <utility>

namespace assignment3
{
	class Node;
	class Scene
	{
	public:
		static void Use(Scene *);
		static void Render();

		// Light
		static Angel::vec4 LightPosition;
		static Angel::vec4 LightPositionEnd;
		static float LightAngle;
		static float Shininess;

		Scene();
		~Scene();

		Angel::mat4 curModelMatrix;
		Angel::vec4 curColor;

		void pushModelMatrix();
		void popModelMatrix();

		void setRoot(Node * root, Angel::vec3 pos);
		std::pair<const Node *, const Angel::vec4 *>
			getRoot() const;

	private:
		static Scene * CurrentScene;

		void render();

		std::stack<Angel::mat4> matrixStack;
		Node * pRootNode;
		Angel::vec4 rootPos;
	};
}
