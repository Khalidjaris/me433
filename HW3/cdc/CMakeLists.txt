add_executable(cdc
        cdc.c
        )

# pull in common dependencies
target_link_libraries(cdc pico_stdlib)

# create map/bin/hex file etc.
pico_add_extra_outputs(cdc)
