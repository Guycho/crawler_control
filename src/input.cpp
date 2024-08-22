#include "input.h"

void on_connect();
void on_disConnect();
void controller_do();
void remove_paired_devices();
float calc_throttle(uint8_t l2, uint8_t r2);
float calc_steering(int8_t lx);

float m_throttle;
float m_steering;
bool m_steering_mode_toggle;
bool m_throttle_mode_toggle;
bool m_arm_toggle;

uint8_t m_dead_band;

void init_ps4(const char* mac, float dead_band) {
    PS4.attach(controller_do);
    PS4.attachOnConnect(on_connect);
    PS4.attachOnDisconnect(on_disConnect);
    PS4.begin(mac);
    remove_paired_devices();  // This helps to solve connection issues
    m_dead_band = dead_band;
}

float get_throttle() { return m_throttle; }
float get_steering() { return m_steering; }

bool get_steering_mode_toggle() {
    bool temp = m_steering_mode_toggle;
    m_steering_mode_toggle = false;
    return temp;
}

bool get_throttle_mode_toggle() {
    bool temp = m_throttle_mode_toggle;
    m_throttle_mode_toggle = false;
    return temp;
}

bool get_arm_toggle() {
    bool temp = m_arm_toggle;
    m_arm_toggle = false;
    return temp;
}

void remove_paired_devices() {
    uint8_t pairedDeviceBtAddr[20][6];
    int count = esp_bt_gap_get_bond_device_num();
    esp_bt_gap_get_bond_device_list(&count, pairedDeviceBtAddr);
    for (int i = 0; i < count; i++) {
        esp_bt_gap_remove_bond_device(pairedDeviceBtAddr[i]);
    }
}

void controller_do() {
    boolean sqd = PS4.event.button_down.square, squ = PS4.event.button_up.square,
            trd = PS4.event.button_down.triangle, tru = PS4.event.button_up.triangle,
            crd = PS4.event.button_down.cross, cru = PS4.event.button_up.cross,
            cid = PS4.event.button_down.circle, ciu = PS4.event.button_up.circle,
            upd = PS4.event.button_down.up, rid = PS4.event.button_down.right,
            dod = PS4.event.button_down.down, lid = PS4.event.button_down.left;

    boolean sq = PS4.Square(), tr = PS4.Triangle(), cr = PS4.Cross(), ci = PS4.Circle();

    int8_t lx = PS4.LStickX(), ly = PS4.LStickY(), rx = PS4.RStickX(), ry = PS4.RStickY();

    uint8_t l2 = PS4.L2Value(), r2 = PS4.R2Value();

    int16_t gx = PS4.GyrX(), gy = PS4.GyrY(), gz = PS4.GyrZ(), ax = PS4.AccX(), ay = PS4.AccY(),
            az = PS4.AccZ();

    if (trd) {
        m_steering_mode_toggle = true;
    }

    if(cid) {
        m_throttle_mode_toggle = true;
    }

    if (crd) {
        m_arm_toggle = true;
    }

    m_throttle = calc_throttle(l2, r2);
    m_steering = calc_steering(lx);
}

float calc_throttle(uint8_t l2, uint8_t r2) {
    float temp = Utils::Calcs::map_float((r2 - l2), -255, 255, -100, 100);
    float scaled = Utils::Calcs::calc_dead_band(temp, 100, m_dead_band);
    return scaled;
}

float calc_steering(int8_t lx) {
    float temp = Utils::Calcs::map_float(lx, -127, 127, -100, 100);
    float scaled = Utils::Calcs::calc_dead_band(temp, 100, m_dead_band);
    return scaled;
}

void on_connect() {}

void on_disConnect() {}
