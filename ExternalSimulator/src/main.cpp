#include <Arduino.h>
#include <SoftwareSerial.h>
// #define RX 2
// #define TX 3
// #define WAIT_FOR_SERIAL(x) while (Serial.available() < x)
// const int LED = 13;
// SoftwareSerial Simulator(RX, TX);
uint8_t _frontCameraTemp[6] = {0x64, 0x04, 0x00, 0x00, 0x00, 0x00};
uint8_t _rearCameraTemp[6] = {0x64, 0x04, 0x00, 0x01, 0x00, 0x00};
uint8_t _boxTemp[6] = {0x64, 0x04, 0x00, 0x04, 0x00, 0x00};
uint8_t _pressure[6] = {0x64, 0x04, 0x00, 0x05, 0x00, 0x00};
uint8_t _turnOnGreenLamp[6] = {0x64, 0x06, 0x00, 0x00, 0x00, 0x01};
uint8_t _turnOffGreenLamp[6] = {0x64, 0x06, 0x00, 0x00, 0x00, 0x00};
uint8_t _turnOnYellowLamp[6] = {0x64, 0x06, 0x00, 0x01, 0x00, 0x01};
uint8_t _turnOffYellowLamp[6] = {0x64, 0x06, 0x00, 0x01, 0x00, 0x00};
uint8_t _turnOnRedLamp[6] = {0x64, 0x06, 0x00, 0x02, 0x00, 0x01};
uint8_t _turnOffRedLamp[6] = {0x64, 0x06, 0x00, 0x02, 0x00, 0x00};
uint8_t _turnOnRelay[6] = {0x64, 0x06, 0x00, 0x03, 0x00, 0x01};
uint8_t _turnOffRelay[6] = {0x64, 0x06, 0x00, 0x03, 0x00, 0x00};
uint8_t _turnOnModem[6] = {0x64, 0x06, 0x00, 0x04, 0x00, 0x01};
uint8_t _turnOffModem[6] = {0x64, 0x06, 0x00, 0x04, 0x00, 0x00};
uint8_t _turnOnFan1[6] = {0x64, 0x06, 0x00, 0x05, 0x00, 0x01};
uint8_t _turnOffFan1[6] = {0x64, 0x06, 0x00, 0x05, 0x00, 0x00};
uint8_t _turnOnFan2[6] = {0x64, 0x06, 0x00, 0x06, 0x00, 0x01};
uint8_t _turnOffFan2[6] = {0x64, 0x06, 0x00, 0x06, 0x00, 0x00};

// void Wait(int high, int low)
// {
//   digitalWrite(LED, HIGH);
//   delay(high);
//   digitalWrite(LED, LOW);
//   delay(low);
// }

void SendData(int8_t *data)
{
  // Serial.print("\nPacket: ");
  for (int i = 0; i < 6; i++)
  {
    Serial.print(data[i]);
    // Simulator.print(data[i]);
    // Serial.print(data[i]);
  }
}

// void SendAllData()
// {
//   SendData(_frontCameraTemp);
//   Wait(1, 4);
//   SendData(_rearCameraTemp);
//   Wait(1, 4);
//   SendData(_boxTemp);
//   Wait(1, 4);
//   SendData(_pressure);
//   Wait(1, 4);
//   SendData(_turnOnGreenLamp);
//   Wait(1, 4);
//   SendData(_turnOffGreenLamp);
//   Wait(1, 4);
//   SendData(_turnOnYellowLamp);
//   Wait(1, 4);
//   SendData(_turnOffYellowLamp);
//   Wait(1, 4);
//   SendData(_turnOnRedLamp);
//   Wait(1, 4);
//   SendData(_turnOffRedLamp);
//   Wait(1, 4);
//   SendData(_turnOnRelay);
//   Wait(1, 4);
//   SendData(_turnOffRelay);
//   Wait(1, 4);
//   SendData(_turnOnModem);
//   Wait(1, 4);
//   SendData(_turnOffModem);
//   Wait(1, 4);
//   SendData(_turnOnFan1);
//   Wait(1, 4);
//   SendData(_turnOffFan1);
//   Wait(1, 4);
//   SendData(_turnOnFan2);
//   Wait(1, 4);
//   SendData(_turnOffFan2);
// }

void setup()
{
  Serial.begin(115200);
  // Simulator.begin(9600);
  // pinMode(LED, OUTPUT);
}

bool executed = false;

void loop()
{
  // if (!executed)
  // {
  //   SendAllData();
  //   executed = true;
  // }
  // SendAllData();
  // SendData(_frontCameraTemp);
  // WAIT_FOR_SERIAL(3);
  // int8_t resp[3];
  // resp[0] = Serial.read();
  // resp[1] = Serial.read();
  // resp[2] = Serial.read();
  // SendData(resp);
  // Serial.print(0x48);
  Serial.write(_frontCameraTemp, 6);
  delay(3000);
  Serial.write(_turnOnGreenLamp, 6);
  delay(3000);
  Serial.write(_turnOffRedLamp, 6);
  delay(3000);
  Serial.write(_turnOffFan2, 6);
  delay(3000);
}
