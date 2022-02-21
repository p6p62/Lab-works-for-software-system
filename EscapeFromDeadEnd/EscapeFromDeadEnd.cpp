#include <iostream>
#include "Field.h"

using std::cout;
using std::endl;

int main()
{
	Field f(10, 20);
	f.set_player_position({ 3, 5 });

	FieldCell p1{ f.get_player_position() };
	cout << p1.row << " " << p1.col << endl;
	cout << f.get_player_position().row << " " << f.get_player_position().col << endl;

	p1.col = 4;
	cout << p1.row << " " << p1.col << endl;
	cout << f.get_player_position().row << " " << f.get_player_position().col << endl;
}