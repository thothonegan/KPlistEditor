set(kplisteditor_SRCS
   main.cpp
   kplisteditor.cpp
 )

kde4_add_executable(kplisteditor ${kplisteditor_SRCS})

target_link_libraries(kplisteditor ${KDE4_KDEUI_LIBS} ${KDE4_KPARTS_LIBS})

########### install files ###############
install(TARGETS kplisteditor ${INSTALL_TARGETS_DEFAULT_ARGS} )
install( PROGRAMS kplisteditor.desktop  DESTINATION  ${XDG_APPS_INSTALL_DIR} )
install( FILES kplisteditor_shell.rc  DESTINATION  ${DATA_INSTALL_DIR}/kplisteditor )


#########################################################################
# KPART SECTION
#########################################################################
#kde_module_LTLIBRARIES = libkplisteditorpart.la
set(kplisteditor_PART_SRCS
   kplisteditor_part.cpp
 )

kde4_add_plugin(kplisteditorpart WITH_PREFIX ${kplisteditor_PART_SRCS})

target_link_libraries(kplisteditorpart ${KDE4_KDEUI_LIBS} ${KDE4_KPARTS_LIBS} )

install( PROGRAMS kplisteditor_part.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR} )
install( FILES kplisteditor_part.rc  DESTINATION  ${DATA_INSTALL_DIR}/kplisteditorpart )

install(TARGETS kplisteditorpart  DESTINATION ${PLUGIN_INSTALL_DIR} )
