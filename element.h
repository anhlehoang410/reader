#ifndef _ELEMENT_
#define _ELEMENT_

#include <QtCore/QStringList>
#include <QtGui/QPaintDevice>
#include <QtGui/QFont>
#include <QtGui/QPixmap>

class Element
{
  public:

    Element()
    {
        current_position = 0;
        current_height = 0;
        element_number = 0;
        is_highlighted = false;
    }

    virtual ~Element()
    {
    }
    
    virtual bool render(QPaintDevice *, int x,int y, int w, int h) = 0;
    virtual QRect size(int w, int downpage, int pageheight) = 0;
    virtual bool pageTerminator() = 0;

    qint64 position()  { return current_position; }
    void setPosition(qint64 p) { current_position = p; }
    qint64 height() { return current_height; }
    void setHeight(qint64 h) { current_height = h; }
    qint64 number() { return element_number; }
    void setNumber(qint64 n) { element_number = n; }

    bool highlighted() 
    {
        return is_highlighted;
    }

    void setHighlighted(bool b)
    {
        is_highlighted = b;
    }
    
  protected:

    qint64 current_position;
    qint64 current_height;
    qint64 element_number;
    bool is_highlighted;

};

class StringFragment
{
  public:

    StringFragment()
    {
        is_italic = false;
        is_bold = false;
    }

    StringFragment(QString t, bool i = false, bool b = false)
    {
        text=t.split(' '); is_italic=i; is_bold=b;
    }
    
    QStringList text;
    bool is_italic;
    bool is_bold;
    
};

class ParagraphElement : public Element
{
  public:

    ParagraphElement()
    {
        font = QFont("Times New Roman", 36);
    }
    
    virtual QRect size(int w, int downpage, int pageheight);
    virtual bool render(QPaintDevice *, int x,int y, int w, int h);

    virtual bool pageTerminator() { return false; } 
    
    void addFragment(StringFragment s)
    {
        fragments.push_back(s);
    }

    int numFragments()
    {
        return fragments.size();
    }
    
  protected:

    QList<StringFragment> fragments;
    QFont font;
    
};

class PictureElement : public Element
{
  public:

    PictureElement(QImage & i)
    {
        pixmap = QPixmap::fromImage(i);
    }

    virtual QRect size(int w, int downpage, int pageheight);
    virtual bool render(QPaintDevice *, int x,int y, int w, int h);
    
    virtual bool pageTerminator() { return false; }
    
  protected:

    QPixmap pixmap;

};

class PagebreakElement : public Element
{
  public:
    
    virtual QRect size(int w, int downpage, int pageheight);
    virtual bool render(QPaintDevice *, int,int,int,int)
    { return true; }
    virtual bool pageTerminator() { return true; }
    
};

#endif
