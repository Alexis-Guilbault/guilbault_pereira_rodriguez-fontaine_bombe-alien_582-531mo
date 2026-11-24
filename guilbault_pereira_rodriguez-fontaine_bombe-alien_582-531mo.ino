#include <Arduino.h>
#include <MicroOscSlip.h>
#include <Bounce2.h>
#include <Chrono.h>
#include <M5_Encoder.h>

MicroOscSlip<128> monOsc(&Serial);
Bounce2::Button bouton;
Chrono chronoPot;
M5_Encoder monEncodeur;

void setup()
{
    Serial.begin(115200);

    bouton.attach(2, INPUT_PULLUP);
    bouton.setPressedState(LOW);
    pinMode(3, OUTPUT);

    Wire.begin();
    monEncodeur.begin();
}

void loop()
{
    bouton.update();
    monEncodeur.update();
    monOsc.onOscMessageReceived(traiterMessageOsc);

    if (bouton.pressed())
    {
        monOsc.sendInt("/but", 1);
    }

    if (chronoPot.hasPassed(20))
    {
        chronoPot.restart();

        int valeur = analogRead(2);

        monOsc.sendInt("/pot", valeur);
    }

    if (chronoPot.hasPassed(20))
    {
        chronoPot.restart();

        int valeur = analogRead(3);

        monOsc.sendInt("/photo", valeur);
    }

    int changementEncodeur = monEncodeur.getEncoderChange();
    if (changementEncodeur != 0)
    {
        monOsc.sendInt("/enc/rot", changementEncodeur);
    }
}

void traiterMessageOsc(MicroOscMessage &message)
{
    if (message.checkOscAddressAndTypeTags("/lumiere", "i"))
    {
        int valeur = message.nextAsInt();
        digitalWrite(3, valeur);
    }
}
