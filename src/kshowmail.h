/*
 * kshowmail.h
 *
 * Copyright (C) 2008 %{AUTHOR} <%{EMAIL}>
 */
#ifndef KSHOWMAIL_H
#define KSHOWMAIL_H


//KDE headers
#include <kstatusbar.h>
#include <KDE/KLocale>
#include <kxmlguiwindow.h>

//KShowmail headers
#include "kshowmailview.h"

/**
 * This class serves as the main window for kshowmail.
 *
 * @short main class
 * @author Eggert Ehmke
 * @author Ulrich Weigelt
 */
class KShowmail : public KXmlGuiWindow
{
    Q_OBJECT

  public:

    /**
     * Default Constructor
     */
    KShowmail();

    /**
     * Default Destructor
     */
    virtual ~KShowmail();
		
	private:
		
		/**
		 * This view is the central widget of KShowmail
		 */
		KShowmailView* view;

};

#endif // _KSHOWMAIL_H_
