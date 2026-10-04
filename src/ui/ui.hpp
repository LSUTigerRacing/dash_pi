#ifndef UI_HPP
#define UI_HPP

#include "Protocol/uart.hpp"

void ui_init();
void ui_update(const UARTData &data);

#endif
