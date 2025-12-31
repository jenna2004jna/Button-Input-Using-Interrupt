# 🔔 Button Input Using Interrupt — Embedded C

A simple example that demonstrates handling a push button using an interrupt in Embedded C.  
Instead of continuously polling the button, the microcontroller responds only when the button is pressed, making the system more efficient and suitable for real products.

- Button pressed → Interrupt triggers → LED toggles

---

## Project Overview

This project is intended for learners who want to understand:
- Hardware interrupts and ISRs (Interrupt Service Routines)
- Safe ISR-to-main communication using volatile flags
- Efficient GPIO handling compared to polling

Interrupts immediately divert program flow to an ISR when an event occurs (e.g., a falling edge on a pin). Keep ISRs short and defer work to the main loop via flags or lightweight queues.

---

## Hardware Requirements

- Microcontroller (generic concept; code is portable to Arduino, STM32, AVR, 8051, etc.)
- Push button
- LED
- 220Ω resistor (for LED)
- Pull-up resistor (e.g., 10kΩ) if MCU doesn't provide internal pull-ups
- Breadboard & jumper wires

---

## Assumptions & Pin Mapping

- Button connected to PIN 0 (active LOW)
  - Pressed → 0
  - Released → 1
- LED connected to PIN 1
- Interrupt triggered on falling edge (pressed)
- If port/interrupt mapping differs on your MCU, update pin and interrupt configuration accordingly

---

## Wiring (conceptual)

- LED anode → MCU PIN 1 (via 220Ω resistor)  
- LED cathode → GND  
- One side of the button → MCU PIN 0  
- Other side of the button → GND  
- Add pull-up:
  - External: connect PIN 0 to VCC via 10kΩ
  - Or enable internal pull-up if MCU supports it

Because the button is active LOW, pressing it pulls PIN 0 to GND and produces a falling edge.

---

## Embedded C Example (platform-agnostic)

This minimal example uses dummy registers to demonstrate ISR and main-loop flag handling. Replace comments and register names with real MCU API (HAL, direct registers, or Arduino functions).

```c
#include <stdint.h>

/* Pin definitions */
#define BUTTON_PIN   0
#define LED_PIN      1

/* Dummy GPIO registers (for understanding) */
volatile uint32_t GPIO_DIR;
volatile uint32_t GPIO_IN;
volatile uint32_t GPIO_OUT;

/* Interrupt flag set by ISR and consumed in main loop */
volatile uint8_t button_flag = 0;

/* Interrupt Service Routine
   Replace the name/signature with the MCU-specific ISR declaration.
   Keep ISR short — just set a flag or read minimal state.
*/
void BUTTON_ISR(void)
{
    /* Clear the hardware interrupt flag here if required by the MCU */
    button_flag = 1;   // Notify main loop that button was pressed
}

int main(void)
{
    /* Configure LED pin as output */
    GPIO_DIR |= (1 << LED_PIN);

    /* Configure button pin as input */
    GPIO_DIR &= ~(1 << BUTTON_PIN);

    /* Configure interrupt on falling edge for BUTTON_PIN (hardware-specific)
       e.g., enable_external_interrupt_falling(BUTTON_PIN);
    */

    while (1)
    {
        if (button_flag)
        {
            button_flag = 0;               // Clear flag
            GPIO_OUT ^= (1 << LED_PIN);    // Toggle LED atomically
        }

        /* Main loop can perform other tasks or enter low-power mode.
           Waking on external interrupt will run the ISR which sets the flag.
        */
    }
}
```

---

## How it works (short)

- Hardware interrupt is configured on button pin for a falling edge.
- When the button is pressed, the MCU runs BUTTON_ISR().
- ISR sets a volatile flag and returns quickly.
- Main loop sees the flag and toggles the LED.
- Using a volatile flag prevents compiler optimizations that could hide updates made in ISRs.

---

## Practical Notes & Best Practices

- Keep ISRs as short as possible; do not call blocking functions or long delays inside an ISR.
- Use volatile for variables shared between ISR and main code.
- If multiple tasks require synchronization, use proper atomic operations or disable/enable interrupts around critical sections.
- Debounce the switch:
  - Hardware: RC filter or dedicated debounce circuit.
  - Software: ignore further interrupts for a short debounce interval, or check stability in the main loop after ISR sets the flag.
- Clear the MCU's interrupt pending/flag status in the ISR if the hardware requires it.
- Choose edge (falling/rising) or level-triggering appropriately based on wiring.

---

## Porting Tips

- Arduino: use attachInterrupt(digitalPinToInterrupt(pin), isr, FALLING) and digitalWrite/digitalRead or direct port manipulation.
- STM32 (HAL): configure EXTI line and NVIC, implement the HAL_GPIO_EXTI_Callback or IRQHandler, and clear pending flags.
- AVR: configure EICRA/EICRB and EIMSK, implement ISR(INTn_vect) and clear flags if needed.
- 8051: configure IE and TCON for external interrupts INT0/INT1 and write the ISR accordingly.

---

## Advantages vs Polling

- Lower CPU usage (can sleep until interrupt)
- Better power efficiency
- Faster response and better for real-time systems

---

## Limitations & Improvements

- Mechanical switches bounce — add debounce logic
- Frequent interrupts from noisy inputs may cause jitter — consider filtering
- For more complex behavior, use edge detection and event queues or an RTOS
- Combine with low-power sleep modes to wake only on button press

---

## Learning Outcomes

- How to configure and use hardware interrupts
- Writing safe and short ISRs
- ISR-to-main communication using volatile flags
- Efficient event-driven embedded design

---

## Author

Jenna  
Embedded Systems | LabVIEW | Embedded C

---

## License

MIT — reuse and adapt freely. Please retain attribution when sharing.
