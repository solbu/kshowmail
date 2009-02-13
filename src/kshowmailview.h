/*
 * kshowmailview.h
 *
 * Copyright (C) 2007 %{AUTHOR} <%{EMAIL}>
 */
#ifndef KSHOWMAILVIEW_H
#define KSHOWMAILVIEW_H

#include <QtGui/QWidget>

#include "ui_kshowmailview_base.h"

class QPainter;
class KUrl;

/**
 * This is the main view class for kshowmail.  Most of the non-menu,
 * non-toolbar, and non-statusbar (e.g., non frame) GUI code should go
 * here.
 *
 * @short Main view
 * @author %{AUTHOR} <%{EMAIL}>
 * @version %{VERSION}
 */

class kshowmailView : public QWidget, public Ui::kshowmailview_base
{
    Q_OBJECT
public:
    /**
     * Default constructor
     */
    kshowmailView(QWidget *parent);

    /**
     * Destructor
     */
    virtual ~kshowmailView();

private:
    Ui::kshowmailview_base ui_kshowmailview_base;

signals:
    /**
     * Use this signal to change the content of the statusbar
     */
    void signalChangeStatusbar(const QString& text);

    /**
     * Use this signal to change the content of the caption
     */
    void signalChangeCaption(const QString& text);

private slots:
    void switchColors();
    void settingsChanged();
};

#endif // kshowmailVIEW_H
