cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

# Project name

project(OutputProcessing LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(ROOT REQUIRED RIO Net Hist Tree MathCore ROOTDataFrame)
#find_package(ROOT REQUIRED RDataFrame)

# Finding ROOT
 #find_package(ROOT 6.20 CONFIG REQUIRED)


# Adding an executable program
add_executable(
    getHistogram
    GetHistogram.cpp
)

#add_executable(
 # getHistogram-Eventwise
 # GetHistogram-Eventwise.cpp
#)

add_executable(
    getSolidAngleCoverage
    GetSolidAngleCoverage.cpp
)

add_executable(
    histogramToTxt
    HistogramToTxt.cpp
)

add_executable(
    mergeFiles
    MergeFiles.cpp
)

add_executable(
    rootToTxt
    RootToTxt.cpp
)

 #add_executable(
  #   getHistogramRDF
  #   getHistogramRDF.cpp
 #)

# Linking to libraries
target_link_libraries(
    getHistogram
    PUBLIC
    Threads::Threads
    ROOT::Core
    ROOT::Tree
    ROOT::TreePlayer
    ROOT::Hist)

#target_link_libraries(
  # getHistogram-Eventwise
  # PUBLIC
  # Threads::Threads
  # ROOT::Core
  # ROOT::Tree
  # ROOT::TreePlayer #Added by Refilwe
  # ROOT::Hist
  # ROOT::ROOTDataFrame #uncommenetd by Refilwe 
  # ROOT::RIO
  #/home/refilwemolaeng/Software/root/v6-20-00_Binaries/lib/libROOTDataFrame.so
  # /home/refilwemolaeng/Software/root/v6-20-00_Binaries/lib/libTreePlayer.so
  #)


target_link_libraries(
    getSolidAngleCoverage
    PUBLIC
    Threads::Threads
    ROOT::Core
    ROOT::Tree)

target_link_libraries(
    histogramToTxt
    PUBLIC
    Threads::Threads
    ROOT::Core
    ROOT::Tree
    ROOT::Hist)

target_link_libraries(
    mergeFiles
    PUBLIC
    Threads::Threads
    ROOT::Core
    ROOT::Tree
    ROOT::Hist)

target_link_libraries(
    rootToTxt
    PUBLIC
    Threads::Threads
    ROOT::Core
    ROOT::Tree
    ROOT::Hist)

 #target_link_libraries(
 #    getHistogramRDF
 #    PUBLIC
 #   Threads::Threads
 #    ROOT::Core
 #    ROOT::Tree
 #    ROOT::Hist
 #    ROOT::ROOTDataFrame
 #    ROOT::RIO)

set(common_compile_options
    -Wall -Wshadow -Wnon-virtual-dtor
    -Werror -Wconversion -Wsign-conversion
    -Woverloaded-virtual
    -pedantic -fPIE -fstack-protector-all
)

target_compile_options(getHistogram PRIVATE ${common_compile_options})
#target_compile_options(getHistogram-Eventwise PRIVATE ${common_compile_options})
target_compile_options(getSolidAngleCoverage PRIVATE ${common_compile_options})
target_compile_options(histogramToTxt PRIVATE ${common_compile_options})
target_compile_options(mergeFiles PRIVATE ${common_compile_options})
target_compile_options(rootToTxt PRIVATE ${common_compile_options})

# Copy the scripts which don't need to be compiled
configure_file(fep_efficiency.sh fep_efficiency.sh COPYONLY)
configure_file(loopGetHistogram.sh loopGetHistogram.sh COPYONLY)
configure_file(loopHistogramToTxt.sh loopHistogramToTxt.sh COPYONLY)
configure_file(utrwrapper.py utrwrapper.py COPYONLY)
