#pragma once

#include <qstring.h>
#include <qvector.h>
#include <qfile.h>

class ParserAbstract
{
public:
	ParserAbstract(QString filePath, QString functionName);
	virtual QString parse() = 0;

protected:
	QString m_filePath;
	QString m_functionName;
};


class CppParser : public ParserAbstract
{
public:
	CppParser(QString filePath, QString functionName) :ParserAbstract(filePath, functionName) {};
	/** Returns the canonical path of the newly created parsed file*/
	QString parse() override;

private:
	void processLines(QFile& outputFile, QVector<QString>& lines);
	int getFirstFunctionLineIndex(QVector<QString>& lines);
	void readOpenningCurlyBrackets(QString& line);
	void readClosingCurlyBrackets(QString& line);

private:
	int m_numberOfOpenningBrackets{ 0 };
	int m_numberOfClosingBrackets{ 0 };
};