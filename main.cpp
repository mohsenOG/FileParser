#include <cstdlib>
#include "FileParser.h"
#include "SvnDiffParser.h"

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

enum ReturnValue {
	ERROR = -1,
	OK = 0
};

int main()
{
	//Get and check the diff file
	QString diffFilePath = QString::fromLocal8Bit(::getenv("ENV_JOB_PATH")) + "/commit.diff";
	QFileInfo diffFileInfo(diffFilePath);
	if (!diffFileInfo.exists() || !diffFileInfo.isFile())
	{
		cout << "commit.diff file does not exist!\nCheck your ENV_JOB_PATH in the jenkins script." << endl;
		return ReturnValue::ERROR;
	}

	SvnDiffParser diffParser;
	QMultiMap<QString, QString> functionSourceFilePathsAndNames = diffParser.parse();

	if (diffParser.hasErrors())
	{
		for (auto msg : diffParser.errors())
		{
			cout << msg.toStdString().c_str() << endl;
		}
		return ReturnValue::ERROR;
	}

	if (functionSourceFilePathsAndNames.isEmpty())
	{
		cout << "There is no change in cpp files.";
		ReturnValue::OK;
	}

	CppParser cppParser{ functionSourceFilePathsAndNames };
	QString codeSummaryCanonicalPath = cppParser.parse();

	if (cppParser.hasErrors())
	{
		for (auto msg : diffParser.errors())
		{
			cout << msg.toStdString().c_str() << endl;
		}
		return ReturnValue::ERROR;
	}

	cout << "Process finished!" << endl;
	return 0;
}
