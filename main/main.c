/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_system.h"
//#include "esp_spi_flash.h"
#include "spi_flash_mmap.h"
#include "esp_spiffs.h"
#include "esp_vfs_dev.h"
#include "esp_vfs_fat.h"
#include "esp_log.h"
#include "esp_console.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "wear_levelling.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "linenoise/linenoise.h"
#include "esp_netif.h"
#include "lwip/sockets.h"
#include "esp_pm.h"
#include "driver/i2c_master.h"
#include "project_specific.h"
#include "common_defines.h"
#include "project_specific.h"
#include "cmd_wifi.h"
#include "cmd_system.h"
#include "utils.h"
#include "tcp_log.h"
#include "mqtt_client.h"
#include "mqtt_ctrl.h"
#include "ntp_sync.h"
#include "esp_ota_ops.h"
#include "westaop.h"

#define TAG "ctrl_dev"

#define CONFIG_STORE_HISTORY 1
#define CONFIG_CONSOLE_MAX_COMMAND_LINE_LENGTH	1024

int restart_in_progress;
int controller_op_registered;
esp_app_desc_t bpart_desc;

void mqtt_exec_fun(int argc, char **argv)
	{
	if(argc)
		{
		ESP_LOGI(TAG, "MQTT command: |%s| - %d", argv[0], strlen(argv[0]));
		if(strcmp("westa", argv[0]) == 0)
			do_westaop(argc, argv);
		if(strcmp("wifi", argv[0]) == 0)
			do_wifi(argc, argv);
		else
			{
			do_system_cmd(argc, argv);
			}
		}
	else
		ESP_LOGI(TAG, "invalid command argc = 0");
	}

static void initialize_nvs(void)
	{
	esp_err_t err = nvs_flash_init();
	if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
		{
		ESP_ERROR_CHECK(nvs_flash_erase());
		err = nvs_flash_init();
		}
	ESP_ERROR_CHECK(err);
	}

void app_main(void)
	{
	gpio_install_isr_service(0);

	restart_in_progress = 0;
	setenv("TZ","EET-2EEST,M3.4.0/03,M10.4.0/04",1);
#if FILESYSTEM == LITTLEFS
	littlefs_storage_check();
#elif FILESYSTEM == SPIFFS
	spiffs_storage_check();
#endif
	initialize_nvs();
	int nv_cert = get_all_nvscerts();
	get_nvs_conf();
	controller_op_registered = 0;

	if(!wifi_join(NULL, NULL, JOIN_TIMEOUT_MS, true))
		ESP_LOGI(TAG, "Failed to connect to %s", dev_conf.sta_ssid);
	else
		ESP_LOGI(TAG, "Connected to %s", dev_conf.sta_ssid);
	esp_wifi_set_ps(WIFI_PS_MAX_MODEM);

	sync_NTP_time();
	tcp_log_init(dev_conf.cs);
	esp_log_set_vprintf(my_log_vprintf);
	if(nv_cert != ESP_OK)
		ESP_LOGE(TAG, "Error retrieving certificates from NVS, MQTT broker connection not possible");

	/* Register commands */
	esp_console_register_help_command();
	register_system();
	register_wifi();
	register_westaop();

	if(mqtt_start(mqtt_exec_fun) != ESP_OK)
		RESTART("Unable to start mqtt client");

	controller_op_registered = 1;

#ifdef WITH_CONSOLE
	esp_console_repl_t *repl = NULL;
    esp_console_repl_config_t repl_config = ESP_CONSOLE_REPL_CONFIG_DEFAULT();
    /* Prompt to be printed before each line.
     * This can be customized, made dynamic, etc.
     */
    repl_config.prompt = PROMPT_STR ">";
    repl_config.max_cmdline_length = CONFIG_CONSOLE_MAX_COMMAND_LINE_LENGTH;
	repl_config.task_stack_size = 8192;
#if CONFIG_STORE_HISTORY
	repl_config.history_save_path = BASE_PATH HISTORY_FILE;
	ESP_LOGI(TAG, "Command history enabled");
#else
	ESP_LOGI(TAG, "Command history disabled");
#endif	
#if defined(CONFIG_ESP_CONSOLE_UART_DEFAULT) || defined(CONFIG_ESP_CONSOLE_UART_CUSTOM)
    esp_console_dev_uart_config_t hw_config = ESP_CONSOLE_DEV_UART_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_console_new_repl_uart(&hw_config, &repl_config, &repl));

#elif defined(CONFIG_ESP_CONSOLE_USB_CDC)
    esp_console_dev_usb_cdc_config_t hw_config = ESP_CONSOLE_DEV_CDC_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_console_new_repl_usb_cdc(&hw_config, &repl_config, &repl));

#elif defined(CONFIG_ESP_CONSOLE_USB_SERIAL_JTAG)
    esp_console_dev_usb_serial_jtag_config_t hw_config = ESP_CONSOLE_DEV_USB_SERIAL_JTAG_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_console_new_repl_usb_serial_jtag(&hw_config, &repl_config, &repl));
    //ESP_LOGI(TAG, "console stack: %d", repl_config.task_stack_size);

#else
	#error Unsupported console type
#endif

	ESP_ERROR_CHECK(esp_console_start_repl(repl));
#endif
	}
