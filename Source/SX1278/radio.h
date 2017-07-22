#ifndef __RADIO_H__
#define __RADIO_H__
#include "defines.h"
#include "lora_types.h"


/*!
 * \brief Initializes the RadioDriver structure with specific radio
 *        functions.
 *
 * \retval radioDriver Pointer to the radio driver variable
 */
RadioDriver_t *RadioDriverInit(void);
#endif // __RADIO_H__
