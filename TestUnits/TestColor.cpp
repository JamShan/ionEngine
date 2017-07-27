
#include <ionCore/Color.h>
#include <catch.hpp>

using namespace ion;

TEST_CASE("Color::Hex")
{
	CHECK(Color::Hex(0xff6666) == color3i(255, 102, 102));
	CHECK(Color::Hex(0xFF0000) == color3i(255, 0, 0));
	CHECK(Color::Hex(0x33EEEE) == color3i(51, 238, 238));
}

TEST_CASE("Color::HSV")
{
	CHECK(Color::HSV(0.f, 0.f, 0.f) == color3f(0, 0, 0));
	CHECK(Color::HSV(0.f, 0.f, 1.f) == color3f(1, 1, 1));
	CHECK(Color::HSV(0.f, 1.f, 1.f) == color3f(1, 0, 0));

	CHECK(Color::HSV(1.f / 6.f, 1.f, 1.f) == color3f(1, 1, 0));
	CHECK(Color::HSV(1.f / 3.f, 1.f, 1.f) == color3f(0, 1, 0));
	CHECK(Color::HSV(1.f / 2.f, 1.f, 1.f) == color3f(0, 1, 1));
	CHECK(Color::HSV(2.f / 3.f, 1.f, 1.f) == color3f(0, 0, 1));
	CHECK(Color::HSV(5.f / 6.f, 1.f, 1.f) == color3f(1, 0, 1));
}
