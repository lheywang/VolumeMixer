# How to reduce executable size :
## Rewrite stdlib functions to get smaller ones
- strtod   --> 2.95 kB !!
- __gethex --> 1.03 kB !!

## Rewrite HAL functions
- HAL\_PCB\_IRQHandler --> 2.23 kB !!
- HAL\_RCC\_OscConfig  --> 1.18 kB !!

## Total :
- 7.39 kB (12% of FLASH !!)
