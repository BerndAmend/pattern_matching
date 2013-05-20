/**
 * @file
 * @author    Bernd Amend <berndamend@gemail.com>
 * @date      2013
 * @copyright Copyright Bernd Amend 2013
 *            Distributed under the Boost Software License, Version 1.0.
 *            (See accompanying file LICENSE_1_0.txt or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */
#include <iostream>
#include <vector>
#include "pattern_matching.hpp"

using namespace pattern_matching;

int main() {
	const std::vector<int> vec = {10, 20, 30, 40};

	int num;

	if (match(vec, 10, &num, range::all()))
		std::cout<<"second element="<< num<<std::endl;

	if (match(vec, 20, &num, range::all()))
		std::cout<<":("<<std::endl;

	if (match(vec, range::all(), 20, &num, range::all()))
		std::cout<<"third element="<<num<<std::endl;

	//std::cout<<match(vec,
	//	Case(pattern(10, ref(num), range::all()), [&](){return num;}}),
	//	Case(pattern(range::all(), 20, ref(num), range::all()), [&](){return num;}}),
	//	Else([&](){return 0;})<<std::endl;

/*
	std::string teststring = "Dies ist ein Test String");
	std::string result;
	if (match(vec, "Dies", range::all(), "ein", ' ', &result, ' ', range::all()))
		std::cout<<"result="<<result<<std::endl;

	//const std::vector<Address> vec = {{"bernd", "amend"}, {"sebastien", "wagener"}};

	//if (matcher<std::vector<int>>::m(vec, dontcare(), dontcare<Address>("bernd", dontcare()), &next, dontcare()))
	//	std::cout<<"third element="<<num<<std::endl;

	Address add = {"test", "user"};
	if(!matcher<Address>::m(add, std::make_tuple(&Address::lastname, "test")))
		std::cout<<":)"<<std::endl;

	if(matcher<Address>::m(add, std::make_tuple(&Address::lastname, "user")))
		std::cout<<":)"<<std::endl;

	const std::vector<Address> vec = {{"a", "b"}, {"c", "d"}, {"e","f"}};
	std::string firstname;
	if (match(vec, Address::m(_firstname="a"), ref(&Address::_firstname), range::all()))
		std::cout<<"second element="<<firstname<<std::endl;
*/
}
