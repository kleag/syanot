include_directories( ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/src/part ${CMAKE_SOURCE_DIR}/src/printing ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### syanot target ###############

set(syanot_SRCS syanotmain.cpp syanot.cpp syanotConfigDialog.cpp
    EasyConstituent.cpp EasyForm.cpp EasyGroup.cpp EasyUtterance.cpp
    EasyDocument.cpp EasyXmlReader.cpp EasyRelation.cpp PartMatch.cpp)

qt4_add_dbus_adaptor(syanot_SRCS org.kde.syanot.xml syanot.h Syanot)

kde4_add_kcfg_files( syanot_SRCS syanotsettings.kcfgc )

kde4_add_ui_files( syanot_SRCS syanotmainwindow.ui )

kde4_add_executable(syanot ${syanot_SRCS})

target_link_libraries(syanot  ${QT_AND_KDECORE_LIBS} ${KDE4_KHTML_LIBS})

install( TARGETS syanot DESTINATION ${BIN_INSTALL_DIR})

########### install syanot files ###############

install( FILES x-syanot.desktop DESTINATION ${MIME_INSTALL_DIR}/application )
install( FILES syanotsettings.kcfg DESTINATION ${KCFG_INSTALL_DIR} )
install( FILES syanotui.rc DESTINATION ${DATA_INSTALL_DIR}/syanot )
install( FILES syanot.desktop DESTINATION ${XDG_APPS_INSTALL_DIR} )
install( FILES syanot.desktop DESTINATION ${SERVICES_INSTALL_DIR} )
install( FILES syanot-coord-coordonant.png DESTINATION ${DATA_INSTALL_DIR}/syanot/pics )
install( FILES syanot-coord-coordg.png DESTINATION ${DATA_INSTALL_DIR}/syanot/pics )
install( FILES syanot-coord-coordd.png DESTINATION ${DATA_INSTALL_DIR}/syanot/pics )

