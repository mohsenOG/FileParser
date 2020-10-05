#include "DiffParser.h"
#include <qfile.h>
#include <qvector.h>
#include <qtextstream.h>

DiffParser::DiffParser()
{
	m_diffFileCanonicalPath = QString::fromLocal8Bit(::getenv("ENV_JOB_PATH")) + "/commit.diff";
}

QMultiMap<QString, QString> DiffParser::parse()
{
	QFile file(m_diffFileCanonicalPath);
	if (!file.open(QIODevice::ReadOnly))
		return QMultiMap<QString, QString>();

	QMultiMap<QString, QString> multimap;
	QString index;
	QTextStream textStream(&file);

	while (!textStream.atEnd())
	{
		QString line = textStream.readLine();
		if (line.startsWith("Index:"))
		{
			index = line.remove(0, 7);
		}
		else if (line.startsWith("@@"))
		{
			int pos = line.lastIndexOf('@');
			int revertPos = line.size() - pos - 2;
			QString functionName = line.right(revertPos);
			multimap.insert(index, functionName);
		}
	}
	file.close();

	return multimap;
}
