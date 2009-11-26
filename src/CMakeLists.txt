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
 )

kde4_add_executable(kshowmail ${kshowmail_SRCS})

target_link_libraries(kshowmail ${KDE4_KDEUI_LIBS} ${KDE4_KUTILS_LIBS} ${KDEPIMLIBS_KMIME_LIBS} )

install(TARGETS kshowmail ${INSTALL_TARGETS_DEFAULT_ARGS} )

########## kcm libraries ################
set( CONFIGACCOUNTS_SRCS
     configaccounts.cpp
     accountsetupitem.cpp
     encryption.cpp
     kwalletaccess.cpp
     accountsetupdialog.cpp
		  )
		 
kde4_add_plugin(kcm_kshowmailconfigaccounts ${CONFIGACCOUNTS_SRCS} )
target_link_libraries(kcm_kshowmailconfigaccounts  ${KDE4_KDEUI_LIBS})

set( CONFIGACTIONS_SRCS
     configactions.cpp
   )

kde4_add_plugin( kcm_kshowmailconfigactions ${CONFIGACTIONS_SRCS} )
target_link_libraries( kcm_kshowmailconfigactions ${KDE4_KDEUI_LIBS} ${KDE4_PHONON_LIBS} ${KDE4_KIO_LIBS} )

set( CONFIGDISPLAY_SRCS
     configdisplay.cpp
   )

kde4_add_plugin( kcm_kshowmailconfigdisplay ${CONFIGDISPLAY_SRCS} )
target_link_libraries( kcm_kshowmailconfigdisplay ${KDE4_KDEUI_LIBS} )

set( CONFIGFILTER_SRCS
     configfilter.cpp
     senderlistdialog.cpp
     filtersetupitem.cpp
     filtersetupdialog.cpp
     mailboxwizard.cpp
     mailboxwizardlistitem.cpp
     filtercriteriawidget.cpp
   )

kde4_add_plugin( kcm_kshowmailconfigfilter ${CONFIGFILTER_SRCS} )
target_link_libraries( kcm_kshowmailconfigfilter ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} ${KDE4_KUTILS_LIBS} )

set( CONFIGLOG_SRCS
     configlog.cpp
   )
kde4_add_plugin( kcm_kshowmailconfiglog ${CONFIGLOG_SRCS} )
target_link_libraries( kcm_kshowmailconfiglog ${KDE4_KDEUI_LIBS} )


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
  DESTINATION ${PLUGIN_INSTALL_DIR}
)

