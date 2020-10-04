#pragma once

#include <QMultiMap>
#include <qstring.h>


class DiffParser
{
public:
	DiffParser(QString& diffFilePath);
	QMultiMap<QString, QString> parse();

private:
	QString m_diffFileCanonicalPath;
};