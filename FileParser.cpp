#include "FileParser.h"
#include "qfileinfo.h"
#include <qdir.h>
#include <cstdlib>


ParserAbstract::ParserAbstract(QMultiMap<QString, QString> functionPathsAndNames) :
	m_fucntionPathsAndNames(functionPathsAndNames)
{
}

QString CppParser::parse()
{
	//QString tempOutputPath = QDir::tempPath() + "output.txt";
	//QString tempOutputPath = "output.txt";
	QString jenkinsJobPath = QString::fromLocal8Bit(::getenv("ENV_JOB_PATH"));
	QString outputFilePath = jenkinsJobPath + "/commitedFunctions.cpp";
	if (QFileInfo::exists(outputFilePath))
		QFile::remove(outputFilePath);

	QFile outputFile(outputFilePath);
	if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Text))
		return QString();

	for (auto functionPathAndName = m_fucntionPathsAndNames.begin(); functionPathAndName != m_fucntionPathsAndNames.end(); ++functionPathAndName)
	{
		QFile inputFile(functionPathAndName.key());
		if (!inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
			return QString();

		QVector<QString>lines;
		lines.reserve(10000);
		while (!inputFile.atEnd())
		{
			lines.append(inputFile.readLine());
		}
		inputFile.close();
		processLines(outputFile, lines, functionPathAndName.value());
		addEmptyLine(outputFile);
	}
	outputFile.close();
	QFileInfo info(outputFile);

	return info.canonicalFilePath();
}

void CppParser::processLines(QFile& outputFile, QVector<QString>& lines, QString functionName)
{
	//finding index of function first line in all lines.
	int idx = getFirstFunctionLineIndex(lines, functionName);
	if (idx == -1)
		return;
	
	resetNumberOfOpeningAndClosingBrackets();
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

int CppParser::getFirstFunctionLineIndex(QVector<QString>& lines, QString functionName)
{
	int idx = -1;
	for (int i = 0; i < lines.size(); ++i)
	{
		if (lines.at(i).contains(functionName, Qt::CaseInsensitive))
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

void CppParser::resetNumberOfOpeningAndClosingBrackets()
{
	m_numberOfOpenningBrackets = 0;
	m_numberOfClosingBrackets = 0;
}

void CppParser::addEmptyLine(QFile& file)
{
	QString emptyString{};
	file.write(emptyString.toUtf8());
}