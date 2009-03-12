/*
 * kshowmail.cpp
 *
 * Copyright (C) 2008 %{AUTHOR} <%{EMAIL}>
 */

#include "kshowmail.h"

KShowmail::KShowmail() : KXmlGuiWindow()
{

    // add a status bar
    //statusBar()->show();

    // a call to KXmlGuiWindow::setupGUI() populates the GUI
    // with actions, using KXMLGUI.
    // It also applies the saved mainwindow settings, if any, and ask the
    // mainwindow to automatically save settings if changed: window size,
    // toolbar position, icon size, etc.
    setupGUI();
}

KShowmail::~KShowmail()
{
}

#include "kshowmail.moc"
