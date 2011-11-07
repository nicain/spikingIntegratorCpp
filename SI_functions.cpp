#include "SI_functions.h"
#include <string>
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.

using namespace std;

string remove_hyphens(boost::uuids::uuid uuid)
{
	string s1 = boost::uuids::to_string(uuid);
	s1.erase(s1.begin()+8, s1.begin()+9);
	s1.erase(s1.begin()+12, s1.begin()+13);
	s1.erase(s1.begin()+16, s1.begin()+17);
	s1.erase(s1.begin()+20, s1.begin()+21);
	return s1;
}