#include <linux/spi/spidev.h>
#include <fnctrl.h>
#include <string>
#include <unistd.h>
#include <string>

class SPIDevice{
private{
		int file_descriptor;
		std:: string device_filepath
}
	
public{
	bool openDevice(){
	file_descriptor = open(device_filepath)
	if (file_descriptor < 0) {
		return false;
		}
	return true;
	}
		
	bool closeDevice(){
		file_descriptor = close(device_filepath)
	if (file_descriptor == 0){
			return true;
	}	
	return false;
	}
}	
};
