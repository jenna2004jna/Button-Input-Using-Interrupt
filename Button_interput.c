#include <stdint.h>

/* Pin definitions */
#define BUTTON_PIN   0
#define LED_PIN      1

/* Dummy GPIO registers */
volatile uint32_t GPIO_DIR;
volatile uint32_t GPIO_OUT;

/* Interrupt flag */
volatile uint8_t button_flag = 0;

/* Interrupt Service Routine (ISR) */
void BUTTON_ISR(void)
{
    button_flag = 1;    // Inform main loop
}

/* Main function */
int main(void)
{
    /* Configure LED as output */
    GPIO_DIR |= (1 << LED_PIN);

    /* Configure button as input */
    GPIO_DIR &= ~(1 << BUTTON_PIN);

    /* Enable button interrupt (hardware specific) */
    // enable_interrupt(BUTTON_PIN);

    while (1)
    {
        if (button_flag)
        {
            button_flag = 0;                 // Clear flag
            GPIO_OUT ^= (1 << LED_PIN);      // Toggle LED
        }

        /* CPU can do other work or sleep here */
    }
}
