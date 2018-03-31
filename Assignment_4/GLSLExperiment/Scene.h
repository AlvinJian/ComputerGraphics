#pragma once
#include <utility>
#include "Angel.h"
#include "Camera.h"
#include "Utils.h"
#include "Stack.h"
#include "Skybox.h"

namespace scn
{
	class Node;
	class SceneGraph
	{
	public:
		// Light
		Angel::vec4 LightPosition;
		Angel::vec4 LightPositionEnd;
		float LightAngle;
		float Shininess;

		Angel::mat4 getShadowProjMatrix();

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
		SceneGraph();
		utils::Stack<Angel::mat4> matrixStack;
		Node * pRootNode;
		Angel::vec4 rootPos;

		Skybox background;
	};

	class Scene : public utils::SingleUsage<SceneGraph>
	{
	public:
		static void Render();

		Scene();
		~Scene();
	};
}
