#pragma once

#include <QMultiMap>
#include <qstring.h>


class DiffParser
{
public:
	DiffParser();
	QMultiMap<QString, QString> parse();

private:
	QString m_diffFileCanonicalPath;
};