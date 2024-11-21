#include "MIDIUSB.h"
byte midi_start = 0xfa;
byte midi_stop = 0xfc;
byte midi_clock = 0xf8;
byte midi_continue = 0xfb;
long bpm = 80.0;
long int ticks = 0 ;
float interval = 60L * 1000 * 1000 / bpm / 24 ;
long int currentTs;
long int nextTs = 0;
bool running = true;
bool startstop = true;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(12, INPUT_PULLUP);//Start button
  pinMode(11, INPUT_PULLUP);//Stop-continue
  
  while ( digitalRead(12) == true){};
  midiEventPacket_t msg = {0x0F, midi_start, 0x00, 0x00};
  MidiUSB.sendMIDI(msg);
  MidiUSB.flush();
  //Serial.write(midi_start);
  //Serial.println("midi_start");
  delay(1);
  nextTs= 0;
}

void loop() { 
  currentTs=micros();
  if ( nextTs < currentTs){
    //Serial.println(currentTs);
    //Serial.println(nextTs);
    nextTs = currentTs + interval;
    if(digitalRead(11)==true){
      if(running == false){
        running = true;
        midiEventPacket_t msg = {0x0F, midi_continue, 0x00, 0x00}   ;
        MidiUSB.sendMIDI(msg);
        //MidiUSB.sendMIDI({0x0F, midi_continue, 0x00, 0x00});
        MidiUSB.flush();
        //Serial.write(midi_continue);
        //Serial.println("midi_start");
        delay(1);
      }
    }
    if (running) {
      midiEventPacket_t msg = {0x0F, midi_clock, 0x00, 0x00};
      MidiUSB.sendMIDI(msg);
      MidiUSB.flush();
      //MidiUSB.sendMIDI({0x0F, midi_clock, 0x00, 0x00});
      //Serial.println("midi_clock");
      if (ticks++%24 == 0){
        digitalWrite(LED_BUILTIN, true);
      }
      else {digitalWrite(LED_BUILTIN, false);}
    }
    if(digitalRead(11)==false){
      if(running == true){
        running = false;
        delay(1);
        midiEventPacket_t msg = {0x0F, midi_stop, 0x00, 0x00};
        MidiUSB.sendMIDI(msg);
        //MidiUSB.sendMIDI({0x0F, midi_stop, 0, 0});
        MidiUSB.flush();
        //Serial.write(midi_stop);
        //Serial.println("midi_stop");
      }
    }
  }
}
