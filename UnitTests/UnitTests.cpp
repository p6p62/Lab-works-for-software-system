#include "pch.h"
#include "CppUnitTest.h"
#include "..\EscapeFromDeadEnd\Field.h"
#include "..\EscapeFromDeadEnd\Field.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(Field_tests)
	{
	public:

		TEST_METHOD(set_player_position)
		{
			Field field(10, 20);

			bool b1 = field.set_player_position({ 2, 5 });
			FieldCell pos1{ field.get_player_position() };
			bool b2 = field.set_player_position({ 2, 20 });
			FieldCell pos2{ field.get_player_position() };
			bool t1 = pos1.row == 2 && pos1.col == 5;
			bool t2 = pos2.row == 2 && pos2.col == 5;

			Assert::IsTrue(b1 && !b2 && t1 && t2);
		}
	};
}
