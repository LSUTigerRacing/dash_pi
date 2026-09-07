#ifndef UART_HPP
#define UART_HPP

#include <cstdint>
#include <mutex>

//  /\_/\
// ( o.o )
//  > ^ <
// Structure
struct UARTData
{
    uint8_t raw_buffer[256];
    int bytes_received;
    bool data_ready;

    // Brake data
    uint16_t brake_temperature; 
    // Coolant data
    uint16_t coolant_temperature; 

    // CAN IDs for other systems
    uint16_t can_id_1;   
    uint8_t can_data[8]; 

    // Motor control data 
    uint16_t motor_voltage;     
    uint16_t motor_current;     
    uint16_t motor_temperature; 
    uint16_t motor_rpm;         
    uint8_t motor_status;       

    // Fault data
    uint8_t fault_code;     
    uint8_t fault_severity; 
    bool has_active_fault;  
};
void uart_init();
void uart_update();
UARTData uart_get_data();
bool uart_has_new_data();
void uart_cleanup();

#endif