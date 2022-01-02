# TM1638
TM1638 library for esp8266
![](http://arduinolearning.com/wp-content/uploads/2014/12/TM1638-Chip-Key-Display-Module.jpg)

## TM1638 control timing : 7-segment display & key scan

### 01. control registers																																													
<img width="730" alt="image" src="https://user-images.githubusercontent.com/32673738/123546217-a4824880-d796-11eb-8736-4bd7ad07e768.png">

### 02. 7-segment display bit allocation
<img width="730" alt="image" src="https://user-images.githubusercontent.com/32673738/123546238-b82daf00-d796-11eb-9f42-c23999610fce.png">

### 03. Data write timing
<img width="645" alt="image" src="https://user-images.githubusercontent.com/32673738/123546288-f1feb580-d796-11eb-81c0-cdaa15c80b6e.png">

### 04. Key bit allocation
<img width="611" alt="image" src="https://user-images.githubusercontent.com/32673738/123546311-10fd4780-d797-11eb-8f61-e0546a0c4241.png">

### 05. Data read (key scan) timing
<img width="1049" alt="image" src="https://user-images.githubusercontent.com/32673738/123546340-28d4cb80-d797-11eb-8d96-bc47e0070416.png">

### 06. 7 segment character display format
![image](https://user-images.githubusercontent.com/32673738/123546384-46a23080-d797-11eb-81bd-d56eb166dd60.png)
![image](https://user-images.githubusercontent.com/32673738/123546412-5e79b480-d797-11eb-84b0-f5e4bef6c6db.png)

## Example
```C++
#include <Arduino.h>
#include <TM1638.h>

TM1638 tm1638(5, 18, 19);

void setup() {
  Serial.begin(115200);
  tm1638.setup();
}

void loop() {
  tm1638.display("ABCDEFGH");
  tm1638.led(0xff);
  tm1638.getKey();
}
```