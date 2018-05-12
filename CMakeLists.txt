cmake_minimum_required( VERSION 3.5 )

project( scanner )

#find_package( cpprest REQUIRED )

add_definitions( -std=c++11 )
add_definitions( -g )
add_definitions( -DDEBUG )


include_directories(
    ./common
    ./dsp/include
    ./scheduler/include
    ./core/include
    ./third
    )


file( GLOB_RECURSE CommonSources    common/*.cpp )
file( GLOB_RECURSE CoreSources      core/source/*.cpp )
file( GLOB_RECURSE TackleSources    tackle/source/*.cpp )
file( GLOB_RECURSE DspSources       dsp/source/*.cpp )
file( GLOB_RECURSE SchedulerSources scheduler/source/*.cpp )


add_executable(
    ${PROJECT_NAME}
    ${CommonSources}
    ${TackleSources}
    ${CoreSources}
    ${DspSources}
    )


list(
    APPEND GNURADIO_LIBRARIES
    gnuradio-runtime
    gnuradio-pmt
    gnuradio-blocks
    gnuradio-analog
    gnuradio-filter
    gnuradio-fft
    gnuradio-audio
    gnuradio-uhd
    )

list(
    APPEND BOOST_LIBRARIES
    boost_system
    boost_thread
    )

target_link_libraries(
    ${PROJECT_NAME}
    -luhd
    ${GNURADIO_LIBRARIES}
    -lpthread
    ${BOOST_LIBRARIES}
    -L./third/bb60_sdk/lib -lbb_api
    )
