#ifdef __FreeBSD__
#include <ieeefp.h>
#include <floatingpoint.h>
#endif
#include <libYowAI.h>
#include "myagent.h"

SoccerAgent *sagent;

void main(int argc, char** argv) {
#ifdef __FreeBSD__
  /* allow divide by zero -- Inf */
  fpsetmask(fpgetmask() & ~(FP_X_DZ|FP_X_INV));
#endif

  sagent = new MyAgent;

  sagent->init(argc, argv);
  sagent->main_loop();
}
