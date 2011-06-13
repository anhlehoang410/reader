#ifndef _PARSER_
#define _PARSER_

#include <QtCore/QStack>
#include <QtCore/QTextStream>
#include <QtCore/QIODevice>

#include "element.h"

class TagAttribute
{
  public:

    QString name;
    QString value;
    
};


class Tag
{
  public:

    QString name;
    QList<TagAttribute> attributes;
    QString contents;
    
};


class Parser 
{
  public:

    Parser(QIODevice * d, int encoding);
    Element * next();

    void dumpTag(Tag &);
    
  protected:
    
    QIODevice * device;
    QStack<Tag> tags;

    int state;
    QTextStream * stream;
    
};

#endif
