//========================================
//
//  tagreader.c | C library to get data from tagreader on Raspberry Pi
//  Written by Abdil Kaya & Robin Janssens
//  Commissioned by University of Antwerp
//
//========================================

#ifndef TAGREADER_H
#define TAGREADER_H

#include "robotapp.h"

/**
 * \brief Scan for passive tag in range, return 7 byte UID
 * \param Data :Location to store tag UID. Buffer must be 21 bytes long at least. UID is stored as a NULL terminated string
 * \return
 * 0:OK \n
 * 1:Tag reader error \n
 * 2:No tag detected \n
 * 3:Other error \n
*/
int TagReaderGetUID(char *Data);


#endif
