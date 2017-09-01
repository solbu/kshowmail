set(kshowmail_SRCS
   kshowmail.cpp
   kshowmailview.cpp
   accountlist.cpp
   main.cpp
   uniqueapp.cpp
   accountviewmodel.cpp
   mailviewmodel.cpp
   mail.cpp
   account.cpp
   accountviewdelegate.cpp
   encryption.cpp
   kwalletaccess.cpp
   maillist.cpp
   exception.cpp
   corruptdataexception.cpp
   filterlogentry.cpp
   filterlogview.cpp
   filterlog.cpp
   headerfilter.cpp
   filteritem.cpp
   filteritemcriteria.cpp
   senderlistfilter.cpp
   showheaderdialog.cpp
   showmaildialog.cpp
   systemtrayicon.cpp
   accountsetupdialogcontext.cpp
   newmaildialog.cpp
   accountviewitem.cpp
   filterlogviewdeletedmodel.cpp
   filterlogviewmovedmodel.cpp
 )

#enable exceptions
SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${KDE4_ENABLE_EXCEPTIONS}" )

add_executable(kshowmail ${kshowmail_SRCS})

target_link_libraries(kshowmail ${KDE4_KDEUI_LIBS} ${KDE4_KUTILS_LIBS} ${KDEPIMLIBS_KMIME_LIBS} KF5::KIOWidgets ${KDE4_PHONON_LIBS} )

install(TARGETS kshowmail ${INSTALL_TARGETS_DEFAULT_ARGS} )

########## kcm libraries ################
set( CONFIGACCOUNTS_SRCS
     configaccounts.cpp
     accountsetupitem.cpp
     encryption.cpp
     kwalletaccess.cpp
     accountsetupdialog.cpp
      )

add_library(kcm_kshowmailconfigaccounts MODULE ${CONFIGACCOUNTS_SRCS} )
target_link_libraries(kcm_kshowmailconfigaccounts  ${KDE4_KDEUI_LIBS})

set( CONFIGACTIONS_SRCS
     configactions.cpp
   )

add_library(kcm_kshowmailconfigactions MODULE ${CONFIGACTIONS_SRCS} )
target_link_libraries( kcm_kshowmailconfigactions ${KDE4_KDEUI_LIBS} ${KDE4_PHONON_LIBS} KF5::KIOWidgets )

set( CONFIGDISPLAY_SRCS
     configdisplay.cpp
   )

add_library(kcm_kshowmailconfigdisplay MODULE ${CONFIGDISPLAY_SRCS} )
target_link_libraries( kcm_kshowmailconfigdisplay ${KDE4_KDEUI_LIBS} )

set( CONFIGFILTER_SRCS
     configfilter.cpp
     senderlistdialog.cpp
     filtersetupitem.cpp
     filtersetupdialog.cpp
     mailboxwizard.cpp
     mailboxwizardlistitem.cpp
     filtercriteriawidget.cpp
     regexcheckdialog.cpp
   )

add_library(kcm_kshowmailconfigfilter MODULE ${CONFIGFILTER_SRCS} )
target_link_libraries( kcm_kshowmailconfigfilter ${KDE4_KDEUI_LIBS} KF5::KIOWidgets ${KDE4_KUTILS_LIBS} )

set( CONFIGLOG_SRCS
     configlog.cpp
   )
add_library(kcm_kshowmailconfiglog MODULE ${CONFIGLOG_SRCS} )
target_link_libraries( kcm_kshowmailconfiglog ${KDE4_KDEUI_LIBS} )

set( CONFIGGENERAL_SRCS
     configgeneral.cpp
   )
add_library(kcm_kshowmailconfiggeneral MODULE ${CONFIGGENERAL_SRCS} )
target_link_libraries( kcm_kshowmailconfiggeneral ${KDE4_KDEUI_LIBS} )

set( CONFIGSPAMCHECK_SRCS
     configspamcheck.cpp
     mailboxwizard.cpp
     mailboxwizardlistitem.cpp
   )
add_library(kcm_kshowmailconfigspamcheck MODULE ${CONFIGSPAMCHECK_SRCS} )
target_link_libraries( kcm_kshowmailconfigspamcheck ${KDE4_KDEUI_LIBS} KF5::KIOWidgets )


########### install files ###############

install( FILES kshowmail.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR} )
install( FILES kshowmailui.rc  DESTINATION  ${DATA_INSTALL_DIR}/kshowmail )
install(FILES
  kshowmailconfigaccounts.desktop
  kshowmailconfigactions.desktop
  kshowmailconfigdisplay.desktop
  kshowmailconfiggeneral.desktop
  kshowmailconfigfilter.desktop
  kshowmailconfiglog.desktop
  kshowmailconfigspamcheck.desktop
  DESTINATION ${SERVICES_INSTALL_DIR}
)

install(TARGETS
  kcm_kshowmailconfigaccounts
  kcm_kshowmailconfigactions
  kcm_kshowmailconfigdisplay
  kcm_kshowmailconfigfilter
  kcm_kshowmailconfiglog
  kcm_kshowmailconfiggeneral
  kcm_kshowmailconfigspamcheck
  DESTINATION ${PLUGIN_INSTALL_DIR}
)

