cmake_minimum_required(VERSION 3.24)

project(KrakenSDKConsumer LANGUAGES CXX)

find_package(KrakenEngine CONFIG REQUIRED)

add_executable(kraken_sdk_consumer main.cpp)
target_link_libraries(kraken_sdk_consumer PRIVATE Kraken::Kraken)
