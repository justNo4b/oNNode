#include "uci.h"
#include "attacks.h"
#include "rays.h"

int main() {
  Rays::init();
  PSquareTable::init();
  ZKey::init();
  Attacks::init();
  Uci::init();

  Uci::start();

  return 0;
}
