
#pragma once

#ifndef PC_DEBUG
void InitLED()
{
  // Setup GPIO Port A, Pin 5 (LED)
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;          // Enable GPIOA clock
  GPIOA->MODER |= (1 << GPIO_MODER_MODE5_Pos);  // Output
}

inline void EnableLED()
{
  // Enable GPIO Port A, Pin 5 (LED)
  GPIOA->ODR |= (1 << GPIO_OTYPER_OT5_Pos);   // Enable LED
}

inline void DisableLED()
{
  // Disable GPIO Port A, Pin 5 (LED)
  GPIOA->ODR &= ~(1 << GPIO_OTYPER_OT5_Pos);  // Disable LED
}

inline void ToggleLED()
{
  // Toggle GPIO Port A, Pin 5 (LED)
  GPIOA->ODR ^= (1 << GPIO_OTYPER_OT5_Pos);   // Toggle LED
}
#endif
