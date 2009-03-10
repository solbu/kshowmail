/*
 * kshowmail.h
 *
 * Copyright (C) 2008 %{AUTHOR} <%{EMAIL}>
 */
#ifndef KSHOWMAIL_H
#define KSHOWMAIL_H


#include <kxmlguiwindow.h>


class QPrinter;
class KToggleAction;
class KUrl;

/**
 * This class serves as the main window for kshowmail.  It handles the
 * menus, toolbars, and status bars.
 *
 * @short Main window class
 * @author %{AUTHOR} <%{EMAIL}>
 * @version %{VERSION}
 */
class kshowmail : public KXmlGuiWindow
{
    Q_OBJECT
public:
    /**
     * Default Constructor
     */
    kshowmail();

    /**
     * Default Destructor
     */
    virtual ~kshowmail();



};

#endif // _KSHOWMAIL_H_
