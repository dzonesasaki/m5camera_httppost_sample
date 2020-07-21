// target : ESP32 wrover , Huge APP
// M5Camera model B
#include <WiFi.h>
#include <HTTPClient.h>
#include <Preferences.h>
#include <esp_camera.h>

// ref to https://www.mgo-tec.com/blog-entry-esp32-ov2640-ssd1331-test1.html/3
// ref to https://github.com/espressif/esp32-camera

#define URL1 "http://"
#define URL2 ":1880/mypost"

char *ssidC; //="SSID";
char *passC; //="PASSPHRESE"; 
char *servC; //="192.168.1.2";//server address
Preferences objEprom;
HTTPClient myHttp;

#define STRING_BOUNDARY "123456789000000000000987654321"
#define STRING_MULTIHEAD02 "Content-Disposition: form-data; name=\"uploadFile\"; filename=\"./testfig2.jpg\""
#define STRING_MULTIHEAD03 "Content-Type: image/jpg"

// for m5camera model B
#define PWDN_GPIO_NUM     -1
#define RESET_GPIO_NUM    15
#define XCLK_GPIO_NUM     27
#define SIOD_GPIO_NUM     22 //25
#define SIOC_GPIO_NUM     23

#define Y9_GPIO_NUM       19
#define Y8_GPIO_NUM       36
#define Y7_GPIO_NUM       18
#define Y6_GPIO_NUM       39
#define Y5_GPIO_NUM        5
#define Y4_GPIO_NUM       34
#define Y3_GPIO_NUM       35
#define Y2_GPIO_NUM       32
#define VSYNC_GPIO_NUM    25 //22
#define HREF_GPIO_NUM     26
#define PCLK_GPIO_NUM     21

void getEprom(void){
  if(objEprom.begin("myParams", true)==true) // readonly mode
  {
    String ssidTa = objEprom.getString("ssid");
    String passTa = objEprom.getString("wifipass");
    String servTa = objEprom.getString("serv");
    objEprom.end();

    ssidC = (char *)malloc(sizeof(char)*(ssidTa.length()+1));
    passC = (char *)malloc(sizeof(char)*(passTa.length()+1));
    servC = (char *)malloc(sizeof(char)*(servTa.length()+1));

    if (servTa.length()<1)
    {
      Serial.println("failed reading EPROM ");
      return;
    }

    ssidTa.toCharArray(ssidC, ssidTa.length()+1 );
    passTa.toCharArray(passC, passTa.length()+1 );
    servTa.toCharArray(servC, servTa.length()+1 );

    Serial.println(String(ssidC));
    Serial.println(String(passC));
    Serial.println(String(servC));
    Serial.println("read EPROM ... done");
  }
  else
  {
    objEprom.end();  
    Serial.println("failed reading EPROM ");
  }
}

void initWifiClient(void){
	Serial.print("Connecting to ");
	Serial.println(ssidC);

	WiFi.begin( ssidC, passC);
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
	Serial.println("WiFi connected.");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());
}




void initCam(void){
  // ref to https://www.mgo-tec.com/blog-entry-esp32-ov2640-ssd1331-test1.html/3
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG; // PIXFORMAT_ + YUV422|GRAYSCALE|RGB565|JPEG
  config.jpeg_quality = 10;
  config.frame_size = FRAMESIZE_UXGA; //FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
  config.fb_count = 1;

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    while(1){}
  }
}



int32_t httppost( uint8_t * ui8BufJpg, uint32_t iNumDat ){

  String stMyURL="";
  stMyURL+=URL1;
  stMyURL+=String(servC);
  stMyURL+=URL2;
  myHttp.begin(stMyURL);
    
  String stConType ="";
  stConType +="multipart/form-data; boundary=";
  stConType +=STRING_BOUNDARY;
  myHttp.addHeader("Content-Type", stConType);

  String stMHead="";
  stMHead += "--";
  stMHead += STRING_BOUNDARY;
  stMHead += "\r\n";
  stMHead += STRING_MULTIHEAD02;
  stMHead += "\r\n";
  stMHead += STRING_MULTIHEAD03;
  stMHead += "\r\n";
  stMHead += "\r\n";
  uint32_t iNumMHead = stMHead.length();

  String stMTail="";
  stMTail += "\r\n";
  stMTail += "--";
  stMTail += STRING_BOUNDARY;
  stMTail += "--";
  stMTail += "\r\n";
  stMTail += "\r\n";  
  uint32_t iNumMTail = stMTail.length();

  uint32_t iNumTotalLen = iNumMHead + iNumMTail + iNumDat;

  uint8_t *uiB = (uint8_t *)ps_malloc(sizeof(uint8_t)*iNumTotalLen);

  for(int uilp=0;uilp<iNumMHead;uilp++){
    uiB[0+uilp]=stMHead[uilp];
  }
  for(int uilp=0;uilp<iNumDat;uilp++){
    uiB[iNumMHead+uilp]=ui8BufJpg[uilp];
  }
  for(int uilp=0;uilp<iNumMTail;uilp++){
    uiB[iNumMHead+iNumDat+uilp]=stMTail[uilp];
  }

  int32_t httpResponseCode = (int32_t)myHttp.POST(uiB,iNumTotalLen);
	myHttp.end();
  free(uiB);
	return (httpResponseCode);
}

void setup() {
  Serial.begin(115200);
  getEprom();
  initWifiClient();
  myHttp.setReuse(true);
  initCam();
}

void loop() {
  delay(2000);

  // ---- get camera capture
  camera_fb_t * fb = NULL;
  fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("failure: Camera capture");
    return;
  }

  // ---- http post
  int iRetHttp = httppost( fb->buf , fb->len);
  if (iRetHttp==200)
  {
    Serial.print("*");
  }
  else
  {
    Serial.println("");
    Serial.print("failure: http post. return code: ");
    Serial.println(iRetHttp);
  }

}
