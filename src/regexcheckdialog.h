/*
Copyright 2010  Ulrich Weigelt <ulrich.weigelt@gmx.de>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of
the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef REGEXCHECKDIALOG_H
#define REGEXCHECKDIALOG_H

//qt headers
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QFont>
#include <QPalette>
#include <QRegExp>

//KDE headers
#include <klocale.h>
#include <klineedit.h>
#include <KMessageBox>

//KShowmail headers
#include <KDialog>

/**
 * @brief This is a small dialog to edit and check a regular expression
 **/
class RegexCheckDialog : public KDialog {

  Q_OBJECT

  public:

    /**
     * Constructor
     * @param parent parent widget
     */
     RegexCheckDialog( QWidget* parent );

     /**
      * Returns the entered regex
      * @return the entered regex
      */
     QString getRegex();

     /**
      * Sets the regex into the edit line
      *
      * @param regex the regex
      */
     void setRegex( QString regex );

  private:

    /**
     * @brief Edit line to enter the regular expression.
     */
    KLineEdit* txtRegEx;

    /**
     * @brief Edit line to enter a test string
     **/
    KLineEdit* txtTestString;

    /**
     * @brief In this line we show the result of the regex check.
     **/
    QLabel* txtResult;

  private slots:

    /**
     * @brief Activated when a button is clicked.
     *
     * Overrides <em>KDialog::slotButtonClicked(int)</em>
     * @param button clicked button (see KDialog::ButtonCode)
     **/
    void slotButtonClicked( int button );

    /**
     * @brief Checkes the regex
     *
     * This methode gets the regex and the test phrases and test whether the
     * regex matches the phrase. Writes the result into <em>txtResult</em>.
     */
    void checkRegex();

  private:

    /**
     * @brief Writes a black colored text into the result field.
     *
     * @param text text to write
     */
    void writeBlack( QString text );

    /**
     * @brief Writes a green colored text into the result field.
     *
     * @param text text to write
     */
    void writeGreen( QString text );

    /**
     * @brief Writes a red colored text into the result field.
     *
     * @param text text to write
     */
    void writeRed( QString text );

};

#endif // REGEXCHECKDIALOG_H
