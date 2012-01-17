#pragma once

#include <string>

namespace axelynx
{
	class Animation
	{
	public:
		virtual std::string GetName() const = 0;
		virtual float getDuration() const = 0;

		~Animation()
		{
		}
	};
}