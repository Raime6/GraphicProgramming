
// Public Domain Code
// Author: Xavier Canals

#pragma once



#include <vector>



namespace finalPractice
{
	template< typename COLOR >

	class ColorBuffer
	{
		public:

			using Color = COLOR;

		private:

			unsigned width;
			unsigned height;

			std::vector< Color > buffer;

		public:

			ColorBuffer(unsigned width, unsigned height) : width(width), height(height), buffer(width* height) {}

			unsigned getWidth()
			{
				return width;
			}

			unsigned getHeight()
			{
				return height;
			}

			Color * colors()
			{
				return buffer.data();
			}

			const Color * colors() const
			{
				return buffer.data();
			}

			Color & get(unsigned offset)
			{
				return buffer[offset];
			}

			const Color & get(unsigned offset) const
			{
				return buffer[offset];
			}

			void set(unsigned offset, const Color & color)
			{
				buffer[offset] = color;
			}
	};
}
