#pragma once
#include <qstring.h>


class Analyser
{
public:
	Analyser(QString filePath);
	~Analyser();

	void analyse();


private:
	QString m_filePath;
};