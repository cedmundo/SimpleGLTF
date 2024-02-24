#include "core.h"

#define WINDOW_WIDTH 500
#define WINDOW_HEIGTH 500
#define WINDOW_TITLE "SimpleGLTF"

int main() {
  StatusCode status;

  status = AppInit(WINDOW_WIDTH, WINDOW_HEIGTH, WINDOW_TITLE);
  if (status != SUCCESS) {
    return AppClose(status);
  }

  while (!AppShouldClose()) {
    BeginFrame();
    // App code
    EndFrame();
  }

  return AppClose(SUCCESS);
}
