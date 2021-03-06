#include <QApplication>
#include <QFont>
#include <QFontDatabase>
#include <QKeyEvent>
#include <QMainWindow>
#include <QProgressBar>
#include <QSettings>
#include <QStatusBar>
#include <QTextBlock>
#include <QTextCursor>
#include <QTextDocument>
#include "command.h"
#include "dialog.h"
#include "environment.h"
#include "exception.h"
#include "general.h"
#include "mega_factory.h"

namespace base {

Dialog::Dialog(QWidget *parent)
    : DialogBase(parent),
      _prompt("> "),
      _informationColor(QColor("blue")),
      _errorColor(QColor("red")),
      _firstInformation(true),
      _gotoEnd(false)
{
    setObjectName("dialog");
    setDocument(_textDocument = new QTextDocument(this));
    restoreFont();
    writeWelcome();
    writePrompt();
    setAcceptRichText(false);
    _progressBar = new QProgressBar(this);
    _progressBar->setFormat(" %p% of %ms");
    _progressBar->hide();
    mainWindow()->statusBar()->addPermanentWidget(_progressBar);
    QObject::connect(qApp, &QApplication::focusChanged,
                     this, &Dialog::receivedFocus);
}

Dialog::~Dialog() {
    saveFont();
}

void Dialog::saveFont() {
    QTextCursor cursor = textCursor();
    QTextCharFormat format = cursor.charFormat();
    QSettings settings;
    settings.setValue("font/family", format.font().family());
    settings.setValue("font/size", format.fontPointSize());
}

void Dialog::setFont(QString family, int pt) {
    // Construct font
    QString family2 = fontExists(family) ? family : preferredFamily();
    QFont font = QFontDatabase().font(family2, QString(), pt);

    // Set cursor to use font
    QTextCursor cursor = dialog().textCursor();
    QTextCharFormat format = cursor.charFormat();
    format.setFont(font);
    cursor.setCharFormat(format);
    setTextCursor(cursor);
}

QFont Dialog::font() {
    QTextCursor cursor = dialog().textCursor();
    QTextCharFormat format = cursor.charFormat();
    return format.font();
}

inline QString defaultFamily() {
    return QGuiApplication::font().family();
}

void Dialog::restoreFont() {
    QSettings settings;
    QString family = resetFont() ?
                     preferredFamily() :
                     settings.value("font/family", preferredFamily()).toString();
    int pt = settings.value("font/size", 11).toInt();

    QTextCursor cursor = textCursor();
    QTextCharFormat format = cursor.charFormat();
    format.setFont(QFontDatabase().font(family, QString(), pt));
    cursor.setCharFormat(format);
    setTextCursor(cursor);
}

bool Dialog::resetFont() {
    QString stored = QSettings().value("font/family").toString();
    return stored.isEmpty() || stored == defaultFamily();
}

QString Dialog::preferredFamily() {
    QList<QString> preferred;
    preferred
        << "InputMonoCompressed Light"
        << "Consolas"
        << "Andale Mono"
        << "Menlo"
        << "Courier";
    for (QString family : preferred) {
        if (fontExists(family))
            return family;
    }
    return QGuiApplication::font().family();
}

bool Dialog::fontExists(QString family) {
    return QFontDatabase().font(family, QString(), 10).family() !=
           QGuiApplication::font().family();
}


void Dialog::progress(int current, int total) {
    _progressBar->setMaximum(total);
    _progressBar->setValue(current);
    _progressBar->show();
     qApp->processEvents();
}

void Dialog::finished() {
    _progressBar->hide();
    qApp->processEvents();
}

void Dialog::message(QString s) {
    mainWindow()->statusBar()->showMessage(s);
    mainWindow()->repaint();
    repaint();
}

void Dialog::information(QString s) {
    if (_firstInformation)
        _firstInformation = false;
    else
        s = "\n" + s;

    insertText(s, _informationColor);
    repaint();
}

void Dialog::errorImpl(QString s) {
    insertText("\n" + s, _errorColor);
    repaint();
}

void Dialog::keyPressEvent(QKeyEvent *event) {
    _gotoEnd = false;
    QTextCursor cursor;
    switch (event->key()) {
    case Qt::Key_Escape:
        event->accept();
        moveCursor(QTextCursor::End);
        writePrompt();
        break;
    case Qt::Key_Tab:
        event->accept();
        break;
    case Qt::Key_Backspace:
        if (bounceCursor(true))
            event->accept();
        else
            QTextEdit::keyPressEvent(event);
        break;
    case Qt::Key_Return:
    case Qt::Key_Enter:
        event->accept();
        cursor = getCursor();
        cursor.movePosition(QTextCursor::EndOfLine);
        setTextCursor(cursor);
        submitCommand();
        insertText("\n" + _prompt);
        break;
    case Qt::Key_Insert:
    case Qt::Key_Pause:
        event->accept();
        break;
    case Qt::Key_Home:
        event->accept();
        cursor = textCursor();
        cursor.setPosition(linePosition(cursorLine()) + _prompt.size());
        setTextCursor(cursor);
        break;
    case Qt::Key_Left:
        if (bounceCursor(true))
            event->accept();
        else
            QTextEdit::keyPressEvent(event);
        break;
    case Qt::Key_Right:
        if (bounceCursor(false))
            event->accept();
        else
            QTextEdit::keyPressEvent(event);
        break;
    case Qt::Key_PageUp:
    case Qt::Key_PageDown:
        event->accept();
        break;
    case Qt::Key_Up:
        event->accept();
        clearLine();
        insertText(_history.previous());
        break;
    case Qt::Key_Down:
        event->accept();
        clearLine();
        insertText(_history.next());
        break;
    default:
        QTextEdit::keyPressEvent(event);
        break;
    }
}

void Dialog::receivedFocus(QWidget *old, QWidget *now) {
    _gotoEnd = (!old && now == this);
}

void Dialog::mousePressEvent(QMouseEvent *event) {
    if (_gotoEnd)  {
        event->accept();
        moveCursor(QTextCursor::End);
        _gotoEnd = false;
    }
    else {
        QTextEdit::mousePressEvent(event);
    }
}


QMainWindow* Dialog::mainWindow() {
    return dynamic_cast<QMainWindow*>(parent());
}

QTextCursor Dialog::getCursor() {
    QTextCursor cursor = dialog().textCursor();
    QTextCharFormat format = cursor.charFormat();
    QFont font = format.font();
    QString family = font.family();
    int pt = font.pointSize();
    format.setFont(QFontDatabase().font(family, QString(), pt));
    cursor.setCharFormat(format);
    return cursor;
}

void Dialog::writePrompt() {
   insertText("\n" + _prompt);
}

void Dialog::writeWelcome() {
    MegaFactory::loadPlugins();

    information(environment().isFirstInstallation() ?
                "\nWelcome to Universal Simulator!" : "\nWelcome back!");

    environment().computationStep(ComputationStep::Start);

    if (environment().isNewInstallation()) {
        information("New installation detected; reconfiguring HOME folder...");
        Command::submit(QStringList() << "reconfigure", this);
        Command::submit(QStringList() << "set" << "folder" << "work" << "HOME", this);
        environment().updateInstallation();
    }

//    if (environment().isFirstInstallation())
//        Command::submit(QStringList() << "save" << "grammar", this);

    QString info = "Work folder:\n  " + environment().folderInfo(Environment::Work) +
                   "\nInput folder:\n  " + environment().folderInfo(Environment::Input);

    QString latestFile = environment().latestLoadArg();
    if (!latestFile.isEmpty())
        info += "\nYour latest file was '" + latestFile + "'";
    _history.add("load \"" + latestFile + "\"");
    information(info);

    environment().computationStep(ComputationStep::Ready);
}


void Dialog::insertText(QString text, QColor color) {
    setTextColor(color);
    QTextCursor cursor = textCursor();
    cursor.insertText(text);
    setTextCursor(cursor);
}

int Dialog::numLines() {
    return _textDocument->lineCount();
}

int Dialog::linePosition(int i) {
    Q_ASSERT(i<numLines());
    return _textDocument->findBlockByLineNumber(i).position();
}

int Dialog::cursorPosition() {
    return textCursor().position();
}

int Dialog::cursorLine() {
    for (int i = numLines()-1; i>=0; --i) {
        if (cursorPosition() > linePosition(i))
            return i;
    }
    Q_ASSERT(false);
    return 0;
}

bool Dialog::bounceCursor(bool sticky) {
    int start = linePosition(cursorLine()) + _prompt.size();
    QTextCursor cursor = textCursor();
    int pos = cursor.position();
    int bounce = (sticky && pos <= start) || (!sticky && pos < start);
    if (bounce) {
        cursor.setPosition(start);
        setTextCursor(cursor);
        return true;
    }
    return false;
}

void Dialog::clearLine() {
    QTextCursor cursor;
    while (!bounceCursor(true)) {
        cursor = textCursor();
        cursor.movePosition(QTextCursor::EndOfLine);
        cursor.deletePreviousChar();
        setTextCursor(cursor);
    }
}

void Dialog::submitCommand() {
    QTextBlock block = _textDocument->findBlock(cursorPosition());
    QString line = block.text().mid(_prompt.size()).simplified();
    QStringList items = base::split(line);
    if (!items.isEmpty())
        Command::submit(items, this);
    if (!line.isEmpty())
        _history.add(line);
}

}
