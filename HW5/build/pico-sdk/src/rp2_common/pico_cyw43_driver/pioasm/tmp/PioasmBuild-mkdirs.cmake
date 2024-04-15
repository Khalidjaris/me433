# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/Omniajam/Desktop/me433/Pico/pico-sdk/tools/pioasm"
  "/Users/Omniajam/Documents/GitHub/me433/HW5/build/pioasm"
  "/Users/Omniajam/Documents/GitHub/me433/HW5/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm"
  "/Users/Omniajam/Documents/GitHub/me433/HW5/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/tmp"
  "/Users/Omniajam/Documents/GitHub/me433/HW5/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/PioasmBuild-stamp"
  "/Users/Omniajam/Documents/GitHub/me433/HW5/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src"
  "/Users/Omniajam/Documents/GitHub/me433/HW5/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/PioasmBuild-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/Omniajam/Documents/GitHub/me433/HW5/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/PioasmBuild-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/Omniajam/Documents/GitHub/me433/HW5/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/PioasmBuild-stamp${cfgdir}") # cfgdir has leading slash
endif()
