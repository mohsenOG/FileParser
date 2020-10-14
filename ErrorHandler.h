#pragma once
#include <QStringList>

class ErrorHandler
{
public:
	ErrorHandler() {};
	QStringList errors() const { return m_errors; };
	bool hasErrors() const { return m_errors.size() ? true : false; }

protected:
	QStringList m_errors{};
};