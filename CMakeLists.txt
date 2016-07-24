cmake_minimum_required(VERSION 2.8)
project( TG )
set(BUILD_SHARED_LIBS OFF)


find_package( OpenCV REQUIRED )

if (OpenCV_FOUND)
   include_directories(${OpenCV_INCLUDE_DIRS})
endif()

find_library(XercesLibrary NAMES xerces-c PATHS /usr/local/lib)
if(NOT XercesLibrary)
  message(FATAL_ERROR "Failed to find the Xerces library.")
endif()

SET(ALETHIA_PARSER_HEADERS
				AlethiaParser/AlethiaParser.h
								)
SET(ALETHIA_PARSER_SOURCE
				AlethiaParser/AlethiaParser.cpp
								)

SET(TG_HEADERS 	  
				StandardHeaders.h
				StatisticalFunctions.h
				sorting.h
				folder.h
				GaborAngleFeature.h
				Morphology.h
				ColorQuantization.h
				SmoothingGapfilling.h
				Image_proc_functions.h
				binarization.h
				RectangleTest.h
				connectedcomponent.h
				PixelValidation.h
				PrepareAlethiaGT.h
					)  


SET(TG_SOURCES     
				main.cpp  
				StatisticalFunctions.cpp
				sorting.cpp
				folder.cpp
				GaborAngleFeature.cpp
				Morphology.cpp
				ColorQuantization.cpp
				SmoothingGapfilling.cpp
				Image_proc_functions.cpp
				binarization.cpp
				RectangleTest.cpp
				connectedcomponent.cpp
				PixelValidation.cpp
				PrepareAlethiaGT.cpp
					)
				


# Or set it globally for all targets:
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

ADD_EXECUTABLE(gabor ${TG_HEADERS} ${TG_SOURCES} ${ALETHIA_PARSER_HEADERS} ${ALETHIA_PARSER_SOURCE})
TARGET_LINK_LIBRARIES(gabor ${OpenCV_LIBS} ${XercesLibrary})
set(CMAKE_BUILD_TYPE Debug)

