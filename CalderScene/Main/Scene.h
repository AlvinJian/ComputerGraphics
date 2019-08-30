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
	class Scene : public utils::SingleUsage<Scene>
	{
	public:
		static void Render();

		// Light
		Angel::vec4 LightPosition;
		Angel::vec4 LightPositionEnd;
		float LightAngle;
		float Shininess;

		// env and shadow
		Skybox background;
		float groundLevel;
		bool shadowToggle;

		Scene();
		~Scene();

		Angel::mat4 curModelMatrix;
		Angel::vec4 curColor;
		Camera camera;

		void render();
		void pushModelMatrix();
		void popModelMatrix();

		virtual void setRoot(Node * root, Angel::vec3 pos);
		std::pair<const Node *, const Angel::vec4 *>
			getRoot() const;

	private:
		utils::Stack<Angel::mat4> matrixStack;
		Node * pRootNode;
		Angel::vec4 rootPos;
	};
}
