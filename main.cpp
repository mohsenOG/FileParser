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

enum Status {
	Error = -1,
	Ok = 0
};

int main()
{
	//Get and check the diff file
	QString svnDiffFilePath = QString::fromLocal8Bit(::getenv("ENV_JOB_PATH")) + "/commit.diff";
	QFileInfo svnDiffFileInfo(svnDiffFilePath);
	if (!svnDiffFileInfo.exists() || !svnDiffFileInfo.isFile())
	{
		cout << "commit.diff file does not exist!\nCheck your ENV_JOB_PATH in the jenkins script." << endl;
		return Status::Error;
	}

	SvnDiffParser svnDiffParser;
	QMultiMap<QString, QString> functionSourceFilePathsAndNames = svnDiffParser.parse();
	if (svnDiffParser.hasErrors())
	{
		for (auto msg : svnDiffParser.errors())
		{
			cout << msg.toStdString().c_str() << endl;
		}
		return Status::Error;
	}

	if (functionSourceFilePathsAndNames.isEmpty())
	{
		cout << "There is no change in cpp files.";
		Status::Ok;
	}

	CppParser cppParser{ functionSourceFilePathsAndNames };
	QString codeSummaryCanonicalPath = cppParser.parse();
	if (cppParser.hasErrors())
	{
		for (auto msg : svnDiffParser.errors())
		{
			cout << msg.toStdString().c_str() << endl;
		}
		return Status::Error;
	}

	cout << "Process finished!" << endl;
	return 0;
}
