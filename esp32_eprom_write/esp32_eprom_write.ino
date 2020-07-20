#include <Preferences.h>

Preferences objEprom;

void setup() {
  Serial.begin(115200);
  String myKey = "myParams";
  String ssidT = "SSID";
  String passT = "PASSWORD";
  String servT = "169.1.1.1";

  objEprom.begin("myParams", false);
  objEprom.putString("ssid", ssidT);
  objEprom.putString("wifipass", passT);
  objEprom.putString("serv", servT);
  objEprom.end();
  Serial.println("wrote params done");
  Serial.println(myKey);
  Serial.println(ssidT);
  Serial.println(passT);
  Serial.println(servT);
}

void loop() {

}
