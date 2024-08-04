/**
 * @file gmail.ino
 * @brief Gmail経由でメールを送信するスケッチ
 */

// Copyright (c) 2022 Hiroshi Tanigawa (Handle: Synapse)
// This sketch is distributed at http://synapse.kyoto
// This sketch is released under the MIT licence.
// https://opensource.org/licenses/mit-license.php

#define SECURE_MODE //!< SECURE_MODEを宣言すると、サーバー認証も行なう。コメントアウトすると、サーバー認証を省略する。プログラムは短くなるがセキュリティ的に甘くなる。
// We will declare later if we use secure mode or not. With secure mode turned off, the code skips server authentication. The program will be shorter, but less secure. 

#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif

#include <WiFiClientSecure.h>

const char SSID[] = "AAAAAAAAAAAAAAAA";                //!< Wi-FiのSSID。自分の環境に合わせて書き換える. Name of your WiFi network.
const char PASSWORD[] = "11111111111111111111114";      //!< Wi-Fiのパスワード。自分の環境に合わせて書き換える. Password of your WiFi network.
const char MAIL_ADDRESS[] = "a@gmail.com"; //!< 送信元のGmailのメールアドレス. E-mail address of the sender (=this ESP32)
const char MAIL_PASSWORD[] = "aaaa bbbb cccc dddd";         //!< 送信元のGmailのパスワード. The 'app password' of the sender. Detail is found in README.md
const char TO_ADDRESS[] = "b@example.com";           //!< 送信先のメールアドレス. E-mail address of the recipient.
const char MAIL_SUBJECT[] = "Hola! Yo soy ESP32."; //!< メールの件名. The title of the e-mail.

//! メールの本文。改行する時には\r\nを挿入する。行が半角ピリオド(.)で始まる時は、半角ピリオドを二重(..)にする
const char MAIL_BODY[] = "Esta un mensaje de ESP32"; // Message in the e-mail. In case the line starts with '.' please use '..' instead.

// GmailのSMTPサーバーのURLとポート番号
const char SERVER_NAME[] = "smtp.gmail.com"; //!< GmailのSMTPサーバーのURL // We will use smtp server of Gmail.
const int SERVER_PORT = 465;                 //!< GmailのSMTPサーバーへの接続に使うポート番号 // Server port.

#ifdef SECURE_MODE
/**
 * Google Trust Services LLCのルート証明書。有効期間が2036年6月22日で終わるので、その日より後では、
 * SECURE_MODEをdefineしていると、このスケッチは動作しない。
 */
/* The root certificate will expire on 22 June 2036. The sketch will not work after that day.
 */
const char ROOT_CERT[] =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIFWjCCA0KgAwIBAgIQbkepxUtHDA3sM9CJuRz04TANBgkqhkiG9w0BAQwFADBH\n"
    "MQswCQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZpY2VzIExM\n"
    "QzEUMBIGA1UEAxMLR1RTIFJvb3QgUjEwHhcNMTYwNjIyMDAwMDAwWhcNMzYwNjIy\n"
    "MDAwMDAwWjBHMQswCQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNl\n"
    "cnZpY2VzIExMQzEUMBIGA1UEAxMLR1RTIFJvb3QgUjEwggIiMA0GCSqGSIb3DQEB\n"
    "AQUAA4ICDwAwggIKAoICAQC2EQKLHuOhd5s73L+UPreVp0A8of2C+X0yBoJx9vaM\n"
    "f/vo27xqLpeXo4xL+Sv2sfnOhB2x+cWX3u+58qPpvBKJXqeqUqv4IyfLpLGcY9vX\n"
    "mX7wCl7raKb0xlpHDU0QM+NOsROjyBhsS+z8CZDfnWQpJSMHobTSPS5g4M/SCYe7\n"
    "zUjwTcLCeoiKu7rPWRnWr4+wB7CeMfGCwcDfLqZtbBkOtdh+JhpFAz2weaSUKK0P\n"
    "fyblqAj+lug8aJRT7oM6iCsVlgmy4HqMLnXWnOunVmSPlk9orj2XwoSPwLxAwAtc\n"
    "vfaHszVsrBhQf4TgTM2S0yDpM7xSma8ytSmzJSq0SPly4cpk9+aCEI3oncKKiPo4\n"
    "Zor8Y/kB+Xj9e1x3+naH+uzfsQ55lVe0vSbv1gHR6xYKu44LtcXFilWr06zqkUsp\n"
    "zBmkMiVOKvFlRNACzqrOSbTqn3yDsEB750Orp2yjj32JgfpMpf/VjsPOS+C12LOO\n"
    "Rc92wO1AK/1TD7Cn1TsNsYqiA94xrcx36m97PtbfkSIS5r762DL8EGMUUXLeXdYW\n"
    "k70paDPvOmbsB4om3xPXV2V4J95eSRQAogB/mqghtqmxlbCluQ0WEdrHbEg8QOB+\n"
    "DVrNVjzRlwW5y0vtOUucxD/SVRNuJLDWcfr0wbrM7Rv1/oFB2ACYPTrIrnqYNxgF\n"
    "lQIDAQABo0IwQDAOBgNVHQ8BAf8EBAMCAQYwDwYDVR0TAQH/BAUwAwEB/zAdBgNV\n"
    "HQ4EFgQU5K8rJnEaK0gnhS9SZizv8IkTcT4wDQYJKoZIhvcNAQEMBQADggIBADiW\n"
    "Cu49tJYeX++dnAsznyvgyv3SjgofQXSlfKqE1OXyHuY3UjKcC9FhHb8owbZEKTV1\n"
    "d5iyfNm9dKyKaOOpMQkpAWBz40d8U6iQSifvS9efk+eCNs6aaAyC58/UEBZvXw6Z\n"
    "XPYfcX3v73svfuo21pdwCxXu11xWajOl40k4DLh9+42FpLFZXvRq4d2h9mREruZR\n"
    "gyFmxhE+885H7pwoHyXa/6xmld01D1zvICxi/ZG6qcz8WpyTgYMpl0p8WnK0OdC3\n"
    "d8t5/Wk6kjftbjhlRn7pYL15iJdfOBL07q9bgsiG1eGZbYwE8na6SfZu6W0eX6Dv\n"
    "J4J2QPim01hcDyxC2kLGe4g0x8HYRZvBPsVhHdljUEn2NIVq4BjFbkerQUIpm/Zg\n"
    "DdIx02OYI5NaAIFItO/Nis3Jz5nu2Z6qNuFoS3FJFDYoOj0dzpqPJeaAcWErtXvM\n"
    "+SUWgeExX6GjfhaknBZqlxi9dnKlC54dNuYvoS++cJEPqOba+MSSQGwlfnuzCdyy\n"
    "F62ARPBopY+Udf90WuioAnwMCeKpSwughQtiue+hMZL77/ZRBIls6Kl0obsXs7X9\n"
    "SQ98POyDGCBDTtWTurQ0sR8WNh8M5mQ5Fkzc4P4dyKliPUDqysU0ArSuiYgzNdws\n"
    "E3PYJ/HQcu51OyLemGhmW/HGY0dVHLqlCFF1pkgl\n"
    "-----END CERTIFICATE-----\n";
#endif

WiFiClientSecure client; //!< SMTPサーバーとのTLS通信に使うクライアント // declare client.

/**
 * 文字列をBase64エンコードする。
 * @param str エンコード前の文字列
 * @returns エンコード後の文字列
 */
String base64(String str)
{
  byte buf = 0;
  byte bufUsed = 0;
  const char convTbl[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  String result = F("");
  for (int i = 0; i < str.length(); i++)
  {
    byte charCode = str[i];
    buf = (buf << (6 - bufUsed)) + (charCode >> (2 + bufUsed));
    result += convTbl[buf];
    charCode &= (1 << (2 + bufUsed)) - 1;
    if (bufUsed >= 4)
    {
      buf = charCode >> (bufUsed - 4);
      result += convTbl[buf];
      buf = charCode && ((1 << (bufUsed - 4)) - 1);
      bufUsed -= 4;
    }
    else
    {
      bufUsed += 2;
      buf = charCode;
    } // if
  } // for i
  if (bufUsed > 0)
  {
    buf <<= (6 - bufUsed);
    result += convTbl[buf];
  } // if
  int len = result.length();
  if (len % 4 != 0)
  {
    for (int i = 0; i < 4 - len % 4; i++)
    {
      result += '=';
    } // for i
  } // if
  return result;
} // base64

/**
 * 文字列をシリアルとSMTPサーバーに送信する。
 * @param str 送信したい文字列
 */
void sendString(String str)
{
  Serial.print("Sending: ");
  Serial.println(str);
  client.print(str + "\r\n");
} // SendString

/**
 * SMTPサーバーからの応答を待つ。10秒応答がなければタイムアウトする。
 * サーバーの応答をシリアルに出力する。
 * 応答コードが400番以上なら、エラーと判断して0を返す。
 * @returns 400番未満の応答コードがSMTPサーバーから返れば1。400番未満の応答コードが返るかタイムアウトすれば0。
 */
byte eRcv()
{
  byte respCode;
  byte thisByte;
  int loopCount = 0;

  while (!client.available())
  {
    delay(1);
    loopCount++;
    // if nothing received for 10 seconds, timeout
    if (loopCount > 10000)
    {
      client.stop();
      Serial.println("\r\nTimeout");
      return 0;
    } // if
  } // while

  respCode = client.peek();
  while (client.available())
  {
    thisByte = client.read();
    Serial.write(thisByte);
  } // while

  if (respCode >= '4')
  {
    return 0;
  } // if
  return 1;
} // eRcv

#ifdef SECURE_MODE
/**
 * NTPで時刻を取得してGMTで内蔵時計を合わせる。時刻の取得は、サーバー認証の際に、証明書の
 * 有効期限が終わっていないかを確認するために必要になる。
 */
void setClock()
{
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  Serial.print("Waiting for NTP time sync");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2)
  {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  } // while
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));
} // setClock
#endif

/**
 * SMTPサーバーに接続し、メールを送信する。
 */
byte sendEmail()
{
  byte thisByte = 0;
  byte respCode;

#ifdef SECURE_MODE
  // NTPで内蔵時計の時刻を合わせる(証明書の期限の確認に、正確な時刻が必要)
  setClock();

  // ルート証明書を使ってサーバー認証を行なうモードに設定する
#ifdef ESP8266
  X509List rootCert(ROOT_CERT);
  client.setTrustAnchors(&rootCert);
#else  // ifdef ESP8266
  client.setCACert(ROOT_CERT);
#endif // ifdef ESP8266

#else  // ifdef SECURE_MODE
  // サーバー認証を行なわないモードに設定する(セキュリティ的には問題あり)
  client.setInsecure();
#endif // ifdef SECURE_MODE

  if (client.connect(SERVER_NAME, SERVER_PORT) == 1)
  {
    Serial.println(F("Connected to server."));
  }
  else
  {
    Serial.println(F("Connection failed."));
    return 0;
  } // if
  if (!eRcv())
    return 0;

  sendString("EHLO www.example.com");
  if (!eRcv())
    return 0;
  sendString("auth plain");
  if (!eRcv())
    return 0;
  sendString(base64(String(MAIL_ADDRESS) + '\0' + MAIL_ADDRESS + '\0' + MAIL_PASSWORD));
  if (!eRcv())
    return 0;
  sendString(String("MAIL From:<") + MAIL_ADDRESS + ">");
  if (!eRcv())
    return 0;
  sendString(String("RCPT To:<") + TO_ADDRESS + ">");
  if (!eRcv())
    return 0;
  sendString("DATA");
  if (!eRcv())
    return 0;
  sendString(String("to:") + TO_ADDRESS);
  sendString(String("from:") + MAIL_ADDRESS);
  sendString(String("Subject:") + MAIL_SUBJECT);
  sendString("");
  sendString(MAIL_BODY);
  sendString(".");
  if (!eRcv())
    return 0;
  sendString("QUIT");
  if (!eRcv())
    return 0;
  client.stop();
  Serial.println("disconnected");
  return 1;
} // sendEmail
