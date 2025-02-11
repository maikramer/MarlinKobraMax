/**
 * HAL for HC32F46x, based heavily on the legacy implementation and STM32F1
 */
#ifdef TARGET_HC32F46x

  #include "../../inc/MarlinConfig.h"
  #include "../shared/Delay.h"
  #include "HAL.h"
  #include "IWatchdog.h"
  #include "drivers/bsp_pwm.h"
  #include "drivers/sysclock/sysclock.h"
  #include "hc32_ddl.h"
  #include <core_hooks.h>
extern "C" char *_sbrk(int incr);

uint16_t MarlinHAL::adc_result;
uint32_t AD_DMA[3];

MarlinHAL::MarlinHAL() {}

// pclk = system_clock/div4 = 50M
// max cycle = 65536
// max feed interval = 65536 / (50000000/8192) = 10.7s
void MarlinHAL::watchdog_init() {
  #if ENABLED(USE_WATCHDOG)
  stc_wdt_init_t wdtConf = {.enCountCycle = WdtCountCycle65536,
                            .enClkDiv = WdtPclk3Div8192,
                            .enRefreshRange = WdtRefresh100Pct,
                            .enSleepModeCountEn = Disable,
                            .enRequestType = WdtTriggerResetRequest};
  WDT.begin(&wdtConf);
  #endif
}

void MarlinHAL::watchdog_refresh() {
  #if ENABLED(USE_WATCHDOG)
  WDT.reload();
  #endif
}

void MarlinHAL::init() {
  // Ensure F_CPU is a constant expression.
  // If the compiler breaks here, it means that delay code that should compute
  // at compile time will not work. So better safe than sorry here.

  NVIC_SetPriorityGrouping(0x3);

  // print clock frequencies to host serial
  SERIAL_LEAF_1.print("-- clocks dump -- \n");
  SERIAL_LEAF_1.printf("System: %u", SYSTEM_CLOCK_FREQUENCIES.system);
  SERIAL_LEAF_1.printf("hclk: %u", SYSTEM_CLOCK_FREQUENCIES.hclk);
  SERIAL_LEAF_1.printf("pclk0: %u", SYSTEM_CLOCK_FREQUENCIES.pclk0);
  SERIAL_LEAF_1.printf("pclk1: %u", SYSTEM_CLOCK_FREQUENCIES.pclk1);
  SERIAL_LEAF_1.printf("pclk2: %u", SYSTEM_CLOCK_FREQUENCIES.pclk2);
  SERIAL_LEAF_1.printf("pclk3: %u", SYSTEM_CLOCK_FREQUENCIES.pclk3);
  SERIAL_LEAF_1.printf("pclk4: %u", SYSTEM_CLOCK_FREQUENCIES.pclk4);
  SERIAL_LEAF_1.printf("exclk: %u", SYSTEM_CLOCK_FREQUENCIES.exclk);
  SERIAL_LEAF_1.printf("F_CPU=%u \n", F_CPU);
  SERIAL_LEAF_1.print("\n");

  #if HAS_MEDIA && DISABLED(ONBOARD_SDIO) && (defined(SDSS) && SDSS != -1)
  OUT_WRITE(
      SDSS,
      HIGH); // Try to set SDSS inactive before any other SPI users start up
  #endif

  #if PIN_EXISTS(LED)
  OUT_WRITE(LED_PIN, LOW);
  #endif

  #if PIN_EXISTS(AUTO_LEVEL_TX)
  OUT_WRITE(AUTO_LEVEL_TX_PIN, HIGH);
  delay(10);
  OUT_WRITE(AUTO_LEVEL_TX_PIN, LOW);
  delay(300);
  OUT_WRITE(AUTO_LEVEL_TX_PIN, HIGH);
  #endif
}

void MarlinHAL::init_board() {}

void MarlinHAL::reboot() { NVIC_SystemReset(); }

bool MarlinHAL::isr_state() { return !__get_PRIMASK(); }

void MarlinHAL::isr_on() { __enable_irq(); }

void MarlinHAL::isr_off() { __disable_irq(); }

void MarlinHAL::delay_ms(const int ms) { delay(ms); }

void MarlinHAL::idletask() { MarlinHAL::watchdog_refresh(); }

uint8_t MarlinHAL::get_reset_source() {
  // query reset cause
  stc_rmu_rstcause_t rstCause;
  MEM_ZERO_STRUCT(rstCause);
  RMU_GetResetCause(&rstCause);

  typedef enum {
    RST_CAU_POWER_ON = 0x01,
    RST_CAU_EXTERNAL = 0x02,
    RST_CAU_BROWN_OUT = 0x04,
    RST_CAU_WATCHDOG = 0x08,
    RST_CAU_JTAG = 0x10,
    RST_CAU_SOFTWARE = 0x20,
    RST_CAU_BACKUP = 0x40,
  } rst_cause_t;

  // map reset causes to those expected by Marlin
  uint8_t cause = RST_CAU_EXTERNAL;

  if (Set == rstCause.enSoftware) {
    cause = RST_CAU_SOFTWARE;
  } else if (Set == rstCause.enWdt) {
    cause = RST_CAU_WATCHDOG;
  } else if (Set == rstCause.enRstPin) {
    cause = RST_CAU_EXTERNAL;
  }

  return cause;
}

void MarlinHAL::clear_reset_source() { RMU_ClrResetFlag(); }

int MarlinHAL::freeMemory() {
  volatile char top;
  return &top - _sbrk(0);
}

void MarlinHAL::adc_init() {}

void MarlinHAL::adc_enable(const pin_t pin) {
  // just set pin mode to analog
  pinMode(pin, INPUT_ANALOG);
}

extern uint16_t g_adc_value[3];
void MarlinHAL::adc_start(const pin_t pin) {
  if (pin == TEMP_BED_PIN) {
    g_adc_idx = 0;
  } else if (pin == TEMP_0_PIN) {
    g_adc_idx = 1;
  } else if (pin == POWER_MONITOR_VOLTAGE_PIN) {
    g_adc_idx = 2;
  } else {
    g_adc_idx = 0x0;
  }
}

bool MarlinHAL::adc_ready() { return true; }

uint16_t MarlinHAL::adc_value() { return g_adc_value[g_adc_idx]; }

// void MarlinHAL::adc_start_conversion(const uint8_t adc_pin) {
//         if(adc_pin>BOARD_NR_GPIO_PINS)return;
//         uint8_t channel = PIN_MAP[adc_pin].adc_channel;
//         DDL_ASSERT(channel!=ADC_PIN_INVALID);
//         adc_result = adc_read(ADC1,channel);
//         switch(adc_pin)
//         {
//             case TEMP_BED_PIN: AD_DMA[0] = adc_result;break;
//             case TEMP_0_PIN: AD_DMA[1] = adc_result;break;
//             case POWER_MONITOR_VOLTAGE_PIN: AD_DMA[2] = adc_result;break;
//             default:break;
//         }
// }

void MarlinHAL::set_pwm_duty(const pin_t pin, const uint16_t v,
                             const uint16_t a, const bool b) {
  switch (pin) {
  case FAN0_PIN:
    fan_pwm_set_ratio(0, v);
    break;
  case FAN2_PIN:
    fan_pwm_set_ratio(2, v);
    break;
  }
}

void MarlinHAL::set_pwm_frequency(const pin_t pin, const uint16_t f_desired) {}

void flashFirmware(const int16_t) { MarlinHAL::reboot(); }

#endif // TARGET_HC32F46x
