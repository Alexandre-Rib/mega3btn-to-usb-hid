#include <Arduino.h>
#include <Joystick.h>

// Report ID=1 pour correspondre à mega9.c côté STM32
// 8 boutons, aucun axe
Joystick_ Joystick(
    0x01,
    JOYSTICK_TYPE_GAMEPAD,
    8, 0,
    false, false, false,
    false, false, false,
    false, false, false, false, false
);

const int PIN_UP          = 5;
const int PIN_DOWN        = 4;
const int PIN_LEFT        = 3;
const int PIN_RIGHT       = 2;
const int PIN_BTN_B_A     = 8;
const int PIN_SELECT      = 7;
const int PIN_BTN_C_START = 6;

unsigned long lastMs   = 0;
uint8_t       prevBtns = 0;
bool          prevConn = false;

uint8_t readMegaDrive(bool *connected)
{
    digitalWrite(PIN_SELECT, LOW);
    delayMicroseconds(20);

    bool p3 = (digitalRead(PIN_LEFT)  == LOW);
    bool p4 = (digitalRead(PIN_RIGHT) == LOW);

    if (!p3 || !p4) {
        digitalWrite(PIN_SELECT, HIGH);
        *connected = false;
        return 0;
    }
    *connected = true;

    bool raw_up    = (digitalRead(PIN_UP)         == LOW);
    bool raw_down  = (digitalRead(PIN_DOWN)        == LOW);
    bool raw_a     = (digitalRead(PIN_BTN_B_A)     == LOW);
    bool raw_start = (digitalRead(PIN_BTN_C_START) == LOW);

    digitalWrite(PIN_SELECT, HIGH);
    delayMicroseconds(20);

    bool raw_left  = (digitalRead(PIN_LEFT)        == LOW);
    bool raw_right = (digitalRead(PIN_RIGHT)       == LOW);
    bool raw_b     = (digitalRead(PIN_BTN_B_A)     == LOW);
    bool raw_c     = (digitalRead(PIN_BTN_C_START) == LOW);

    if (Serial) {
        Serial.print("UP:"); Serial.print(raw_up);
        Serial.print(" DN:"); Serial.print(raw_down);
        Serial.print(" LF:"); Serial.print(raw_left);
        Serial.print(" RT:"); Serial.print(raw_right);
        Serial.print(" A:");  Serial.print(raw_a);
        Serial.print(" B:");  Serial.print(raw_b);
        Serial.print(" C:");  Serial.print(raw_c);
        Serial.print(" ST:"); Serial.println(raw_start);
    }

    uint8_t btns = 0;
    if (raw_up)    btns |= (1 << 0);
    if (raw_down)  btns |= (1 << 1);
    if (raw_left)  btns |= (1 << 2);
    if (raw_right) btns |= (1 << 3);
    if (raw_c)     btns |= (1 << 4);
    if (raw_b)     btns |= (1 << 5);
    if (raw_a)     btns |= (1 << 6);
    if (raw_start) btns |= (1 << 7);
    return btns;
}

void setup()
{
    Serial.begin(115200);
    Joystick.begin(false);   // false = pas d'envoi automatique

    pinMode(PIN_SELECT, OUTPUT);
    digitalWrite(PIN_SELECT, HIGH);
    pinMode(PIN_UP,           INPUT_PULLUP);
    pinMode(PIN_DOWN,         INPUT_PULLUP);
    pinMode(PIN_LEFT,         INPUT_PULLUP);
    pinMode(PIN_RIGHT,        INPUT_PULLUP);
    pinMode(PIN_BTN_B_A,      INPUT_PULLUP);
    pinMode(PIN_BTN_C_START,  INPUT_PULLUP);
}

void loop()
{
    unsigned long now = millis();
    if (now - lastMs < 10) return;
    lastMs = now;

    bool    connected;
    uint8_t btns = readMegaDrive(&connected);

    if (btns != prevBtns || connected != prevConn) {
        for (uint8_t i = 0; i < 8; i++)
            Joystick.setButton(i, connected ? ((btns >> i) & 1) : 0);
        Joystick.sendState();
    }

    prevBtns = btns;
    prevConn = connected;
}
