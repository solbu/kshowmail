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

#ifndef TYPES_H
#define TYPES_H

//Qt headers
#include <QList>
#include <qmap.h>
#include <qstring.h>

using namespace std;

/**
 * @brief All special types for KShowMail.
 */
namespace Types
{
     /**
      * State of the application during running time.
      */
     enum State_Type {idle,                  /**<it's doing nothing*/

                      deleting,              /**<it's deleting a message on the server*/

                      configure,             /**<the user is configuring the application or
                                              * the mail or just the header of the mail is
                                              * being shown*/

                      showing,               /**<it's downloading and showing a mail*/

                      refreshing             /**<it's refreshing the mail lists*/
     };

     /**
      * State of an account.
      */
     enum AccountState_Type
     {
       AccountIdle,         /**<it's doing nothing*/

       AccountDeleting,     /**<a deletion is running*/

       AccountDownloading,  /**<the account is downloading mail bodies*/

       AccountRefreshing    /**<the account is refreshing its mail list*/
     };

     /**
      * List of mail numbers. It is a QValueList of integers.
      */
     typedef QList<int> MailNumberList_Type;

     /**
      * Map to notice which accounts need to get an order to do a task.
      * The Key is the account name and the data a boolean.
      * TRUE means the task was ordered, FALSE the task was ended.
      */
     typedef QMap<QString, bool> AccountTaskMap_Type;

    /**
     * Actions returned by filters.
     */
     enum FilterAction_Type
     {
       FActPass,           /**<Mail has passed the filter check. It can showed in the list.*/
       FActDelete,         /**<Undesirable mail. Delete it.*/
       FActMark,           /**<Probably undesirable mail. Mark it in the mail list.*/
       FActMove,           /**<Move the mail to a certain mail box. The name of the box will returned by a further parameter.*/
       FActSpamcheck,      /**<This mail shall be forwared to a spam filter, which checks the whole mail including body.*/
       FActIgnore,         /**<This mail shall not be shown.*/
       FActNone            /**<The mail doesn't match up with the filter. Perform the next one.*/
     };

     /**
      * @brief This struct is for use in MailToDownloadMap_Type and holds some parameters about the mail for
      * downloading.
      */
     struct DownloadActionParams_Type
     {
       FilterAction_Type action;  /**<the filter action*/
       QString mailbox;            /**<name of the mailbox if action is MOVE*/
     };

     /**
      * @brief This map is used by ConfigElem and ShowRecord to hold a list of mail numbers which shall be downloaded for
      * some filter actions; at time for moving and spam check
      */
     typedef QMap<int, DownloadActionParams_Type> MailToDownloadMap_Type;

    /**
     * @brief Contains all values of a filter criteria.
     */
     struct FilterCriteria_Type
     {
       int source;       /**<Object of comparison; see constants.h for valid values. (CONFIG_VALUE_FILTER_CRITERIA_SOURCE...)*/
       int condition;    /**<Condition of the comparison. see constants.h for valid values. (CONFIG_VALUE_FILTER_CRITERIA_COND...)*/
       uint numValue;    /**<Value of a numeric criteria, e.g. Size*/
       QString txtValue; /**<Value of a text criteria, e.g. Subject*/
       bool cs;          /**<TRUE - Comparison is case sensitive. Just used for numeric criteria.*/
     };

    /**
     * List of filter criterias.
     */
    typedef list<FilterCriteria_Type> FilterCriteriaList_Type;
		
		/**
		 * Transfer Security Types
		 */
		enum TransferSecurity_Type
		{
			TransSecNone,			/**<no transfer security*/
			TransSecSSL,			/**<use SSL*/
			TransSecTLS				/**<use TLS*/
		};
		
		/**
		 * Password Storage Type
		 */
		enum PasswordStorage_Type
		{
			PwStorageNone,		/**<no password storage*/
			PwStorageFile,		/**<save password into the config file*/
			PwStorageKWallet	/**<use KWallet*/
		};
		
		/**
		 * Account list sorting<p>
		 * The members of this enum represent the columns of the account list which shall be sorted.
		 */
		enum AccountSort_Type {
		
			AccSortActive,		/**<sort by active state*/
			AccSortName,			/**<sort by name*/
			AccSortServer,		/**<sort by server*/
			AccSortUser,			/**<sort by user*/
			AccSortNrMess,		/**<sort by the number of messages*/
			AccSortSize				/**<sort by the total size of messages*/
		};
		
		/**
		 * Mail list sorting<p>
		 * The members of this enum represent the columns of the mail list which shall be sorted.
		 */
		enum MailSort_Type {
		
			MailSortNumber,			/**<sort by mail number*/
			MailSortState,			/**<sort by mail state*/
			MailSortAccount,		/**<sort by account*/
			MailSortFrom,				/**<sort by sender*/
			MailSortTo,					/**<sort by addressee*/
			MailSortSubject,		/**<sort by subject*/
			MailSortDate,				/**<sort by date*/
			MailSortSize,				/**<sort by size*/
			MailSortContent			/**<sort by content type*/
		};

    /**
     * @brief Log view sorting
     *
     * The members of this enum represent the columns of the log view list which shall be sorted.
     */
    enum LogViewSort {

      LogViewSortDate,    /**<sort by date*/
      LogViewSortFrom,    /**<sort by sender*/
      LogViewSortAccount, /**<sort by account*/
      LogViewSortSubject, /**<sort by subject*/
      LogViewSortMailbox  /**<sort by mailbox*/
    };
     

}

#endif
