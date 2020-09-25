#include "FileParser.h"

#include <iostream>
#include <qstring.h>
#include <qfile.h>

using namespace std;

int main()
{
	QString filePath = "XXX";
	QString functionName = "XXX";
	CppParser parser{ filePath, functionName };
	auto& file = parser.parse();
	cout << "Hello" << endl;
	return 0;
}
