#include <iostream>
#include "Field.h"

using std::cout;
using std::endl;

int main()
{
	cout << "escape success))" << endl;
	Field f(20, 30);
	f.add_field_block({ 5, 5, {15, 21} });
	f.add_field_block({ 2, 1, {0, 0} });
	f.add_field_block({ 1, 2, {10, 24} });

	Field::print_field(cout, f, f.get_field_blocks());
}