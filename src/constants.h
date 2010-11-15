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

#ifndef CONSTANTS_H
#define CONSTANTS_H

/**
 * @brief Contains all constants.
 */
namespace Constants
{
  /**
  * Command Line Options
  */
  //option for show config dialog at start
  #define CMDLINE_CONFIG        "configure"
  #define CMDLINE_CONFIG_SHORT  "c"
  //option for looking for new mails at start
  #define CMDLINE_REFRESH       "refresh"
  #define CMDLINE_REFRESH_SHORT "r"

  /**
   * Defaults
   */
  //general defaults
  #define DEFAULT_CONFIRM_CLOSE           false
  #define DEFAULT_CONFIRM_DELETE          true
  #define DEFAULT_START_MINIMIZED         false
  #define DEFAULT_CLOSE_TO_TRAY           true
  #define DEFAULT_SHOW_CONNECTION_ERRORS  true
  #define DEFAULT_KEEP_NEW                false
  #define DEFAULT_AUTO_REFRESH            false
  #define DEFAULT_STORE_HEADERS           false

  #define DEFAULT_INITIAL_TIME    5
  #define DEFAULT_INTERVAL_TIME   5
  #define MINIMUM_TIMEOUT_TIME    5
  #define DEFAULT_TIMEOUT_TIME    30

  #define DEFAULT_MAIL_DOUBLE_CLICK_ACTION  "body"

  //default actions
  #define DEFAULT_ACTION_NEW_MAIL_ALERTWINDOW   true
  #define DEFAULT_ACTION_NEW_MAIL_MAINWINDOW    false
  #define DEFAULT_ACTION_NEW_MAIL_BEEP          false
  #define DEFAULT_ACTION_NEW_MAIL_SOUND         false
  #define DEFAULT_ACTION_NEW_MAIL_COMMAND       false
  #define DEFAULT_ACTION_NO_NEW_MAIL_MINIMIZE   false
  #define DEFAULT_ACTION_NO_NEW_MAIL_TERMINATE  false

  //default display options
  #define DEFAULT_DISPLAY_ACCOUNT_ACTIVE    true
  #define DEFAULT_DISPLAY_ACCOUNT_ACCOUNT   true
  #define DEFAULT_DISPLAY_ACCOUNT_SERVER    true
  #define DEFAULT_DISPLAY_ACCOUNT_USER      true
  #define DEFAULT_DISPLAY_ACCOUNT_MESSAGES  true
  #define DEFAULT_DISPLAY_ACCOUNT_SIZE      true
  #define DEFAULT_DISPLAY_MESSAGE_NUMBER    true
  #define DEFAULT_DISPLAY_MESSAGE_ACCOUNT   true
  #define DEFAULT_DISPLAY_MESSAGE_FROM      true
  #define DEFAULT_DISPLAY_MESSAGE_TO        true
  #define DEFAULT_DISPLAY_MESSAGE_SUBJECT   true
  #define DEFAULT_DISPLAY_MESSAGE_DATE      true
  #define DEFAULT_DISPLAY_MESSAGE_SIZE      true
  #define DEFAULT_DISPLAY_MESSAGE_CONTENT   true
  #define DEFAULT_DISPLAY_MESSAGE_STATE     true

  //default view options
  #define DEFAULT_VIEW_USE_HTML      false
  #define DEFAULT_SORT_ORDER         "asc"
  #define DEFAULT_SORT_COLUMN_ACCOUNT 1
  #define DEFAULT_SORT_COLUMN_MESSAGE 2
  #define DEFAULT_SORT_COLUMN_LOGVIEW_DELETED 0
  #define DEFAULT_SORT_COLUMN_LOGVIEW_MOVED 0

  //default column widthes
  #define DEFAULT_WIDTH_ACCOUNT_ACTIVE    50
  #define DEFAULT_WIDTH_ACCOUNT_ACCOUNT   100
  #define DEFAULT_WIDTH_ACCOUNT_SERVER    100
  #define DEFAULT_WIDTH_ACCOUNT_USER      100
  #define DEFAULT_WIDTH_ACCOUNT_MESSAGES  80
  #define DEFAULT_WIDTH_ACCOUNT_SIZE      80
  #define DEFAULT_WIDTH_MESSAGE_NUMBER    50
  #define DEFAULT_WIDTH_MESSAGE_ACCOUNT   100
  #define DEFAULT_WIDTH_MESSAGE_FROM      200
  #define DEFAULT_WIDTH_MESSAGE_TO        200
  #define DEFAULT_WIDTH_MESSAGE_SUBJECT   350
  #define DEFAULT_WIDTH_MESSAGE_DATE      100
  #define DEFAULT_WIDTH_MESSAGE_SIZE      80
  #define DEFAULT_WIDTH_MESSAGE_CONTENT   80
  #define DEFAULT_WIDTH_MESSAGE_STATE     50

  //default column widthes of the log view
  #define DEFAULT_WIDTH_LOGVIEW_DELETED_BY 20
  #define DEFAULT_WIDTH_LOGVIEW_DELETED_DATE 80
  #define DEFAULT_WIDTH_LOGVIEW_DELETED_FROM 80
  #define DEFAULT_WIDTH_LOGVIEW_DELETED_SUBJECT 80
  #define DEFAULT_WIDTH_LOGVIEW_DELETED_ACCOUNT 80
  #define DEFAULT_WIDTH_LOGVIEW_MOVED_DATE 80
  #define DEFAULT_WIDTH_LOGVIEW_MOVED_FROM 80
  #define DEFAULT_WIDTH_LOGVIEW_MOVED_SUBJECT 80
  #define DEFAULT_WIDTH_LOGVIEW_MOVED_ACCOUNT 80
  #define DEFAULT_WIDTH_LOGVIEW_MOVED_MAILBOX 80



  //account defaults
  #define DEFAULT_ACCOUNT_NAME ""
  #define DEFAULT_ACCOUNT_SERVER ""
  #define DEFAULT_ACCOUNT_PROTOCOL "POP3"
  #define DEFAULT_ACCOUNT_PORT_POP3 110
  #define DEFAULT_ACCOUNT_PORT_POP3SSL 995
  #define DEFAULT_ACCOUNT_USER ""
  #define DEFAULT_ACCOUNT_PASSWORD ""
  #define DEFAULT_ACCOUNT_PASSWORD_STORAGE  1
    //1: don't save, 2: save in file, 3: use KWallet
  #define DEFAULT_ACCOUNT_ACTIVE true
  #define DEFAULT_ACCOUNT_SECTRANSFER 1
    //1: no secure transfer, 2: SSL, 3: TLS
  #define DEFAULT_ACCOUNT_ALLOW_UNSECURE_LOGIN false

  //filter defaults
  #define DEFAULT_FILTER_ACTIVE false
  #define DEFAULT_FILTER_OTHERS_ACTION  1
    //1: Pass, 2: Delete, 3: Mark, 4: Move to Mailbox, 5: Spamcheck, 6: Ignore
  #define DEFAULT_FILTER_BLACKLIST_ACTION 1
    //1: Delete, 2: Mark
  #define DEFAULT_FILTER_NAME "New Filter"
  #define DEFAULT_FILTER_CRITERIA_LINKAGE 1
    //1: Match all (AND); 2: Match any (OR)
  #define DEFAULT_FILTER_CRITERIA_SOURCE 1
    //1: From, 2: To, 3: Size (Bytes), 4: Subject, 5: Header, 6: Account
  #define DEFAULT_FILTER_CRITERIA_COND_TEXT 1
    //1: contains, 2: doesn't contains, 3: equals, 4: doesn't equal, 5: matches regular expression, 6: doesn't match regular expression
  #define DEFAULT_FILTER_CRITERIA_COND_NUM 3
    //1: is equal to, 2: is not equal to, 3: is greater than, 4: is greater than or equal to, 5: is less than, 6: is less than or equal to
  #define DEFAULT_FILTER_CRITERIA_SIZE 20000
  #define DEFAULT_FILTER_CRITERIA_CASE_SENSITIVE false
  #define DEFAULT_FILTER_ACTION  1
    //1: Pass, 2: Delete, 3: Mark, 4: Move to Mailbox, 5: Spamcheck, 6: Ignore
  #define DEFAULT_FILTER_ACTION_MOVE_MAILBOX ""

  //Spam check defaults
  #define DEFAULT_SPAMCHECK_ACTION 3
    //2: Delete, 3: Mark, 4: Move to Mailbox
  #define DEFAULT_SPAMCHECK_ACTION_MOVE_MAILBOX ""


  //Log Defaults
  #define DEFAULT_LOG_LOG_DELETED_MAILS         true
  #define DEFAULT_LOG_LOG_MOVED_MAILS           true
  #define DEFAULT_LOG_LOG_MANUAL_DELETED_MAILS  true
  //the log entires will be deleted after some days (constant value is "days") or at app exit (value is "exit")
  #define DEFAULT_LOG_REMOVE_DELETED_MAILS    "days"
  #define DEFAULT_LOG_REMOVE_MOVED_MAILS      "exit"
  #define DEFAULT_LOG_REMOVE_MANUAL_DELETED_MAILS    "days"
    //number of days the entries will be stored
  #define DEFAULT_LOG_HOLDDAYS_DELETED_MAILS  7
  #define DEFAULT_LOG_HOLDDAYS_MOVED_MAILS    7
  #define DEFAULT_LOG_HOLDDAYS_MANUAL_DELETED_MAILS  7

  /**
   * XML file to store mails
   */
  #define MAIL_FILE               "mails.xml"
  #define MAIL_FILE_DOCTYPE       "KShowmail"
  #define ROOT_ELEMENT            "accounts"
  #define ACCOUNT_ELEMENT         "account"
  #define ATTRIBUTE_ACCOUNT_NAME  "account"
  #define ATTRIBUTE_MAIL_UID      "uidl"
  #define ATTRIBUTE_MAIL_SIZE     "size"
  #define ATTRIBUTE_MAIL_NUMBER   "number"
  #define ITEM_MAIL_HEADER        "header"
  #define ITEM_MESSAGE            "message"

  /**
   * Log
   */
  #define LOG_FILE          "kshowmaillog.xml"
  #define LOG_DOCTYPE       "KShowmailLog"
  #define LOG_ROOT_ELEMENT  "DeletedMails"
  #define LOG_ENTRY_ELEMENT "LogEntry"
  #define LOG_ENTRY_ATTRIBUTE_DATETIME "DateTime"
  #define LOG_ENTRY_ATTRIBUTE_SENDER "Sender"
  #define LOG_ENTRY_ATTRIBUTE_ACCOUNT "Account"
  #define LOG_ENTRY_ATTRIBUTE_SUBJECT "Subject"
  #define LOG_ENTRY_ATTRIBUTE_KIND_DELETE "KindDelete"
  #define LOG_ENTRY_VALUE_KIND_DELETE_MANUAL "manual"
  #define LOG_ENTRY_VALUE_KIND_DELETE_FILTER "filter"
  #define LOG_ENTRY_ATTRIBUTE_FILTER "Filter"

  /**
   * Geometry
   */
  //Size of the view, which shows the mail text
  #define WIDTH_VIEW_MAILBODY         500
  #define HEIGHT_VIEW_MAILBODY        400

  //Size of the view, which shows the mail header
  #define WIDTH_VIEW_MAILHEADER   500
  #define HEIGHT_VIEW_MAILHEADER  400

  //Size of the text comparative value line of the filter criteria widget
  #define WIDTH_FILTER_TEXT_VALUE_LINE  300

  /**
   * Configuration file
   */
  #define CONFIG_GROUP_GENERAL                  "General"
  #define CONFIG_ENTRY_CONFIRM_CLOSE            "confirmClose"
  #define CONFIG_ENTRY_CONFIRM_DELETE           "confirmDelete"
  #define CONFIG_ENTRY_START_MINIMIZED          "startMinimized"
  #define CONFIG_ENTRY_CLOSE_TO_TRAY            "closeToTray"
  #define CONFIG_ENTRY_SHOW_CONNECTION_ERRORS   "showConnectionErrors"
  #define CONFIG_ENTRY_KEEP_NEW                 "keepMailsNew"
  #define CONFIG_ENTRY_INITIAL_TIME             "InitialTime"
  #define CONFIG_ENTRY_INTERVAL_TIME            "IntervalTime"
  #define CONFIG_ENTRY_TIMEOUT_TIME             "TimeoutTime"
  #define CONFIG_ENTRY_AUTO_REFRESH             "AutomaticRefresh"
  #define CONFIG_ENTRY_STORE_HEADERS            "storeHeaders"
  #define CONFIG_ENTRY_MAIL_DOUBLE_CLICK_ACTION "mailDoubleClickAction"
  #define CONFIG_VALUE_MAIL_DOUBLE_CLICK_ACTION_BODY "body"
  #define CONFIG_VALUE_MAIL_DOUBLE_CLICK_ACTION_HEADER "header"

  #define CONFIG_GROUP_ACTIONS                "Actions"
  #define CONFIG_ENTRY_NEW_MAIL_ALERTWINDOW   "showAlertMessageIfNewMail"
  #define CONFIG_ENTRY_NEW_MAIL_MAINWINDOW    "showMainWindowIfNewMail"
  #define CONFIG_ENTRY_NEW_MAIL_BEEP          "beepIfNewMail"
  #define CONFIG_ENTRY_NEW_MAIL_SOUND         "playSoundIfNewMail"
  #define CONFIG_ENTRY_NEW_MAIL_SOUNDPATH     "PathToSoundFile"
  #define CONFIG_ENTRY_NEW_MAIL_COMMAND       "executeCommandIfNewMail"
  #define CONFIG_ENTRY_NEW_MAIL_COMMANDPATH   "Command"
  #define CONFIG_ENTRY_NO_NEW_MAIL_TERMINATE  "exitIfNoNewMails"
  #define CONFIG_ENTRY_NO_NEW_MAIL_MINIMIZE   "minimizeMainWindowIfNoNewMails"

  #define CONFIG_GROUP_VIEW                "View"
  #define CONFIG_ENTRY_VIEW_USE_HTML       "useHTML"
  #define CONFIG_ENTRY_VIEW_MAIN_WINDOW_SPLITTER "MainWindowSplitter"

  #define CONFIG_VALUE_SORT_ORDER_ASCENDING				"asc"
  #define CONFIG_VALUE_SORT_ORDER_DESCENDING      "desc"

  #define CONFIG_GROUP_ACCOUNT_LIST               "LayoutAccountList"
  #define CONFIG_ENTRY_DISPLAY_ACCOUNT_ACTIVE     "showAccountListActiveColumn"
  #define CONFIG_ENTRY_DISPLAY_ACCOUNT_ACCOUNT    "showAccountListAccountColumn"
  #define CONFIG_ENTRY_DISPLAY_ACCOUNT_SERVER     "showAccountListServerColumn"
  #define CONFIG_ENTRY_DISPLAY_ACCOUNT_USER       "showAccountListUserColumn"
  #define CONFIG_ENTRY_DISPLAY_ACCOUNT_MESSAGES   "showAccountListMessagesColumn"
  #define CONFIG_ENTRY_DISPLAY_ACCOUNT_SIZE       "showAccountListSizeColumn"
  #define CONFIG_ENTRY_WIDTH_ACCOUNT_ACTIVE   		"widthAccountListActiveColumn"
  #define CONFIG_ENTRY_WIDTH_ACCOUNT_ACCOUNT  		"widthAccountListAccountColumn"
  #define CONFIG_ENTRY_WIDTH_ACCOUNT_SERVER   		"widthAccountListServerColumn"
  #define CONFIG_ENTRY_WIDTH_ACCOUNT_USER     		"widthAccountListUserColumn"
  #define CONFIG_ENTRY_WIDTH_ACCOUNT_MESSAGES 		"widthAccountListMessagesColumn"
  #define CONFIG_ENTRY_WIDTH_ACCOUNT_SIZE     		"widthAccountListSizeColumn"
  #define CONFIG_ENTRY_SORT_COLUMN_ACCOUNT				"sortColumnAccountList"
  #define CONFIG_ENTRY_SORT_ORDER_ACCOUNT					"sortOrderAccountList"


  #define CONFIG_GROUP_MESSAGE_LIST               "LayoutMessageList"
  #define CONFIG_ENTRY_DISPLAY_MESSAGE_NUMBER     "showMessageListNumberColumn"
  #define CONFIG_ENTRY_DISPLAY_MESSAGE_ACCOUNT    "showMessageListAccountColumn"
  #define CONFIG_ENTRY_DISPLAY_MESSAGE_FROM       "showMessageListFromColumn"
  #define CONFIG_ENTRY_DISPLAY_MESSAGE_TO         "showMessageListToColumn"
  #define CONFIG_ENTRY_DISPLAY_MESSAGE_SUBJECT    "showMessageListSubjectColumn"
  #define CONFIG_ENTRY_DISPLAY_MESSAGE_DATE       "showMessageListDateColumn"
  #define CONFIG_ENTRY_DISPLAY_MESSAGE_SIZE       "showMessageListSizeColumn"
  #define CONFIG_ENTRY_DISPLAY_MESSAGE_CONTENT    "showMessageListContentColumn"
  #define CONFIG_ENTRY_DISPLAY_MESSAGE_STATE      "showMessageListStateColumn"
  #define CONFIG_ENTRY_WIDTH_MESSAGE_NUMBER   		"widthMessageListNumberColumn"
  #define CONFIG_ENTRY_WIDTH_MESSAGE_ACCOUNT  		"widthMessageListAccountColumn"
  #define CONFIG_ENTRY_WIDTH_MESSAGE_FROM     		"widthMessageListFromColumn"
  #define CONFIG_ENTRY_WIDTH_MESSAGE_TO       		"widthMessageListToColumn"
  #define CONFIG_ENTRY_WIDTH_MESSAGE_SUBJECT  		"widthMessageListSubjectColumn"
  #define CONFIG_ENTRY_WIDTH_MESSAGE_DATE     		"widthMessageListDateColumn"
  #define CONFIG_ENTRY_WIDTH_MESSAGE_SIZE     		"widthMessageListSizeColumn"
  #define CONFIG_ENTRY_WIDTH_MESSAGE_CONTENT  		"widthMessageListContentColumn"
  #define CONFIG_ENTRY_WIDTH_MESSAGE_STATE    		"widthMessageListStateColumn"
  #define CONFIG_ENTRY_SORT_COLUMN_MESSAGE				"sortColumnMessageList"
  #define CONFIG_ENTRY_SORT_ORDER_MESSAGE					"sortOrderMessageList"

  #define CONFIG_VALUE_ACCOUNT_PASSWORD_DONT_SAVE     1
  #define CONFIG_VALUE_ACCOUNT_PASSWORD_SAVE_FILE     2
  #define CONFIG_VALUE_ACCOUNT_PASSWORD_SAVE_KWALLET  3
  #define CONFIG_VALUE_ACCOUNT_SECTRANSFER_NONE       1
  #define CONFIG_VALUE_ACCOUNT_SECTRANSFER_SSL        2
  #define CONFIG_VALUE_ACCOUNT_SECTRANSFER_TLS        3

  #define CONFIG_GROUP_ACCOUNTS      "Accounts"
  #define CONFIG_ENTRY_ACCOUNTS_LIST "NamesList"

  #define CONFIG_ENTRY_ACCOUNT_NAME             "Name"
  #define CONFIG_ENTRY_ACCOUNT_SERVER           "Server"
  #define CONFIG_ENTRY_ACCOUNT_PROTOCOL         "Protocol"
  #define CONFIG_ENTRY_ACCOUNT_PORT             "Port"
  #define CONFIG_ENTRY_ACCOUNT_USER             "User"
  #define CONFIG_ENTRY_ACCOUNT_PASSWORD         "Password"
  #define CONFIG_ENTRY_ACCOUNT_PASSWORD_STORAGE "PasswordStorage"
  #define CONFIG_ENTRY_ACCOUNT_ACTIVE           "Active"
  #define CONFIG_ENTRY_ACCOUNT_SECTRANSFER      "SecureTransfer"
  #define CONFIG_ENTRY_ACCOUNT_ALLOW_UNSECURE_LOGIN "AllowUnsecureLogin"

  #define CONFIG_VALUE_ACCOUNT_PROTOCOL_POP3    "POP3"


  //general filter
  #define CONFIG_GROUP_FILTER "Filter"

  #define CONFIG_ENTRY_FILTER_ACTIVE  "Active"

  #define CONFIG_VALUE_FILTER_OTHERS_ACTION_PASS        1
  #define CONFIG_VALUE_FILTER_OTHERS_ACTION_DELETE      2
  #define CONFIG_VALUE_FILTER_OTHERS_ACTION_MARK        3
  #define CONFIG_VALUE_FILTER_OTHERS_ACTION_MOVE        4
  #define CONFIG_VALUE_FILTER_OTHERS_ACTION_SPAMCHECK   5
  #define CONFIG_VALUE_FILTER_OTHERS_ACTION_IGNORE      6

  #define CONFIG_ENTRY_FILTER_OTHERS_ACTION   "ActionForOthers"
  #define CONFIG_ENTRY_FILTER_OTHERS_MAILBOX  "MailboxForOthers"

  #define CONFIG_VALUE_FILTER_BLACKLIST_ACTION_DELETE  1
  #define CONFIG_VALUE_FILTER_BLACKLIST_ACTION_MARK    2

  #define CONFIG_ENTRY_FILTER_BLACKLIST_ACTION  "ActionBlacklist"
  #define CONFIG_ENTRY_FILTER_BLACKLIST         "Blacklist"
  #define CONFIG_ENTRY_FILTER_WHITELIST         "Whitelist"
  #define CONFIG_ENTRY_FILTER_NUMBER_OF_FILTERS "NumberFilters"

  //single filter
  #define CONFIG_ENTRY_FILTER_NAME "Name"
  #define CONFIG_ENTRY_FILTER_CRITERIA_LINKAGE "CriteriaLinkage"
  #define CONFIG_ENTRY_FILTER_CRITERIA_NUMBER "CriteriaNumber"
  #define CONFIG_ENTRY_FILTER_CRITERIA_SOURCE "CriteriaSource"
  #define CONFIG_ENTRY_FILTER_CRITERIA_CONDITION "CriteriaCondition"
  #define CONFIG_ENTRY_FILTER_CRITERIA_VALUE "CriteriaValue"
  #define CONFIG_ENTRY_FILTER_CRITERIA_CASESENSITIVE "CriteriaCaseSensitive"
  #define CONFIG_ENTRY_FILTER_ACTION "Action"
  #define CONFIG_ENTRY_FILTER_MOVE_MAILBOX "Mailbox"

  #define CONFIG_VALUE_FILTER_CRITERIA_LINKAGE_MATCH_ALL 1
  #define CONFIG_VALUE_FILTER_CRITERIA_LINKAGE_MATCH_ANY 2

  #define CONFIG_VALUE_FILTER_CRITERIA_SOURCE_FROM    1
  #define CONFIG_VALUE_FILTER_CRITERIA_SOURCE_TO      2
  #define CONFIG_VALUE_FILTER_CRITERIA_SOURCE_SIZE    3
  #define CONFIG_VALUE_FILTER_CRITERIA_SOURCE_SUBJECT 4
  #define CONFIG_VALUE_FILTER_CRITERIA_SOURCE_HEADER  5
  #define CONFIG_VALUE_FILTER_CRITERIA_SOURCE_ACCOUNT 6

  #define CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_CONTAINS     1
  #define CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_NOT_CONTAINS 2
  #define CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_EQUALS       3
  #define CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_NOT_EQUALS   4
  #define CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_REGEXPR      5
  #define CONFIG_VALUE_FILTER_CRITERIA_COND_TEXT_NOT_REGEXPR  6

  #define CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_EQUAL         1
  #define CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_NOT_EQUAL     2
  #define CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_GREATER       3
  #define CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_GREATER_EQUAL 4
  #define CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_LESS          5
  #define CONFIG_VALUE_FILTER_CRITERIA_COND_NUM_LESS_EQUAL    6

  #define CONFIG_VALUE_FILTER_ACTION_PASS        1
  #define CONFIG_VALUE_FILTER_ACTION_DELETE      2
  #define CONFIG_VALUE_FILTER_ACTION_MARK        3
  #define CONFIG_VALUE_FILTER_ACTION_MOVE        4
  #define CONFIG_VALUE_FILTER_ACTION_SPAMCHECK   5
  #define CONFIG_VALUE_FILTER_ACTION_IGNORE      6

  //Spamcheck
  #define CONFIG_GROUP_SPAMCHECK "Spamcheck"

  #define CONFIG_ENTRY_SPAMCHECK_ACTION "Action"
  #define CONFIG_ENTRY_SPAMCHECK_MOVE_MAILBOX "Mailbox"

  #define CONFIG_VALUE_SPAMCHECK_ACTION_DELETE      2
  #define CONFIG_VALUE_SPAMCHECK_ACTION_MARK        3
  #define CONFIG_VALUE_SPAMCHECK_ACTION_MOVE        4

  //Log
  #define CONFIG_GROUP_LOG                                "Log"
  #define CONFIG_ENTRY_LOG_LOG_DELETED_MAILS              "LogDeletedMails"
  #define CONFIG_ENTRY_LOG_LOG_MOVED_MAILS                "LogMovedMails"
  #define CONFIG_ENTRY_LOG_LOG_MANUAL_DELETED_MAILS       "LogManualDeletedMails"
  #define CONFIG_ENTRY_LOG_REMOVE_DELETED_MAILS           "RemoveEventOfDeletedMails"
  #define CONFIG_ENTRY_LOG_REMOVE_MOVED_MAILS             "RemoveEvendOfMovedMails"
  #define CONFIG_ENTRY_LOG_REMOVE_MANUAL_DELETED_MAILS    "RemoveEventOfManualDeletedMails"
  #define CONFIG_ENTRY_LOG_HOLDDAYS_DELETED_MAILS         "HoldDaysOfDeletedMails"
  #define CONFIG_ENTRY_LOG_HOLDDAYS_MOVED_MAILS           "HoldDaysOfMovedMails"
  #define CONFIG_ENTRY_LOG_HOLDDAYS_MANUAL_DELETED_MAILS  "HoldDaysOfManualDeletedMails"

  #define CONFIG_VALUE_LOG_REMOVE_MAILS_AT_EXIT     "exit"
  #define CONFIG_VALUE_LOG_REMOVE_MAILS_AFTER_DAYS  "days"

  //Log view
  #define CONFIG_GROUP_LOGVIEW                        "LogView"
  #define CONFIG_ENTRY_WIDTH_LOGVIEW_DELETED_BY       "WidthLogViewDeletedBy"
  #define CONFIG_ENTRY_WIDTH_LOGVIEW_DELETED_DATE     "WidthLogViewDeletedDate"
  #define CONFIG_ENTRY_WIDTH_LOGVIEW_DELETED_SUBJECT  "WidthLogViewDeletedSubject"
  #define CONFIG_ENTRY_WIDTH_LOGVIEW_DELETED_ACCOUNT  "WidthLogViewDeletedAccount"
  #define CONFIG_ENTRY_WIDTH_LOGVIEW_DELETED_FROM     "WidthLogViewDeletedFrom"
  #define CONFIG_ENTRY_WIDTH_LOGVIEW_MOVED_DATE       "WidthLogViewMovedDate"
  #define CONFIG_ENTRY_WIDTH_LOGVIEW_MOVED_SUBJECT    "WidthLogViewMovedSubject"
  #define CONFIG_ENTRY_WIDTH_LOGVIEW_MOVED_ACCOUNT    "WidthLogViewMovedAccount"
  #define CONFIG_ENTRY_WIDTH_LOGVIEW_MOVED_FROM       "WidthLogViewMovedFrom"
  #define CONFIG_ENTRY_WIDTH_LOGVIEW_MOVED_MAILBOX    "WidthLogViewMovedMailbox"
  #define CONFIG_ENTRY_SORT_COLUMN_LOGVIEW_DELETED    "sortColumnLogViewDeleted"
  #define CONFIG_ENTRY_SORT_COLUMN_LOGVIEW_MOVED      "sortColumnLogViewMoved"
  #define CONFIG_ENTRY_SORT_ORDER_LOGVIEW_DELETED     "sortOrderLogViewDeleted"
  #define CONFIG_ENTRY_SORT_ORDER_LOGVIEW_MOVED       "sortOrderLogViewMoved"


  //Status Bar
  #define STATUSBAR_FIELD_STATE         1
  #define STATUSBAR_FIELD_FILTER        2
  #define STATUSBAR_FIELD_NEXT_REFRESH  3
  #define STATUSBAR_FIELD_LAST_REFRESH  4


}
#endif
