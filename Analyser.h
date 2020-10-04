#pragma once
#include <qstring.h>


class Analyser
{
public:
	static const QString lizardScriptPath;

public:
	Analyser(QString filePath);

	QString analyse();


private:
	QString m_inputFilePath;
};