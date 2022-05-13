uint8_t inByte = 0;

struct JetsonMessage {
    uint64_t magic;
    float targetYawOffset;
    float targetPitchOffset;
    uint8_t cvState;
} __attribute__((packed));

static_assert(sizeof(JetsonMessage) == 17);

JetsonMessage testMsg = {
    .magic = 0xabcdef0123456789,  // 1002855686552083439, is close to the max size of 9223372036854775807
    .targetYawOffset = 69.0f,
    .targetPitchOffset = 420.0f,
    .cvState = 0,  // "int" encase we need to last-second cram some other data in during the competition
};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); //This pipes to the serial monitor
  while(!Serial);
  Serial1.begin(115200); //This is the UART, pipes to sensors attached to board
}

void loop() {
  // put your main code here, to run repeatedly:
  testMsg.targetYawOffset = testMsg.targetYawOffset + 1.0f;
  testMsg.targetPitchOffset = testMsg.targetPitchOffset + 1.0f;
  testMsg.cvState = 0 ? 1 : 0;
  uint8_t *buffer = reinterpret_cast<uint8_t*>(&testMsg);
  size_t len = static_cast<size_t>(sizeof(testMsg));
  if (Serial1.availableForWrite()){
    // digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
    Serial1.write(buffer, len);
  }
  if (Serial1.available()){
    Serial1.readBytes(&inByte, 1);
  }
  if (Serial.availableForWrite()){
    // Serial.write(buffer, len);
    Serial.print("\nx offset: ");
    Serial.print(testMsg.targetYawOffset);
    Serial.print("\ny offset: ");
    Serial.print(testMsg.targetPitchOffset);
    Serial.print("\ncv state: ");
    Serial.print(testMsg.cvState);
  }
  delay(1000);
}
