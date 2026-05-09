#include"UserInterface.h"

using namespace std;

string colors::ColorSet(const string& code)
{
	return "\033[" + code + "m";
}

string colors::ColorReset()
{
	return "\033[0m";
}