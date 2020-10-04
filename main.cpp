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

using namespace std;

int main()
{
	//Get and check the diff file
	//QString diffFilePath = QString::fromLocal8Bit(::getenv("ENV_JOB_PATH")) + "/commit.diff";
	QString diffFilePath = "commit.diff";
	QFileInfo diffFileInfo(diffFilePath);
	if (!diffFileInfo.exists() || !diffFileInfo.isFile())
	{
		cout << "commit diff file does not exist!" << endl;
		return -1;
	}
	DiffParser diffParser{ diffFilePath };
	QMultiMap<QString, QString> functionSourceFilePathsAndNames = diffParser.parse();

	
	CppParser parser{ functionSourceFilePathsAndNames };
	QString codeSummaryPath = parser.parse();
	
	Analyser analyser(codeSummaryPath);
	QString resultHtml = analyser.analyse();

	cout << "Process finished!" << endl;
	return 0;
}
