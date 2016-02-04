
#pragma once

#include <ionMath.h>
#include <ionGraphics.h>


namespace ion
{
	namespace Scene
	{

		class CLight
		{

		public:

			vec3f const & GetPosition() const;
			void SetPosition(vec3f const & Position);

			uint GetAttributeCount() const;
			Graphics::IUniform const * GetAttributeByName(string const & Name) const;

			virtual string const & GetLightType() const = 0;

		protected:

			vec3f Position;
			map<string, Graphics::IUniform *> Attributes;

		};

	}
}
