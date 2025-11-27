cmake_minimum_required(VERSION 3.5)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

project(LORARX_BOXES)
set(PARTITION_TABLE_CSV "partitions.csv")
littlefs_create_partition_image(littlefs littlefs FLASH_IN_PROJECT)