#pragma once

#include <lev/node/node.h>
#include <lev/math/transform_2d.h>

namespace lev
{
	class Node2D : public Node
	{
	public:
		Transform2D transform;

		Mat3x2 matrix_relative_to_parent(const Node* node)
		{
			// any node passed into here should be of type Node2D
			// todo just for testing not perm
			return this->transform.matrix() * ((Node2D*)node)->transform.matrix();
		}
	};
}
