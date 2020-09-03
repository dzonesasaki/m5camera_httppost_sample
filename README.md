# m5camera_httppost_sample
a sample of sending image via http post by m5camera using Arduino-IDE

# m5camera prep

1. download the code from the following site: https://github.com/dzonesasaki/m5camera_httppost_sample 
2. Extract the zip file and open the folder.
3. open ```esp32_eprom_write.ino```
4. rewrite the SSID, passphrase, and server IP address to suit your environment.
5. connect m5camera to your PC using USB
6. select  "Tools" -> "Board:..." -> "ESP32 Arduino" -> "ESP32 wrover Module".
7. select "Tools" -> "Partition Scheme:..." -> "Huge APP (3MB No OTA/1MB SPIFFS)". 
8. press upload button to write the sketch
9.  open ```m5cam_post.ino```
10. press upload button to write the sketch
11. you can see a picture on (Server IP address):1880/get

# for the server

1. start node-red on the server
2. Copy the json code from the following page in your browser: https://github.com/dzonesasaki/m5camera_httppost_sample/blob/master/server_node-red/httppost_server.json
3. paste the code and deploy .
4. you can see a picture on (Server IP address):1880/get

# m5cameraの準備

1. ここからコードをダウンロード https://github.com/dzonesasaki/m5camera_httppost_sample 
2. zipファイルを展開し，フォルダを開く
3. esp32_eprom_write.inoを開く
4. 自分の環境に合わせて，SSID，パスフレーズ，サーバIPアドレスを書き換える
5. PCにm5cameraをUSB接続する
6. arduino-IDEで「ツール」→「ボード:...」→「ESP32 Arduino」→「ESP32 wrover Module」を選択
7. arduino-IDEで「ツール」→「Partition Scheme:...」→「Huge APP(3MB No OTA/1MB SPIFFS)」を選択
8. マイコンボードに書き込みボタンを押下
9. m5cam_post.inoを開く
10. マイコンボードに書き込みボタンを押下
11. m5cameraのUSBを抜く
12. m5cameraを好きな所に設置し，電源供給する

# サーバの準備

1. サーバ上でnode-redを起動
2. ブラウザでnode-redにアクセス
3. ブラウザで，次のページのjsonコードをコピー https://github.com/dzonesasaki/m5camera_httppost_sample/blob/master/server_node-red/httppost_server.json
4. node-redの三アイコンからimportで貼り付けしokし，ノードを設置し，Deploy
5. ブラウザで，次のURLを打ち込むと写真が出る サーバのIPアドレス:1880/get
