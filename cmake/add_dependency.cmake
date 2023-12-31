#######################################################################################################################
# add_dependency.cmake
#
# Define macro to search for a dependency and add it to our project. This works only with dependency using the
# project-template!
#
# The macro is called "add_dependency" and takes 3 arguments:
#   - name of the dependency project as passed to FIND_PACKAGE (e.g. "ChimeraTK-DeviceAccess")
#   - required version as passed to FIND_PACKAGE
#   - a list of components used by this project including the REQUIRED keyword etc.
#
# The macro will call FIND_PACKAGE, add the returned include directories to the system list, add the library directories
# to the list of link directories, and update the CMAKE_CXX_FLAGS variable.
#
#######################################################################################################################

#######################################################################################################################
#
# IMPORTANT NOTE:
#
# DO NOT MODIFY THIS FILE inside a project. Instead update the project-template repository and pull the change from
# there. Make sure to keep the file generic, since it will be used by other projects, too.
#
# If you have modified this file inside a project despite this warning, make sure to cherry-pick all your changes
# into the project-template repository immediately.
#
#######################################################################################################################

FUNCTION(add_dependency dependency_project_name required_version)
  # collect additional arguments and put into the list of components
  SET(components "")
  foreach(arg IN LISTS ARGN)
    SET(components ${components} ${arg})
  endforeach()
  if("${required_version}" MATCHES "REQUIRED")
    message(FATAL_ERROR "wrong usage: add_dependency(${dependency_project_name} ${required_version} ...)")
  endif()
  FIND_PACKAGE(${dependency_project_name} ${required_version} COMPONENTS ${components})
  include_directories(SYSTEM ${${dependency_project_name}_INCLUDE_DIRS} ${${dependency_project_name}_INCLUDE_DIR})
  link_directories(${${dependency_project_name}_LIBRARY_DIRS})
  link_directories(${${dependency_project_name}_LIBRARY_DIR})
  #set the cxx flags used in the child project
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${${dependency_project_name}_CXX_FLAGS}" PARENT_SCOPE)
  #set all the flags we found also in the parent scope, so the child can hand them over to the grand children
  SET(${dependency_project_name}_LIBRARIES ${${dependency_project_name}_LIBRARIES} PARENT_SCOPE)
  SET(${dependency_project_name}_LIBRARY_DIRS "${${dependency_project_name}_LIBRARY_DIRS} ${${dependency_project_name}_LIBRARY_DIR}" PARENT_SCOPE)
  SET(${dependency_project_name}_LINKER_FLAGS "${${dependency_project_name}_LINKER_FLAGS} ${${dependency_project_name}_LINK_FLAGS}" PARENT_SCOPE)
  SET(${dependency_project_name}_LINK_FLAGS "${${dependency_project_name}_LINKER_FLAGS} ${${dependency_project_name}_LINK_FLAGS}" PARENT_SCOPE)
  SET(${dependency_project_name}_CXX_FLAGS "${${dependency_project_name}_CXX_FLAGS}" PARENT_SCOPE)
  SET(${dependency_project_name}_FOUND ${${dependency_project_name}_FOUND} PARENT_SCOPE)
  SET(${dependency_project_name}_VERSION ${${dependency_project_name}_VERSION} PARENT_SCOPE)
  SET(${dependency_project_name}_INCLUDE_DIRS ${${dependency_project_name}_INCLUDE_DIRS} PARENT_SCOPE)
  SET(${dependency_project_name}_PREFIX ${${dependency_project_name}_PREFIX} PARENT_SCOPE)
ENDFUNCTION(add_dependency)

# make sure that cmake finds modules provided by project-template.
# since with new cmake concept for imported targets, dependencies also search for implicit dependencies,
# all projects using add_dependency also require this module path.
set(_projectTemplateModulePath ${CMAKE_SOURCE_DIR}/cmake/Modules)
# substr search is better than regex if paths have special characters
string(FIND ":${CMAKE_MODULE_PATH}:" ":${_projectTemplateModulePath}:" _projectTemplateModulePathPos)
if (${_projectTemplateModulePathPos} EQUAL -1)
    list(APPEND CMAKE_MODULE_PATH "${_projectTemplateModulePath}")
endif()

message(WARNING "
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
You are using add_dependency() in your CMakeLists.txt which is deprecated and may lead to subtle problems. Please follow the project-template migration guide to cmake imported targets.
Furhter processing is delayed by 5 seconds.
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
")
execute_process(COMMAND ${CMAKE_COMMAND} -E sleep 5.0)
