#define _POSIX_C_SOURCE 200112L

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

#include "mzapo_lib/mzapo_parlcd.h"
#include "mzapo_lib/mzapo_phys.h"
#include "mzapo_lib/mzapo_regs.h"
#include "mzapo_lib/serialize_lock.h"

//macros
//LCD Macros
#define SCREEN_WIDTH
#define SCREEN_HEIGHT

int main(int argc, char *argv[])
{

  /* Serialize execution of applications */

  /* Try to acquire lock the first */
  if (serialize_lock(1) <= 0) {
    printf("System is occupied\n");

    if (1) {
      printf("Waitting\n");
      /* Wait till application holding lock releases it or exits */
      serialize_lock(0);
    }
  }

  serialize_unlock();

  return 0;
}
