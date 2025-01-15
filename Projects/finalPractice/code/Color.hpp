
/*
	Public Domain Code

	Author: Xavier Canals
	Author: Ángel Rodríguez
*/

#pragma once



#include <cstdint>



namespace finalPractice
{
	/// <summary>
	/// Monochrome8 is a typedef for uint8_t representing a single channel color value.
	/// This is typically used for grayscale or other monochrome color representations.
	/// </summary>
	using Monochrome8 = uint8_t;

	/// <summary>
	/// The Rgba8888 union represents a color using the RGBA format with 8 bits per channel.
	/// It provides both direct access to the whole color value as a 32-bit integer and 
	/// individual components (red, green, blue, and alpha) as a byte array.
	/// </summary>
	union Rgba8888
	{
		enum { RED, GREEN, BLUE, ALPHA }; ///< Enumerates the component indices for RGBA.

		uint32_t         value;			  ///< The entire color value as a 32-bit unsigned integer.
		uint8_t  components[4];			  ///< An array of 4 unsigned 8-bit integers representing the individual color components: RED, GREEN, BLUE, and ALPHA.
	};
}
