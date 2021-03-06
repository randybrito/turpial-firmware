/**
 * @file defaults.h
 * @author Locha Mesh project developers (locha.io)
 * @brief
 * @version 0.1
 * @date 2019-11-07
 *
 * @copyright Copyright (c) 2020 Locha Mesh project developers
 * @license Apache 2.0, see LICENSE file for details
 */

#ifndef DEFAULTS_H
#define DEFAULTS_H

#define NVS_APP_NAMESPACE "turpial"           /*!< NVS application namespace */
#define NVS_IS_CONFIGURED_KEY "is_configured" /*!< Is turpial configured? */

#define RAD_ENABLED true /*!< Radio subsystem enabled? */
#define RAD_TX_PIN 5      /*!< Radio UART TX pin */
#define RAD_RX_PIN 4      /*!< Radio UART RX pin */
#define RAD_PORT UART_NUM_2
#define BUF_SIZE (1024)
#define RAD_BUF_SIZE (BUF_SIZE)

#define WIFI_MODE WIFI_MODE_AP              /*!< Wi-Fi Operation mode */
#define WST_SSID ""                         /*!< STA SSID */
#define WST_PASS ""                         /*!< STA Password */
#define WAP_SSID "io.locha.ap"              /*!< Default AP SSID */
#define WAP_PASS "default_password"         /*!< Default AP Password */
#define WAP_CHANNEL 6                       /*!< AP Channel */
#define WAP_MAXCONN 4                       /*!< AP Maximum connections */
#define WAP_AUTHMODE WIFI_AUTH_WPA_WPA2_PSK /*!< AP Authentication mode */

#define DEFAULT_REST_PORT 2656
#define USER_NAME_KEY "username"      /*!< default user_name for app to do request to server */
#define USER_PASSWORD_KEY "password"  /*!< default user_password app to do request to server */
#define DEFAUL_USERNAME "admin"       /*!< default username */
#define DEFAULT_USER_PASSWORD "admin" /*!< default user password */
#define MAX_USER_NAME_LENGTH 20       /*!< length of thr username */
#define MAX_USER_PASSWORD_LENGTH 20   /*!< max length of password for user */

#define DEFAULT_USER_BUTTON GPIO_NUM_21 /*!< default user button to control device */

#define DEFAULT_RGB_LED_R 0 /*!< default led red */
#define DEFAULT_RGB_LED_G 2 /*!< default led green */
#define DEFAULT_RGB_LED_B 4 /*!< default led blue */

#endif // DEFAULTS_H
