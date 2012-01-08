#include <QtCore/QFile>
#include <QtCore/QBuffer>
#include <QtCore/QDir>
#include <QtGui/QApplication>
#include <QtGui/QScrollArea>

#include "mobi.h"
#include "page.h"
#include "parser.h"
#include "bookdevice.h"
#include "bookshelf.h"
#include "shelfscreen.h"
#include "filter.h"
#include "dictionaryfilter.h"
#include "whitaker.h"
#include "settings.h"

Bookshelf * bookshelf;
QStackedWidget * top_level;
Settings * settings;

FilterManager * filter_manager;

int main(int argc, char ** argv)
{
    QApplication app(argc, argv);
    QFont font("Droid Sans");
    font.setBold(true);
    app.setFont(font);

    settings = new Settings();
    
    filter_manager = new FilterManager();
    filter_manager->addFilter(new DictionaryFilter(new WhitakerDictionary()));
    
    bookshelf = new Bookshelf;

    QString docs_path;
    docs_path = QDir::homePath() + QDir::separator();
    bookshelf->addPath(docs_path + "Documents");
    bookshelf->addPath(docs_path + "calliope");
    bookshelf->addPath("/sdcard/kindle");
    bookshelf->addPath("/sdcard/calliope");
    
    Shelfscreen * shelfscreen = new Shelfscreen;
    shelfscreen->updateSlot();
    QObject::connect(bookshelf, SIGNAL(shelfChanged()), shelfscreen, SLOT(updateSlot()));
    
    QScrollArea * qsa = new QScrollArea;
    qsa->setWidgetResizable(true);
    qsa->setWidget(shelfscreen); 
    shelfscreen->update();
    
    top_level = new QStackedWidget;
    top_level->addWidget(qsa);
    
    settings->setActiveTouch("Dictionary");
    Filter * f = filter_manager->getFilter(settings->getActiveTouch());
    filter_manager->setActiveTouchFilter(f);
    
    if (shelfscreen->readBook(settings->getCurrentBook()))
    {
        shelfscreen->currentPage()->setPage(settings->getCurrentPage());
    }
    
    top_level->setGeometry(settings->getX(), settings->getY(),
                           settings->getWidth(), settings->getHeight());
    if (settings->getFullScreen())
    {
        top_level->setWindowState(top_level->windowState() ^ Qt::WindowFullScreen);
    }
    top_level->show();
    
    app.exec();
}
