cmake_minimum_required(VERSION 3.27)
project(D2RBO LANGUAGES CXX VERSION 1.0.0)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt6_standard_project_setup()

if(WIN32)
  set(APP_RC "resources/d2rbo.rc")
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/version.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/version.rc
    @ONLY
  )

  set(VERSION_RC ${CMAKE_CURRENT_BINARY_DIR}/version.rc)

endif()

qt6_add_executable(
  D2RBO
  WIN32
  MACOSX_BUNDLE
  ${APP_RC}
  ${VERSION_RC}
  src/main.cpp
  src/mainwindow.cpp src/mainwindow.h src/mainwindow.ui
  src/overlay.cpp src/overlay.h
  src/keyboardhook.cpp src/keyboardhook.h
  src/profile.h src/profileitem.h
  src/profiles.cpp src/profiles.h src/profiles.ui
  src/skillwidget.cpp src/skillwidget.h src/skillwidget.ui
  src/assets.cpp src/assets.h
  src/theme.cpp src/theme.h
)

target_compile_definitions(
  D2RBO
  PRIVATE
    "$<$<CONFIG:Debug>:ASSETS_PATH=\"${CMAKE_CURRENT_LIST_DIR}/resources\">")

qt6_add_resources(
  D2RBO "resources" PREFIX "/"
  FILES
  "resources/images/Battle Orders.png"
  "resources/default_profile.json"
)

include_directories(SYSTEM "nlohmann_json/include")
target_link_libraries(D2RBO PRIVATE Qt::Core Qt::Widgets)

install(
  TARGETS D2RBO
  RUNTIME DESTINATION .
  LIBRARY DESTINATION .
)
install(
  DIRECTORY "resources/images" "resources/all_skills"
  DESTINATION .
)

qt6_generate_deploy_script(
    TARGET D2RBO
    OUTPUT_SCRIPT deploy_script
    CONTENT "
        qt6_deploy_runtime_dependencies(
            EXECUTABLE $<TARGET_FILE:D2RBO>
            BIN_DIR .
            LIB_DIR .
            NO_COMPILER_RUNTIME
            NO_TRANSLATIONS
            EXCLUDE_PLUGINS qtuiotouchplugin
            DEPLOY_TOOL_OPTIONS
                --no-system-d3d-compiler
                --no-system-dxc-compiler
                --no-opengl-sw
        )
    "
)

install(SCRIPT ${deploy_script})
set(CPACK_GENERATOR "ZIP")
include(CPack)
