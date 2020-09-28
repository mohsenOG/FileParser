#include "FileParser.h"
#include "qfileinfo.h"
#include <qdir.h>

ParserAbstract::ParserAbstract(QString filePath, QString functionName) :
	m_filePath(filePath),
	m_functionName(functionName)
{
}

QString CppParser::parse()
{
	//TODO convert m_filePath to absolute path
	//TODO make a generic path rather than hard coded
	QFile inputFile(m_filePath);
	//QString tempFilePath = QDir::tempPath() + "output.txt";
	QString tempFilePath = "output.txt";
	QFile outputFile(tempFilePath);
	if (!inputFile.open(QIODevice::ReadOnly | QIODevice::Text) ||
		!outputFile.open(QIODevice::WriteOnly | QIODevice::Text))
		return QString();

	QVector<QString>lines;
	lines.reserve(10000);
	while (!inputFile.atEnd())
	{
		lines.append(inputFile.readLine());
	}
	processLines(outputFile, lines);
	inputFile.close();
	outputFile.close();

	QFileInfo info(outputFile);

	return info.canonicalFilePath();
}

void CppParser::processLines(QFile& outputFile, QVector<QString>& lines)
{
	//finding index of function first line in all lines.
	int idx = getFirstFunctionLineIndex(lines);
	if (idx == -1)
		return;

	for (int i = idx; i < lines.size(); ++i)
	{
		readOpenningCurlyBrackets(lines[i]);
		readClosingCurlyBrackets(lines[i]);
		outputFile.write(lines[i].toUtf8());

		// When no. of { and } are equal, means the function is read completely.
		if (m_numberOfOpenningBrackets > 0 && m_numberOfClosingBrackets > 0 && m_numberOfOpenningBrackets == m_numberOfClosingBrackets)
			break;
	}
	return;
}

int CppParser::getFirstFunctionLineIndex(QVector<QString>& lines)
{
	int idx = -1;
	for (int i = 0; i < lines.size(); ++i)
	{
		if (lines.at(i).contains(m_functionName, Qt::CaseInsensitive))
			idx = i;
	}
	return idx;
}

void CppParser::readOpenningCurlyBrackets(QString& line)
{
	if (line.contains("{", Qt::CaseInsensitive))
		++m_numberOfOpenningBrackets;
}

void CppParser::readClosingCurlyBrackets(QString& line)
{
	if (line.contains("}", Qt::CaseInsensitive))
		++m_numberOfClosingBrackets;
}