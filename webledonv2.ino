
// Inclusão das bibliotecas
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <U8g2lib.h>

U8G2_SSD1306_128X64_NONAME_F_SW_I2C 
u8g2(U8G2_R0, /* clock=*/ 14, /* data=*/ 12, U8X8_PIN_NONE);
// Configração do WiFi
const char* ssid = "HUAWEI-B311-3CD2";  // SSID Wifi
const char* password = "E4MHNJ5BJGM";  // Senha Wifi

// Variáveis de Server e Status do LED
ESP8266WebServer server(80);
bool LEDstatus = LOW;

void setup() {

  u8g2.begin();

  // Inicia Serial e LED
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  // Inicia Conexão WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Aguarda Conexão e Informa IP
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Rede WiFi: ");
  Serial.println(ssid);
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
  delay(100);

  // Configura Handles do Server e Inicia Server
  server.on("/", handle_OnConnect);
  server.on("/ledon", handle_ledon);
  server.on("/ledoff", handle_ledoff);
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("Servidor HTTP iniciado!");

}

void loop() {

  u8g2.firstPage();
do {
  u8g2.setFont(u8g2_font_helvB14_tn);
  u8g2.setCursor(0,13);
  u8g2.print(WiFi.localIP().toString());
  u8g2.setFont(u8g2_font_inb46_mr);
  u8g2.setCursor(17, 62);
  u8g2.print(LEDstatus == HIGH ? "OFF" : "ON");
  u8g2.setFont(u8g2_font_streamline_all_t);
  u8g2.drawGlyph(0, 38, 0x018e);
} while (u8g2.nextPage());

  server.handleClient();    // Faz o Handle
  if (LEDstatus)            // Checa se LED deve acender
    digitalWrite(LED_BUILTIN, HIGH); 
  else
    digitalWrite(LED_BUILTIN, LOW);
}


// FUNÇÕES HANDLE PARA HTML SERVER

void handle_OnConnect() {
  LEDstatus = LOW;
  server.send(200, "text/html", SendHTML(false));
}

void handle_ledon() {
  LEDstatus = HIGH;
  server.send(200, "text/html", SendHTML(true));
}

void handle_ledoff() {
  LEDstatus = LOW;
  server.send(200, "text/html", SendHTML(false));
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}


String SendHTML(uint8_t led) {
  String ptr = "<!DOCTYPE html>\n";
  ptr += "<html>\n";
  ptr += "<head>\n";
  ptr += "<title>Controlar o LED</title>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1>LED</h1>\n";
  ptr += "<p>Clique para Ligar e Desligar o LED.</p>\n";
  ptr += "<form method=\"get\">\n";
  if (led)
    ptr += "<input type=\"button\" value=\"    LED DESLIGADO   \" onclick=\"window.location.href='/ledoff'\">\n";

  else
    ptr += "<input type=\"button\" value=\"              LED LIGADO             \" onclick=\"window.location.href='/ledon'\">\n";
  ptr += "</form>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}
