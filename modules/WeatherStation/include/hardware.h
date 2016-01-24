#ifndef HARDWARE_H
#define HARDWARE_H

#define PCB0000 1

#if PCB0000
    #include "../source/PCB0000.h"
#else
    #error "Choose version of PCB to which project shall apply"
#endif

#endif /* end of HARDWARE_H */
