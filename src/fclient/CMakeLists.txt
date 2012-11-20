include_directories (.)

file (GLOB BASE_FILES fclient/*.c fclient/*.cpp fclient/*.h fclient/*.hpp)

add_executable (fclient
    ${BASE_FILES}
)
target_link_libraries (fclient ya3de ${AWESOMIUM_LIBRARY})
set_target_properties (fclient PROPERTIES COTIRE_ADD_UNITY_BUILD FALSE)
cotire (fclient)

source_group ("fclient" FILES ${BASE_FILES})
