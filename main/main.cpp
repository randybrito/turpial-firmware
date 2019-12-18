/**
 * @file main.cpp
 * @author Locha Mesh Developers (contact@locha.io)
 * @brief
 * @version 0.1
 * @date 2019-09-11
 *
 * @copyright Copyright (c) 2019
 *
 */

#include <cstdio>
#include <memory>
#include <sstream>

#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <BLEPreferences.h>
#include <Storage.h>
#include <Radio.h>
#include <WiFi.h>
#include <FuelGauge.h>

#include <HttpServer.h>
#include <WebSocket.h>
#include <WsHandlerEvents.h>

#include "defaults.h"

HttpServer httpServer;

static const char* TAG = "app_main";

esp_err_t getIsConfigured(bool& is_configured)
{
    esp_err_t err;

    storage::NVS app_nvs;
    err = app_nvs.open(NVS_APP_NAMESPACE, NVS_READWRITE);
    if (err != ESP_OK) {
        const char* err_str = esp_err_to_name(err);
        ESP_LOGE(TAG,
            "Couldn't open namespace \"%s\" (%s)",
            NVS_APP_NAMESPACE,
            err_str);
        return err;
    }

    err = app_nvs.get_bool(NVS_IS_CONFIGURED_KEY, is_configured);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        // Set is_configured to true on flash so on next init the config is
        // readed directly by the ESP-IDF Wi-Fi library component.
        err = app_nvs.set_bool(NVS_IS_CONFIGURED_KEY, true);
        if (err != ESP_OK) return err;
        err = app_nvs.commit();
        if (err != ESP_OK) return err;
        // Set the return variable to "false" to forcibly set the default
        // configuration
        is_configured = false;
    } else {
        return err;
    }

    return ESP_OK;
}


void webSocketHandler(HttpRequest* pHttpRequest, HttpResponse* pHttpResponse)
{
    WsHandlerEvents* myHandler = new WsHandlerEvents();
    if (pHttpRequest->isWebsocket()) {
        pHttpRequest->getWebSocket()->setHandler(myHandler);
        ESP_LOGI("available clients---->>", "%d", pHttpRequest->getWebSocket()->availableClients());
    }
}

extern "C" void app_main()
{
    esp_err_t err;

    err = storage::init();

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Couldn't initialize NVS, error (%s)", esp_err_to_name(err));
        return;
    }

    bool is_configured = false;
    err = getIsConfigured(is_configured);
    if (err != ESP_OK) {
        ESP_LOGE(TAG,
            "Couldn't get \"is_configured\" value (%s)",
            esp_err_to_name(err));
    }

    network::WiFi& wifi = network::WiFi::getInstance();
    err = wifi.init();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Couldn't initalize Wi-Fi interface (%s)", esp_err_to_name(err));
        return;
    }

    if (!is_configured) {
        wifi.setMode(WIFI_MODE);

        network::APConfig ap_config = {
            .ssid = WAP_SSID,
            .password = WAP_PASS,
            .authmode = WAP_AUTHMODE,
            .max_conn = WAP_MAXCONN,
            .channel = WAP_CHANNEL,
        };
        wifi.setApConfig(ap_config);

        network::STAConfig sta_config = {
            .ssid = WST_SSID,
            .password = WST_PASS,
        };
        wifi.setStaConfig(sta_config);
    }

    err = wifi.start();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Couldn't start Wi-Fi, err = %s", esp_err_to_name(err));
        return;
    }


    ble::ServerParams server_params;
    server_params.device_name = "Turpial-1234";
    server_params.static_passkey = 123456;
    server_params.app_id = 0;
    ble_preferences::start(server_params);

#if RAD_ENABLED == true
    radio::Radio* radio_task = new radio::Radio();
    radio_task->start();
#endif

    httpServer.addPathHandler(HttpRequest::HTTP_METHOD_GET, "/", webSocketHandler);
    httpServer.start(80);


#if ESC_ENABLED == true
    esc::FuelGauge fuel_gauge;

    err = fuel_gauge.enterConfig(true);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Can't enter configuration mode, err = %s",
                 esp_err_to_name(err));
        return;
    }

    err = fuel_gauge.setCapacity(ESC_MAX_BATTERY_CAPACITY);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Can't set capacity, err = %s",
                 esp_err_to_name(err));
        return;
    }

    err = fuel_gauge.setGPOUTPolarity(true);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Can't set polarity, err = %s",
                 esp_err_to_name(err));
        return;
    }

    err = fuel_gauge.setGPOUTFunction(esc::SOC_INT);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Can't set function, err = %s",
                 esp_err_to_name(err));
        return;
    }

    err = fuel_gauge.setSOCIDelta(1);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Can't set function, err = %s",
                 esp_err_to_name(err));
        return;
    }

    err = fuel_gauge.exitConfig(true);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Can't exit configuration mode, err = %s",
                 esp_err_to_name(err));
        return;
    }

    // Test interrupt
    fuel_gauge.pulseGPOUT();

    std::uint16_t voltage = 0;
    std::int16_t avg_current = 0;
    std::int16_t avg_power = 0;

    err = fuel_gauge.voltage(&voltage);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Can't get voltage, err = %s", esp_err_to_name(err));
        return;
    }

    err = fuel_gauge.avgCurrent(&avg_current);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Can't get avg. current, err = %s",
                 esp_err_to_name(err));
        return;
    }

    err = fuel_gauge.avgPower(&avg_power);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Can't get avg. power, err = %s", esp_err_to_name(err));
        return;
    }

    ESP_LOGI(TAG, "%d mV | %d mA | %d mA", voltage, avg_current, avg_power);
#endif
}
