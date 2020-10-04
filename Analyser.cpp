#include "Analyser.h"
#include <qfile.h>
#include <qprocess.h>
#include <qstringlist.h>
#include <iostream>
#include <qfileinfo.h>
#include <qdir.h>

const QString Analyser::lizardScriptPath = "./3rdParty/lizard-master/lizard.py";


Analyser::Analyser(QString filePath) :
	m_inputFilePath(filePath)
{

}
QString Analyser::analyse()
{
	QStringList args;
	args << "cmd.exe /C python" << lizardScriptPath << "-w" << m_inputFilePath;

	QProcess lizardProcess;
	lizardProcess.start(args.join(' '));
	lizardProcess.waitForStarted();
	lizardProcess.waitForFinished();
	auto result = lizardProcess.readAllStandardOutput();

	// write the result to file.
	QString outputPath = QDir::tempPath() + "/complexityAnalyser_result.html";
	QFile file(outputPath);
	
	if (file.exists())
		QFile::remove(outputPath);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return QString();

	file.write(result);
	file.close();

	QFileInfo fileInfo(file);

	return fileInfo.canonicalFilePath();
}