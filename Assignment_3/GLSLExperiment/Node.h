#pragma once
#include <vector>
#include <stack>
#include "Angel.h"
#include "Scene.h"

namespace assignment3
{
	class Node
	{
	public:
		Node();
		virtual ~Node();
		virtual void action(SceneGraph & scene) = 0;
	};

	class GroupNode : public Node
	{
	public:
		GroupNode();
		virtual ~GroupNode();

		GroupNode& addChild(Node* c);
		const std::vector<Node*> & getChilds() const;
		virtual void action(SceneGraph & scene) override;

	private:
		std::vector<Node*> childs;
	};
}

