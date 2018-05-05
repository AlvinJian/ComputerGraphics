#pragma once
#include "Angel.h"

namespace scn
{
	class Camera
	{
	public:
		Camera();
		~Camera();

		Angel::vec4 position;
		Angel::vec4 up;
		Angel::vec4 at;

		Angel::mat4 createViewMat() const;
	};
}
