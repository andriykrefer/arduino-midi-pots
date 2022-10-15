#define TOTAL_POTS (6)
#define TOTAL_SAMPLES (32)
#define CHANGE_TOLERANCE (2)

typedef struct {
    int pin;
    int val;
    int last_send_val;
    int samples[TOTAL_SAMPLES];
    int analog_val_start;
    int analog_val_end;
    uint8_t cc_number;
    uint8_t channel;
} st_pot;

st_pot pots[TOTAL_POTS];

void setup() {
    pots[0].pin = A5;
    pots[1].pin = A4;
    pots[2].pin = A3;
    pots[3].pin = A2;
    pots[4].pin = A1;
    pots[5].pin = A0;
    pots[0].analog_val_start = 0;
    pots[1].analog_val_start = 0;
    pots[2].analog_val_start = 0;
    pots[3].analog_val_start = 1024;
    pots[4].analog_val_start = 1024;
    pots[5].analog_val_start = 1024;
    pots[0].analog_val_end = 1024;
    pots[1].analog_val_end = 1024;
    pots[2].analog_val_end = 1024;
    pots[3].analog_val_end = 0;
    pots[4].analog_val_end = 0;
    pots[5].analog_val_end = 0;
    pots[0].cc_number = 14;
    pots[1].cc_number = 15;
    pots[2].cc_number = 16;
    pots[3].cc_number = 17;
    pots[4].cc_number = 18;
    pots[5].cc_number = 19;
    pots[0].channel = 0;
    pots[1].channel = 0;
    pots[2].channel = 0;
    pots[3].channel = 0;
    pots[4].channel = 0;
    pots[5].channel = 0;
    // Serial.begin(115200);
    Serial.begin(31250);
}

void loop() {
    for (int s=0; s<TOTAL_SAMPLES; s++) {
        for (int i=0; i<TOTAL_POTS; i++) {
            pots[i].samples[s] = analogRead(pots[i].pin);
        }
        delay(2);
    }
    // Calculate average and midi value
    for (int i=0; i<TOTAL_POTS; i++) {
        uint32_t avg = 0;
        for (int s=0; s<TOTAL_SAMPLES; s++) {
            avg += pots[i].samples[s];
        }
        avg = avg/TOTAL_SAMPLES;
        pots[i].val = map(
            avg,
            pots[i].analog_val_start,
            pots[i].analog_val_end,
            0,
            127
        );
    }

    // Send values that changed (by at least CHANGE_TOLERANCE)
    for (int i=0; i<TOTAL_POTS; i++) {
        int delta = pots[i].val - pots[i].last_send_val;
        delta = delta < 0 ? -delta : delta;
        bool has_changed = delta >= CHANGE_TOLERANCE;
        if (has_changed) {
            // Serial.print(pots[i].val);
            SendCC(pots[i].channel, pots[i].cc_number, pots[i].val);
            pots[i].last_send_val = pots[i].val;
        } else {
            // Serial.print("---");
        }
        // Serial.print(" | ");
    }
    // Serial.println("");
}

void SendCC(uint8_t ch, uint8_t cc_number, uint8_t val) {
    Serial.write(0xB0 + ch);
    Serial.write(cc_number);
    Serial.write(val);
}
