/* Arduino SdFat Library
 * Copyright (C) 2012 by William Greiman
 *
 * This file is part of the Arduino SdFat Library
 *
 * This Library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with the Arduino SdFat Library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */
#include <SdFat.h>
#include <SdFatUtil.h>
#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#endif  // __arm__
//------------------------------------------------------------------------------
/** Amount of free RAM
 * \return The number of free bytes.
 */
int SdFatUtil::FreeRam() {
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#else  // __arm__
  extern char *__malloc_heap_start;
  extern char *__brkval;
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}
//------------------------------------------------------------------------------
/** %Print a string in flash memory.
 *
 * \param[in] pr Print object for output.
 * \param[in] str Pointer to string stored in flash memory.
 */
void SdFatUtil::print_P(Print* pr, PGM_P str) {
  for (uint8_t c; (c = pgm_read_byte(str)); str++) pr->write(c);
}
//------------------------------------------------------------------------------
/** %Print a string in flash memory followed by a CR/LF.
 *
 * \param[in] pr Print object for output.
 * \param[in] str Pointer to string stored in flash memory.
 */
void SdFatUtil::println_P(Print* pr, PGM_P str) {
  print_P(pr, str);
  pr->println();
}
//------------------------------------------------------------------------------
/** %Print a string in flash memory to Serial.
 *
 * \param[in] str Pointer to string stored in flash memory.
 */
void SdFatUtil::SerialPrint_P(PGM_P str) {
  print_P(SdFat::stdOut(), str);
}
//------------------------------------------------------------------------------
/** %Print a string in flash memory to Serial followed by a CR/LF.
 *
 * \param[in] str Pointer to string stored in flash memory.
 */
void SdFatUtil::SerialPrintln_P(PGM_P str) {
  println_P(SdFat::stdOut(), str);
}
