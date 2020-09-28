#pragma once

#include <qvector.h>
#include <qfile.h>
#include <QMultiMap>
#include <qstring.h>

class ParserAbstract
{
public:
	ParserAbstract(QMultiMap<QString, QString> functionPathsAndNames);
	virtual QString parse() = 0;

protected:
	QMultiMap<QString, QString> m_fucntionPathsAndNames;
};


class CppParser : public ParserAbstract
{
public:
	CppParser(QMultiMap<QString, QString> functionPathsAndNames) :ParserAbstract(functionPathsAndNames) {};
	/** Returns the canonical path of the newly created parsed file*/
	QString parse() override;

private:
	void processLines(QFile& outputFile, QVector<QString>& lines, QString functionName);
	int getFirstFunctionLineIndex(QVector<QString>& lines, QString functionName);
	void readOpenningCurlyBrackets(QString& line);
	void readClosingCurlyBrackets(QString& line);
	void resetNumberOfOpeningAndClosingBrackets();
	void addEmptyLine(QFile& file);



private:
	int m_numberOfOpenningBrackets{ 0 };
	int m_numberOfClosingBrackets{ 0 };
};