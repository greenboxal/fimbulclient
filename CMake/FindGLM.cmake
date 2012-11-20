include (FindPackageHandleStandardArgs)

find_path(GLM_INCLUDE_DIR glm/glm.hpp
	PATH_SUFFIXES include
	PATHS
	${GLM_ROOT}
	$ENV{GLM_ROOT}
	~/Library/Frameworks
	/Library/Frameworks
	/usr/local/
	/usr/
	/sw          # Fink
	/opt/local/  # DarwinPorts
	/opt/csw/    # Blastwave
	/opt/)
	
find_package_handle_standard_args (GLM DEFAULT_MSG GLM_INCLUDE_DIR)
mark_as_advanced (GLM_INCLUDE_DIR)

if (GLM_FOUND)
	message (STATUS "Found GLM: ${GLM_INCLUDE_DIR}")
endif ()
