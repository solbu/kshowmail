//
// C++ Interface: showheaderdialog
//
// Description:
//
//
// Author: Ulrich Weigelt <ulrich.weigelt@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SHOWHEADERDIALOG_H
#define SHOWHEADERDIALOG_H

//QT headers
#include <qlayout.h>
#include <qlabel.h>
#include <QStringList>

//KDE headers
#include <KDialog>
#include <klocale.h>
#include <klineedit.h>
#include <ktextedit.h>

//KShowmail headers
#include "constants.h"

/**
 * Used in ShowRecordElem to show the mail header.
 *
 * @author Ulrich Weigelt <ulrich.weigelt@gmx.de>
 */
class ShowHeaderDialog : public KDialog
{
Q_OBJECT

  public:
    /**
     * Generic constructor.
     * @param parent parent of the dialog
     * @param caption the dialog caption
     * @param subject mail subject
     * @param header mail header
     */
    ShowHeaderDialog( QWidget* parent, QString& caption, QString& subject, QStringList header );

    /**
     * Destructor
     */
    ~ShowHeaderDialog();
};

#endif
