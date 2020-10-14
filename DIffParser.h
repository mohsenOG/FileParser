#pragma once

#include <QMultiMap>
#include <qstring.h>
#include <qstringlist.h>

#include "ErrorHandler.h"


class DiffParser : public ErrorHandler
{
public:
	DiffParser();
	QMultiMap<QString, QString> parse();

private:
	QString m_diffFileCanonicalPath;
};