idf_component_register	(
SRCS 					"main.c"
						"../../esp32_common/tcp/ntp_sync.c"
						"../../esp32_common/utils/utils.c"
						"../../esp32_common/utils/ota.c"
						"../../esp32_common/cmds/cmd_wifi.c"
						"../../esp32_common/cmds/cmd_system.c"
						"../../esp32_common/tcp/mqtt_ctrl.c"
						"../../esp32_common/tcp/tcp_log.c"
						"../../esp32_common/comm/i2ccomm.c"
						"../../esp32_common/utils/crash_handler.c"
						"../westa_op/westaop.c"
						"../westa_op/dht22.c"
						"../westa_op/bosch_bmp280/bmp2.c"
						"../westa_op/aht20.c"
						
INCLUDE_DIRS 			"."
						"../certs"
						"../../esp32_common"
    					"../../esp32_common/cmds"
    					"../../esp32_common/tcp"
    					"../../esp32_common/utils"
    					"../../esp32_common/owb"
						"../westa_op"
						"../westa_op/bosch_bmp280"
						"../../esp32_common/comm"
						)
target_link_libraries(${COMPONENT_LIB} INTERFACE "-ucommon_app_desc")
target_compile_definitions(${COMPONENT_LIB} PRIVATE
    COMMON_REPO_VERSION="${COMMON_REPO_VERSION}")