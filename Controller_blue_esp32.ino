#include <Bluepad32.h>

#define in1 33
#define in2 32

#define in3 14
#define in4 27

const int ledPin1 = 16;
const int ledPin2 = 18;

GamepadPtr myGamepads[BP32_MAX_GAMEPADS];

long lastMillis;
const int freq = 5000;
const int ledChannel1 = 0;
const int ledChannel2 = 1;
const int resolution = 8;

int led = 0;

// This callback gets called any time a new gamepad is connected.
// Up to 4 gamepads can be connected at the same time.

void onConnectedGamepad(GamepadPtr gp) {
  bool foundEmptySlot = false;

  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myGamepads[i] == nullptr) {
      Serial.printf("CALLBACK: Gamepad is connected, index=%d\n", i);
      // Additionally, you can get certain gamepad properties like:
      // Model, VID, PID, BTAddr, flags, etc.
      GamepadProperties properties = gp->getProperties();
      Serial.printf("Gamepad model: %s, VID=0x%04x, PID=0x%04x\n",
                    gp->getModelName().c_str(), properties.vendor_id,
                    properties.product_id);
      myGamepads[i] = gp;
      foundEmptySlot = true;
      break;
    }
  }
  if (!foundEmptySlot) {
    Serial.println(
      "CALLBACK: Gamepad connected, but could not found empty slot");
  }
}

void onDisconnectedGamepad(GamepadPtr gp) {
  bool foundGamepad = false;

  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myGamepads[i] == gp) {
      Serial.printf("CALLBACK: Gamepad is disconnected from index=%d\n", i);
      myGamepads[i] = nullptr;
      foundGamepad = true;
      break;
    }
  }

  if (!foundGamepad) {
    Serial.println(
      "CALLBACK: Gamepad disconnected, but not found in myGamepads");
  }
}

// Arduino setup function. Runs in CPU 1
void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.printf("Firmware: %s\n", BP32.firmwareVersion());
  const uint8_t *addr = BP32.localBdAddress();
  Serial.printf("BD Addr: %2X:%2X:%2X:%2X:%2X:%2X\n", addr[0], addr[1], addr[2],
                addr[3], addr[4], addr[5]);

  // Setup the Bluepad32 callbacks
  BP32.setup(&onConnectedGamepad, &onDisconnectedGamepad);

  // "forgetBluetoothKeys()" should be called when the user performs
  // a "device factory reset", or similar.
  // Calling "forgetBluetoothKeys" in setup() just as an example.
  // Forgetting Bluetooth keys prevents "paired" gamepads to reconnect.
  // But might also fix some connection / re-connection issues.
  BP32.forgetBluetoothKeys();

  /* PWM channels*/
  ledcSetup(ledChannel1, freq, resolution);
  ledcAttachPin(ledPin1, ledChannel1);

  ledcSetup(ledChannel2, freq, resolution);
  ledcAttachPin(ledPin2, ledChannel2);

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void set_forward() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void set_backward() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}


// Arduino loop function. Runs in CPU 1
void loop() {
  // This call fetches all the gamepad info from the NINA (ESP32) module.
  // Just call this function in your main loop.
  // The gamepads pointer (the ones received in the callbacks) gets updated
  // automatically.
  BP32.update();

  // It is safe to always do this before using the gamepad API.
  // This guarantees that the gamepad is valid and connected.
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    GamepadPtr myGamepad = myGamepads[i];

    if (myGamepad && myGamepad->isConnected()) {
      // There are different ways to query whether a button is pressed.
      // By query each button individually:
      //  a(), b(), x(), y(), l1(), etc...
      if (myGamepad->a()) {
        static int colorIdx = 0;
        // Some gamepads like DS4 and DualSense support changing the color LED.
        // It is possible to change it by calling:
        switch (colorIdx % 3) {
          case 0:
            // Red
            myGamepad->setColorLED(255, 0, 0);
            break;
          case 1:
            // Green
            myGamepad->setColorLED(0, 255, 0);
            break;
          case 2:
            // Blue
            myGamepad->setColorLED(0, 0, 255);
            break;
        }
        colorIdx++;
      }

      if (myGamepad->b()) {
        // Turn on the 4 LED. Each bit represents one LED.
        static int led = 0;
        led++;
        // Some gamepads like the DS3, DualSense, Nintendo Wii, Nintendo Switch
        // support changing the "Player LEDs": those 4 LEDs that usually
        // indicate the "gamepad seat". It is possible to change them by
        // calling:
        myGamepad->setPlayerLEDs(led & 0x0f);
      }

      if (myGamepad->x()) {
        // Duration: 255 is ~2 seconds
        // force: intensity
        // Some gamepads like DS3, DS4, DualSense, Switch, Xbox One S support
        // rumble.
        // It is possible to set it by calling:
        myGamepad->setRumble(0xc0 /* force */, 0xc0 /* duration */);
      }

      int valor = myGamepad->buttons();
      if (valor == 1) {

        if (led == 0) {
          led = 1;
        } else {
          led = 0;
        }

        digitalWrite(LED_BUILTIN, led);
      }

      if (millis() - lastMillis > 50) {
        lastMillis = millis();
        Serial.print("Y: ");
        Serial.println(myGamepad->axisY());
        Serial.print("X: ");
        Serial.println(myGamepad->axisX());
        Serial.print("RY: ");
        Serial.println(myGamepad->axisRY());
        Serial.print("RX: ");
        Serial.println(myGamepad->axisRX());

        if (myGamepad->axisY() == 0 && myGamepad->axisX() == 0 && myGamepad->axisRX() == 0 && myGamepad->axisRY() == 0) {
          ledcWrite(ledChannel1, 0);
          ledcWrite(ledChannel2, 0);
          Serial.println("Duty zerado!");
        }

        // Negative Y -> going forward
        if (myGamepad->axisY() < 0 && myGamepad->axisX() == 0) {
          int duty = map(-myGamepad->axisY(), 0, 512, 200, 255);
          set_forward();

          ledcWrite(ledChannel1, duty);
          ledcWrite(ledChannel2, duty);
          Serial.println(duty);
        }

        // Positive RY -> going backwards
        if (myGamepad->axisY() > 0 && myGamepad->axisX() == 0) {
          int duty = map(myGamepad->axisY(), 0, 508, 200, 255);
          set_backward();

          ledcWrite(ledChannel1, duty);
          ledcWrite(ledChannel2, duty);
          Serial.println(duty);
        }

        // Viradas
        if (myGamepad->axisRY() == 0 && myGamepad->axisRX() != 0) {
          set_forward();


          if (myGamepad->axisRX() < 0) {
            // RX negativo -> indo pra esquerda (0 até -512)
            int duty = map(myGamepad->axisRX(), 0, -512, 128, 255);
            Serial.print("Duty de virada:");
            Serial.println(duty);

            ledcWrite(ledChannel2, duty);
            ledcWrite(ledChannel1, 0);
          } else {
            // RX positivo -> indo pra direita (0 até 508)
            int duty = map(myGamepad->axisRX(), 0, 508, 128, 255);
            Serial.print("Duty de virada:");
            Serial.println(duty);

            ledcWrite(ledChannel2, 0);
            ledcWrite(ledChannel1, duty);
          }
        }
      }
    }
  }

  // Yield to lower priority task
  vTaskDelay(1);
}
