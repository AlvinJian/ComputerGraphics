#pragma once
#include "Angel.h"
#include <utility>

namespace assignment2 {
	class Deform
	{
	public:
		enum AXIS
		{
			X_AXIS = 0, Y_AXIS, Z_AXIS
		};
		using ShearVal = std::pair<AXIS, float>;
		using TwistVal = std::pair<AXIS, float>;

		Deform();
		~Deform();

		Angel::mat4 getShearMatrix() const;
		void addShear(const ShearVal& val);
		// Angel::mat4 getTwistMatrix() const;
		// void addTwist(const TwistVal& val);
	private:
		float shearVal[3];
		float twistVal[3];
	};
}

