#pragma once
#include <qstring.h>


class Analyser
{
public:
	Analyser(QString filePath);
	QString analyse();

private:
	QString m_codeSummaryCanonicalPath;
	QString m_lizardCanonicalPath;
	QString m_jobPath;
};