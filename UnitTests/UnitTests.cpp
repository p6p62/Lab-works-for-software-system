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

		TEST_METHOD(set_end_game_position)
		{
			Field field(10, 20);

			bool b1 = field.set_endgame_position({ 2, 5 });
			FieldCell pos1{ field.get_endgame_position() };
			bool b2 = field.set_endgame_position({ 2, 20 });
			FieldCell pos2{ field.get_endgame_position() };
			bool t1 = pos1.row == 2 && pos1.col == 5;
			bool t2 = pos2.row == 2 && pos2.col == 5;

			Assert::IsTrue(b1 && !b2 && t1 && t2);
		}

		TEST_METHOD(is_field_in_endgame_state)
		{
			Field field(15, 25);
			field.set_endgame_position({ 5, 6 });

			field.set_player_position({ 1, 2 });
			bool t1 = field.is_endgame_state();

			field.set_player_position({ 5, 2 });
			bool t2 = field.is_endgame_state();

			field.set_player_position({ 5, 6 });
			bool t3 = field.is_endgame_state();

			Assert::IsTrue(!t1 && !t2 && t3);
		}

		TEST_METHOD(add_field_block)
		{
			Field f(70, 70);

			bool t1 = f.add_field_block({ 10, 10, {20, 20} }); // true
			bool t2 = f.add_field_block({ 40, 15, {20, 5} }); // true
			bool t3 = f.add_field_block({ 6, 15, {30, 25} }); // true
			bool t4 = f.add_field_block({ 5, 4, {42, 55} }); // true
			bool t5 = f.add_field_block({ 13, 4, {27, 34} }); // false
			bool t6 = f.add_field_block({ 15, 5, {8, 65} }); // true
			bool t7 = f.add_field_block({ 2, 5, {22, 61} }); // false
			bool t8 = f.add_field_block({ 7, 4, {66, 9} }); // false

			std::string s;
			s.push_back(t1 + '0');
			s.push_back(t2 + '0');
			s.push_back(t3 + '0');
			s.push_back(t4 + '0');
			s.push_back(t5 + '0');
			s.push_back(t6 + '0');
			s.push_back(t7 + '0');
			s.push_back(t8 + '0');

			const char* result = s.c_str();

			const char* expected = "11110100";
			Assert::AreEqual(expected, result);
		}
	};
}
