# -*- explicit-buffer-name: "CMakeLists.txt<M1-MOBJ/8-10>" -*-
#
# To display executed commands, use:
#     > cmake -DCMAKE_VERBOSE_MAKEFILE:STRING=YES ../src

cmake_minimum_required(VERSION 2.8.0)
project(tme88)

set (CMAKE_CXX_FLAGS            "-Wall -g" CACHE STRING "C++ Compiler Release options." FORCE)
set (CMAKE_INSTALL_PREFIX       "../install" )

find_package(LibXml2 REQUIRED)
find_package(Qt4     REQUIRED)
include(${QT_USE_FILE})

include_directories( ${SCHEMATIC_SOURCE_DIR}
        ${LIBXML2_INCLUDE_DIR}
        )

set( includes     Indentation.h
        XmlUtil.h
        Point.h
        Box.h
        Term.h
        Net.h
        Instance.h
        Node.h
        Line.h
        Shape.h
        Symbol.h
        Cell.h
        CellsLib.h
        InstanceModel.h
        InstanceWidget.h
        OpenCellDialog.h
        CellModel.h
        )

set( mocIncludes  SaveCellDialog.h
        CellWidget.h
        CellViewer.h
        InstanceModel.h
        InstanceWidget.h
        OpenCellDialog.h
        CellsModel.h
        CellsLib.h
        )

set( cpps         Indentation.cpp
        XmlUtil.cpp
        Point.cpp
        Box.cpp
        Term.cpp
        Net.cpp
        Instance.cpp
        Node.cpp
        Line.cpp
        Shape.cpp
        Symbol.cpp
        Cell.cpp
        SaveCellDialog.cpp
        CellWidget.cpp
        CellViewer.cpp
        InstanceModel.cpp
        InstanceWidget.cpp
        OpenCellDialog.cpp
        CellsModel.cpp
        CellsLib.cpp
        # NetsWidget.cpp
        # NetsModel.cpp
        Main.cpp)

qt4_wrap_cpp( mocCpps      ${mocIncludes} )

add_executable( tme88       ${cpps} ${mocCpps} )
target_link_libraries( tme88       ${QT_LIBRARIES} ${LIBXML2_LIBRARIES} )
install( TARGETS      tme88                      DESTINATION bin )
install( FILES        ${includes} ${mocIncludes}  DESTINATION include )
