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
 )

kde4_add_executable(kshowmail ${kshowmail_SRCS})

target_link_libraries(kshowmail ${KDE4_KDEUI_LIBS} ${KDE4_KUTILS_LIBS} )

install(TARGETS kshowmail ${INSTALL_TARGETS_DEFAULT_ARGS} )

########## kcm libraries ################
set( CONFIGACCOUNTS_SRCS
     configaccounts.cpp
		  )
		 
kde4_add_plugin(kcm_kshowmailconfigaccounts ${CONFIGACCOUNTS_SRCS} )
target_link_libraries(kcm_kshowmailconfigaccounts  ${KDE4_KDEUI_LIBS})



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
  DESTINATION ${PLUGIN_INSTALL_DIR}
)

