#include <cstdlib>
#include "FileParser.h"
#include "Analyser.h"
#include "DIffParser.h"

#include <iostream>
#include <qstring.h>
#include <qfile.h>
#include <QMultiMap>
#include <qdir.h>
#include <qfileinfo.h>

#include <stdio.h>
#include <QProcessEnvironment>
#include <qstringlist.h>

using namespace std;

int main()
{
	//Get and check the diff file
	QString diffFilePath = QString::fromLocal8Bit(::getenv("ENV_JOB_PATH")) + "/commit.diff";
	//printf("diff file path: %s", diffFilePath.toStdString().c_str());

	QFileInfo diffFileInfo(diffFilePath);
	if (!diffFileInfo.exists() || !diffFileInfo.isFile())
	{
		cout << "commit diff file does not exist!" << endl;
		return -1;
	}

	DiffParser diffParser;
	QMultiMap<QString, QString> functionSourceFilePathsAndNames = diffParser.parse();

	if (functionSourceFilePathsAndNames.isEmpty())
	{
		cout << "function source files could not be parsed!" << endl;
		return -1;
	}

	CppParser parser{ functionSourceFilePathsAndNames };
	QString codeSummaryCanonicalPath = parser.parse();


	Analyser analyser(codeSummaryCanonicalPath);
	QString resultHtml = analyser.analyse();
	if (resultHtml.isEmpty())
	{
		cout << "html is not created!" << endl;
		return -1;
	}


	cout << "Process finished!" << endl;
	return 0;
}
