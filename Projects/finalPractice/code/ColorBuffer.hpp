
/*
	Public Domain Code

	Author: Xavier Canals
	Author: Ángel Rodríguez
*/

#pragma once



#include <vector>



namespace finalPractice
{
	/// <summary>
	/// The ColorBuffer class represents a 2D array of colors stored in a flat buffer.
	/// It allows for efficient storage and access to a grid of colors, typically used in image processing
	/// or rendering contexts.
	/// </summary>
	/// 
	/// <typeparam name="COLOR">The type of color used in the buffer, such as Rgba8888 or Monochrome8.</typeparam>
	template< typename COLOR >
	class ColorBuffer
	{
		public:

			using Color = COLOR;		 ///< Type alias for the color used in the buffer.

		private:

			unsigned  width;			 ///< The width of the color buffer (in pixels).
			unsigned height;             ///< The height of the color buffer (in pixels).
			
			std::vector< Color > buffer; ///< A 1D vector storing all colors in the buffer.

		public:

			/// <summary>
			/// Constructs a ColorBuffer with the specified width and height, initializing all pixels to default color values.
			/// </summary>
			/// 
			/// <param name="width">The width of the color buffer (in pixels).</param>
			/// <param name="height">The height of the color buffer (in pixels).</param>
			ColorBuffer(unsigned width, unsigned height) : width(width), height(height), buffer(width* height) {}

			/// <summary>
			/// Gets the width of the color buffer.
			/// </summary>
			/// 
			/// <return>The width of the buffer in pixels.</return>
			unsigned getWidth()
			{
				return width;
			}

			/// <summary>
			/// Gets the height of the color buffer.
			/// </summary>
			/// 
			/// <return>The height of the buffer in pixels.</return>
			unsigned getHeight()
			{
				return height;
			}

			/// <summary>
			/// Returns a pointer to the color data, allowing access to all colors in the buffer.
			/// </summary>
			/// 
			/// <return>A pointer to the first element of the color buffer.</return>
			Color * colors()
			{
				return buffer.data();
			}

			/// <summary>
			/// Returns a constant pointer to the color data, allowing read-only access to all colors in the buffer.
			/// </summary>
			/// 
			/// <return>A constant pointer to the first element of the color buffer.</return>
			const Color * colors() const
			{
				return buffer.data();
			}

			/// <summary>
			/// Gets the color at a specific offset in the buffer.
			/// </summary>
			/// 
			/// <param name="offset">The index of the color to retrieve (in 1D array format).</param>
			/// 
			/// <return>The color at the specified offset.</return>
			Color & get(unsigned offset)
			{
				return buffer[offset];
			}

			/// <summary>
			/// Gets the color at a specific offset in the buffer (read-only version).
			/// </summary>
			/// 
			/// <param name="offset">The index of the color to retrieve (in 1D array format).</param>
			/// 
			/// <return>The color at the specified offset.</return>
			const Color & get(unsigned offset) const
			{
				return buffer[offset];
			}

			/// <summary>
			/// Sets the color at a specific offset in the buffer.
			/// </summary>
			/// 
			/// <param name="offset">The index of the color to set (in 1D array format).</param>
			/// <param name="color">The color to set at the specified offset.</param>
			void set(unsigned offset, const Color & color)
			{
				buffer[offset] = color;
			}
	};
}
