#include "FileParser.h"
#include "qfileinfo.h"
#include <qdir.h>
#include <cstdlib>
#include <cstdio>


ParserAbstract::ParserAbstract(QMultiMap<QString, QString> functionPathsAndNames) :
	m_functionPathsAndNames(functionPathsAndNames)
{
}

CppParser::CppParser(QMultiMap<QString, QString> functionPathsAndNames)
	:
	ParserAbstract(functionPathsAndNames)
{
	// keep only cpp files.
	auto keyList = m_functionPathsAndNames.uniqueKeys();
	for (auto key : keyList)
	{
		if (key.isEmpty() || !key.endsWith(".cpp"))
			m_functionPathsAndNames.remove(key);
	}

}


QString CppParser::parse()
{
	QString jenkinsJobPath = QString::fromLocal8Bit(::getenv("ENV_JOB_PATH"));
	QString outputFilePath = jenkinsJobPath + "/commitedFunctions.cpp";
	
	//printf("jenkinsJobPath %s", jenkinsJobPath.toStdString().c_str());
	//printf("outputFilePath %s", outputFilePath.toStdString().c_str());

	if (QFileInfo::exists(outputFilePath))
		QFile::remove(outputFilePath);

	QFile outputFile(outputFilePath);
	if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Text))
		return QString();

	// Parse each function and save it to a single file in the jenkins job
	for (auto functionPathAndName = m_functionPathsAndNames.begin(); functionPathAndName != m_functionPathsAndNames.end(); ++functionPathAndName)
	{
		QString canonicalFilePath = QString::fromLocal8Bit(::getenv("ENV_A4_WORKSPACE_PATH")) + "/" + functionPathAndName.key();
		//printf("canonicalFilePath %s", canonicalFilePath.toStdString().c_str());

		QFile inputFile(canonicalFilePath);
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

		// When no. of { and } are equal, means the function is complete.
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