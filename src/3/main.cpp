/**
 * @file gmail.ino
 * @brief Gmail経由でメールを送信するスケッチ
 */

// Copyright (c) 2022 Hiroshi Tanigawa (Handle: Synapse)
// This sketch is distributed at http://synapse.kyoto
// This sketch is released under the MIT licence.
// https://opensource.org/licenses/mit-license.php

#include <WiFi.h>
#include "send_mail_synapse.h"

#include <WiFiClientSecure.h>

/*setup関数。Wi - Fiに接続し、メールを送信する。 */

void setup()
{
  Serial.begin(115200);
  delay(100);
  Serial.println();
  Serial.println("Running in secure mode.");
  Serial.print("Connecting To ");
  Serial.println(SSID);
  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  } // while
  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  byte ret = sendEmail();
} // setup

/**
 * loop関数。何もしない。
 */
void loop()
{
} // loop
