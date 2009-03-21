set(kshowmail_SRCS
   kshowmail.cpp
	 kshowmailview.cpp
   main.cpp
   uniqueapp.cpp
 )

kde4_add_executable(kshowmail ${kshowmail_SRCS})

target_link_libraries(kshowmail ${KDE4_KDEUI_LIBS} )

install(TARGETS kshowmail ${INSTALL_TARGETS_DEFAULT_ARGS} )


########### install files ###############

install( FILES kshowmail.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR} )
install( FILES kshowmail.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
install( FILES kshowmailui.rc  DESTINATION  ${DATA_INSTALL_DIR}/kshowmail )
