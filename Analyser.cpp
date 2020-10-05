#include "Analyser.h"
#include <qfile.h>
#include <qprocess.h>
#include <qstringlist.h>
#include <iostream>
#include <qfileinfo.h>
#include <qdir.h>

Analyser::Analyser(QString filePath) :
	m_codeSummaryCanonicalPath(filePath)
{
	m_lizardCanonicalPath = QString::fromLocal8Bit(::getenv("ENV_A4_WORKSPACE_PATH")) + "/tools/lizard-master/lizard.py";
	m_jobPath = QString::fromLocal8Bit(::getenv("ENV_JOB_PATH"));
}

QString Analyser::analyse()
{
	QStringList args;
	args << "cmd.exe /C python" << m_lizardCanonicalPath << "-H" << m_codeSummaryCanonicalPath;

	QProcess lizardProcess;
	lizardProcess.start(args.join(' '));
	lizardProcess.waitForStarted();
	lizardProcess.waitForFinished();
	auto result = lizardProcess.readAllStandardOutput();

	// write the result to file.
	QString outputCanonicalPath = m_jobPath + "/complexityAnalyserResult.html";
	QFile file(outputCanonicalPath);
	
	if (file.exists())
		QFile::remove(outputCanonicalPath);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return QString();

	file.write(result);
	file.close();
	
	QFileInfo fileInfo(file);

	return fileInfo.canonicalFilePath();
}