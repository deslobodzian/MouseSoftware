# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(baldr_receiver)
FILE(GLOB APP_SOURCE src/*.c)
target_include_directories(app PRIVATE inc/)
target_sources(app PRIVATE
    ${APP_SOURCE}
)
