#pragma once

#include <QMultiMap>
#include <qstring.h>
#include <qstringlist.h>

#include "ErrorHandler.h"


class SvnDiffParser : public ErrorHandler
{
public:
	SvnDiffParser();
	QMultiMap<QString, QString> parse();

private:
	QString m_diffFileCanonicalPath;
};