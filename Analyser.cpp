#include "Analyser.h"
#include "Python.h"
#include <qfile.h>

Analyser::Analyser(QString filePath) :
	m_filePath(filePath)
{

}

Analyser::~Analyser()
{
	QFile::remove(m_filePath);
}

void Analyser::analyse()
{
}