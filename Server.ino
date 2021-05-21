#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#define WIFI_SSID
#define WIFI_PASSWORD

#define AP_SSID "IOT_ap"
#define AP_PASSWORD "iot12345"

#define URL_BACKEND "http://192.168.0.107:5000"

ESP8266WebServer server(80);

char wifiPage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="pt-br">
    <head>
        <meta charset="utf-8">
        <title>Wifi Connect</title>
        <style>
            *{
                padding: 0;
                margin: 0;
            }
            .infos{
                display: flex;
                justify-content: center;
                align-items: center;
                flex-direction: row;
                width: 550px;
                margin-top: 10px;
            }
            .infos div{
                width: 50%;
                text-align: center;
                color: #000000;
            }
           ul{
               list-style: none;
               overflow: hidden;
           }
           li{
               display: flex;
               justify-content: center;
               align-items: center;
               background-color: #dadada;
               height: 50px;
               border-bottom: solid 3px #797979;
               cursor: pointer;
           }
           li:hover{
               background-color: #b1b1b1;
           }
           li:last-child{
            border-bottom: none;
           }
           .list-wifi{
               background-color: #dbdbdb;
               border-radius: 10px;
                width: 550px;
                height: 550px;
           }
           .list-wifi > div:nth-child(2){
                height: 85%;
                margin-bottom: 10px;
                overflow: hidden;
                overflow-y: auto;
           }
           .list-wifi > div:last-child{
               display: flex;
               justify-content: center;
               align-items: center;
           }

           .list-wifi > div:last-child button{
               width: 200px;
               height: 40px;
               outline: hidden;
               border: none;
               border: solid 2px #036d8d;
               color: #f9f9f9;
               cursor: pointer;
               background-color: #099bc7;
               transition: background-color 200ms;
               border-radius: 5px;
           }
           .list-wifi > div:last-child button:hover{
            background-color: #036d8d;
            transition: background-color 200ms;
           }
           .list-wifi > div:last-child button:focus{
               outline: 0;
           }
           ::-webkit-scrollbar {
                width: 5px;
            }

            
            ::-webkit-scrollbar-track {
            background: #e6e6e6f8; 
            border-radius: 10px; 
            
            }
 
          
            ::-webkit-scrollbar-thumb {
            background: rgba(136, 136, 136, 0.568);
            border-radius: 10px; 
            }

           
            ::-webkit-scrollbar-thumb:hover {
            background: #555; 
            }
           #list li > div:first-child{
               display: flex;
               justify-content: center;
               align-items: center;
               flex-direction: row;
               text-align: center;
               width: 100%;
               height: 100%;
           }
           #list li > div:first-child div{
               width: 50%;
           }
           .container{
               display:flex;
               justify-content: center;
               align-items: center;
               height: 100%;
           }
           body{
               background-color: #09a4fd;
           }

           html,body{
            height: 100%;
            margin: 0;
            padding: 0;
           }
           .popup{
               display: none;
           }
           .popup.active{
               display: block;
               position: fixed;
               top: 0;
               left: 0;
               width: 100% !important;
               height: 100%;
               background-color: #00000060;
               z-index: 1;
           }
           .popup .overlay{
               position: absolute;
              
               
           }
           .popup .content{
               position: absolute;
               top: 50%;
               left: 50%;
               transform: translate(-50%, -50%);
               z-index: 2;
                width: 350px;
                height: 200px;
                background-color: #dbdbdb;
                display: flex;
                align-items: center;
                flex-direction: column;
           }

           .popup .content div:first-child{
               margin-top: 10px;
               margin-bottom: 30px;
           }
           .popup .content div:last-child{
               display: flex;
               justify-content: space-around;
               align-items: center;
               width: 100%;
               flex-direction: row;
               margin-bottom: 10px;
               margin-top: 30px;
           }
           .popup .content input{
               width: 200px;
               height: 35px;
           }

           .popup .content button{
            width: 100px;
            height: 30px;
           }

           
        </style>
    </head>
    <body>
        <div class="container">
            <div class="list-wifi">
                <div class="infos">
                    <div><p>Nome da rede</p></div>
                    <div><p>Itensidade do sinal</p></div>
                </div>
                <div>
                    <ul id="list">
                       
                    </ul>
                    
                </div>
                <div>
                    <button onclick="scanWifi()">Procurar redes</button>
                </div>
            </div>
        </div>

        <script>
            
            function scanWifi(){
                document.getElementById("list").innerHTML = "";
                var xhr = new XMLHttpRequest();
                var url = "/wifi";
                xhr.onreadystatechange = function(){
                  if(this.readyState == 4 && this.status == 200){
                     JSON.parse(this.response).map(function(item, i){
                     var li = document.createElement("li");
                     var div1 = document.createElement("div");
                     var div2 = document.createElement("div");
                     var divInfo = document.createElement("div");
                     var h3 = document.createElement("h3");
                     var p = document.createElement("p");
  
                     var ssid = document.createTextNode(item.ssid);
                     var rssi = document.createTextNode(item.rssi);
                     
                     h3.appendChild(ssid);
                     div1.appendChild(h3);
                     p.appendChild(rssi);
                     div2.appendChild(p);
                     divInfo.appendChild(div1);
                     divInfo.appendChild(div2);
                     li.appendChild(divInfo);
                     var list = document.getElementById("list");
                     list.appendChild(li);
                     li.id = i;
                     createPopup(item.ssid, li, i);
                     divInfo.onclick = ()=>togglePopup(i);
                    });
                  
                      }
                   }
                    xhr.open("GET", url, true);
                    xhr.send(); 

            }

            function createPopup(title, list, n){
                var popup = document.createElement("div");
                var overlay = document.createElement("div");
                var content = document.createElement("div");
                
                var divTitle = document.createElement("div");
                var divInput = document.createElement("div");
                var divBtn = document.createElement("div");
                var h3 = document.createElement("h3");
                var input = document.createElement("input");
                var cancelBtn = document.createElement("button");
                var connectBtn = document.createElement("button");

                var name = document.createTextNode(title);

                popup.className = "popup";
                popup.id = "popup";
                overlay.className = "overlay"
                content.className = "content";
                input.placeholder = "Senha";
                input.type = "password";
                cancelBtn.onclick = ()=> togglePopup(n);
                connectBtn.onclick = ()=>handleConnect({ssid:title, password:input.value});
                content.appendChild(divTitle);
                content.appendChild(divInput);
                content.appendChild(divBtn);
                h3.appendChild(name);
                divTitle.appendChild(h3);
                divInput.appendChild(input);
                cancelBtn.appendChild(document.createTextNode("Cancelar"));
                connectBtn.appendChild(document.createTextNode("Conectar"));
                divBtn.appendChild(cancelBtn);
                divBtn.appendChild(connectBtn);

                popup.appendChild(overlay);
                popup.appendChild(content);
                list.appendChild(popup);
            }

            function togglePopup(id){
                var li = document.getElementById(id);
                var popup = li.lastChild;
                
                popup.classList.toggle("active"); 
            }
         
            function handleConnect(value){
              var xhr = new XMLHttpRequest();
                var url = "/connect";
                xhr.open("POST", url);
                xhr.send(JSON.stringify(value));
            }
  </script>
       
    </body>
</html>
)=====";

void connectionPage(){
  server.send(200, "text/html", wifiPage);
}

void setup() {
  Serial.begin(115200);
  delay(1000);
   pinMode(BUILTIN_LED,OUTPUT);
  delay(1000);
  
  Serial.println("Server Started.");
  WiFi.softAP(AP_SSID, AP_PASSWORD);
  Serial.println("Access Point");
  Serial.print("SSID: ");
  Serial.println(AP_SSID);
  Serial.print("PASSWORD: ");
  Serial.println(AP_PASSWORD);
  Serial.print("ACCESS THE IP: ");
  Serial.println(WiFi.softAPIP());
  server.begin();
  delay(100);
  server.on("/", connectionPage);
  server.on("/wifi", scanWifi);
  server.on("/connect", connectWifi);
  
}

void loop() {
  server.handleClient();
  Serial.println(WiFi.status());
  /*
    WL_IDLE_STATUS      = 0,
    WL_NO_SSID_AVAIL    = 1,
    WL_SCAN_COMPLETED   = 2,
    WL_CONNECTED        = 3,
    WL_CONNECT_FAILED   = 4,
    WL_CONNECTION_LOST  = 5,
    WL_DISCONNECTED     = 6
    */

    if(WiFi.status() == 3){
      digitalWrite(BUILTIN_LED, LOW);
    }else{
      digitalWrite(BUILTIN_LED, HIGH);
    }
   
}

void scanWifi(){
  Serial.begin(115200);
  Serial.println("Scanning Networks");
  int n = WiFi.scanNetworks();
  if(n < 1 ){
    Serial.println("No networks found");
    server.send(200, "text/plain", "Nenhum wifi encontrado");
  }else{
    Serial.println("Networks found");
    Serial.println(n);
  Serial.println(WiFi.status());
    
    DynamicJsonDocument json(2048);
    JsonArray wifi = json.to<JsonArray>();
    
    for(int i = 0; i < n; i++){
      JsonObject oWifi = wifi.createNestedObject(); 
      oWifi["ssid"] =  WiFi.SSID(i);
      oWifi["rssi"] = WiFi.RSSI(i);
      oWifi["type"] = (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*";
    }
    String fJson;
    serializeJson(json, fJson);
    server.send(200,"text/plain", fJson);
  }
}

void connectWifi(){
  WiFi.disconnect();
  Serial.print("Conecting in Wifi: ");
  DynamicJsonDocument json(1024);
  deserializeJson(json, server.arg("plain"));
  String ssid = json["ssid"];
  Serial.println(ssid);
  String password = json["password"];
   WiFi.begin(ssid, password);
   delay(5000);
   Serial.print("IP: ");
   Serial.println(WiFi.localIP());
    server.send(200, "text/plain", "Connected.");
}

/*String request(String path){
  HTTPClient http;
  http.begin(URL_BACKEND + path);
  int httpCode = http.GET();
  if(httpCode < 0){
    Serial.println("request error: " + httpCode);
    return "";
  }

  if(httpCode != HTTP_CODE_OK){
    return "";
  }
 String response = http.getString();
  
  http.end();

  return response;
}*/

/*String requestJSON(String path){
  HTTPClient http;
  http.begin(URL_BACKEND + path);
  int httpCode = http.GET();
  if(httpCode < 0){
    Serial.println("request error: " + httpCode);
    return "";
  }

  if(httpCode != HTTP_CODE_OK){
    return "";
  }
 String response = http.getString();
  
  http.end();

  return response;
}*/
