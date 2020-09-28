#include "FileParser.h"

#include <iostream>
#include <qstring.h>
#include <qfile.h>
#include <QMultiMap>
#include <qdir.h>

using namespace std;

int main()
{

	QString filePath = "E:/a4-dev/animator/a4lib_cmd/src/CommandC2d.cpp";
	QString functionName = "int GraphAxisWeightFilter(CommandS * cmd, int nDim";
	QMultiMap<QString, QString> pathsAndNames;
	pathsAndNames.insert(filePath, functionName);
	CppParser parser{ pathsAndNames};
	QString filepath = parser.parse();

	//Py_Initialize();
	//Py_Finalize();

	cout << "hellp" << endl;
	return 0;
}
