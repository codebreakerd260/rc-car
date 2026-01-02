### **ESP8266 + L298N 2WD RC Car Wiring Diagram**

```text
       ESP8266 (NodeMCU / Wemos D1 Mini)
       --------------------------------
          D1  ---> IN1  (L298N Left motor forward)
          D2  ---> IN2  (L298N Left motor backward)
          D3  ---> IN3  (L298N Right motor forward)
          D4  ---> IN4  (L298N Right motor backward)
          D5  ---> ENA  (PWM speed control Left motor)
          D6  ---> ENB  (PWM speed control Right motor)
          GND ---> GND  (common ground with L298N & battery)
          3V3/5V ---> 5V (optional if using L298N onboard 5V for ESP)
          
       L298N Motor Driver
       ------------------
          IN1, IN2 -> Left motor
          IN3, IN4 -> Right motor
          ENA ---> D5 (PWM from ESP8266)
          ENB ---> D6 (PWM from ESP8266)
          +12V ---> Battery positive (+6V to +12V depending on motor)
          GND  ---> Battery negative (common with ESP8266 GND)
          
       DC Motors (2WD)
       ----------------
          Left motor  <-> OUT1 & OUT2 (L298N)
          Right motor <-> OUT3 & OUT4 (L298N)
          
       Battery Pack
       ------------
          +  ---> +12V on L298N
          -  ---> GND (shared with ESP8266 & L298N)
```

---

### **Notes**

1. **PWM pins:** D5 → ENA (left motor), D6 → ENB (right motor). This allows variable speed.
2. **Motor voltage:** Make sure battery voltage matches motor specs (6–12V typical for 2WD bot kits).
3. **Common ground:** Must connect ESP8266 GND ↔ L298N GND ↔ battery negative.
4. **Logic level:** ESP8266 3.3V pins are usually enough for L298N IN1–IN4. If not, use a **level shifter**.