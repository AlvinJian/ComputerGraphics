#pragma once
#include <stack>
#include <utility>
#include "Angel.h"
#include "Camera.h"
#include "SingleUsage.h"

namespace assignment3
{
	class Node;
	class SceneGraph
	{
	public:
		// Light
		static Angel::vec4 LightPosition;
		static Angel::vec4 LightPositionEnd;
		static float LightAngle;
		static float Shininess;

		SceneGraph();
		~SceneGraph();

		Angel::mat4 curModelMatrix;
		Angel::vec4 curColor;
		Camera camera;

		void render();
		void pushModelMatrix();
		void popModelMatrix();

		virtual void setRoot(Node * root, Angel::vec3 pos);
		std::pair<const Node *, const Angel::vec4 *>
			getRoot() const;

	protected:
		std::stack<Angel::mat4> matrixStack;
		Node * pRootNode;
		Angel::vec4 rootPos;
	};

	class Scene : public common::SingleUsage<SceneGraph>
	{
	public:
		static void Render();

		Scene();
		~Scene();
	};
}
