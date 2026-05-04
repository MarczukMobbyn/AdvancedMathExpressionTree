cmake_minimum_required(VERSION 4.0)
project(TEP_Lista_3)

set(CMAKE_CXX_STANDARD 11)

add_compile_options(-fno-elide-constructors)

add_executable(TEP_Lista_3 main.cpp
        CNode.h
        CNumber.h
        CVar.h
        COperator.h
        CNumber.cpp
        CVar.cpp
        COperator.cpp
        CTree.h
        CTree.cpp
        CInterface.h
        CInterface.cpp
        CTreeException.h
        CError.h
        CError.cpp
        CResult.h
        CResultWriter.h
        CRefCounter.h
        CMySmartPointer.h
)
