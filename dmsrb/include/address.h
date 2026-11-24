
#ifndef ADDRESS_H
#define ADDRESS_H

typedef struct {
    unsigned int virtual_address;
    unsigned int page_number;
    unsigned int offset;
} Address;

Address decode_address(unsigned int virtual_address);

#endif
