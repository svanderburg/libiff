#ifndef __CATDATA_H
#define __CATDATA_H

#include <ifftypes.h>

#define HELO_1_BYTES_SIZE 4
#define BYE_1_BYTES_SIZE 3
#define HELO_2_BYTES_SIZE 5
#define BYE_2_BYTES_SIZE 4

IFF_UByte helo1Data[] = {'a', 'b', 'c', 'd'};
IFF_UByte bye1Data[] = {'E', 'F', 'G'};
IFF_UByte helo2Data[] = {'a', 'b', 'c', 'd', 'e'};
IFF_UByte bye2Data[] = {'F', 'G', 'H', 'I'};

#endif
