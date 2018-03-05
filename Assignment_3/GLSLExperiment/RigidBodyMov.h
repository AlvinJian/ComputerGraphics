#pragma once
#include "Angel.h"

namespace assignment3 {
	class RigidBodyMov
	{
	public:
		RigidBodyMov();
		~RigidBodyMov();

		Angel::mat4 getTranslateMatrix() const;
		Angel::mat4 getRotateMatrix() const;
		Angel::vec3& getTranslate();
		Angel::vec3& getRotate();
		void translate(float dx, float dy, float dz);
		void rotate(float dx, float dy, float dz);

	private:
		Angel::vec3 transState;
		Angel::vec3 rotState;
	};
}

