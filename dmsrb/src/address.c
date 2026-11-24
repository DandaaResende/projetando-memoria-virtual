
#include "address.h"

Address decode_address(unsigned int virtual_address)
{
    Address address;

    address.virtual_address = virtual_address;
    address.page_number = (virtual_address >> 8) & 0xFF;
    address.offset = virtual_address & 0xFF;

    return address;
}
