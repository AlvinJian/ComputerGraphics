#pragma once
#include <vector>
#include <stack>
#include "Angel.h"

namespace assignment3
{
	class Scene;
	class Node
	{
	public:
		Node();
		virtual ~Node();
		virtual void action(Scene & scene) = 0;
	};

	class GroupNode : public Node
	{
	public:
		GroupNode();
		virtual ~GroupNode();

		GroupNode& addChild(Node* c);
		const std::vector<Node*> & getChilds() const;
		virtual void action(Scene & scene) override;

	private:
		std::vector<Node*> childs;
	};
}

