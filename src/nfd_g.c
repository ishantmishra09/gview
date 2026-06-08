#include "nfd_g.h"

#include <nfd.h>
#include <stdio.h>

nfdchar_t *open_dialog(void) {

  nfdchar_t *outPath = NULL;

  nfdfilteritem_t filters[] = {{"Images", "png,jpg,jpeg,bmp,ppm,webp,tif,tiff"}};

  nfdresult_t result = NFD_OpenDialogU8(&outPath, filters, 1, NULL);
  if (result == NFD_OKAY) {

    return outPath;
  } else if (result == NFD_ERROR) {

    fprintf(stderr, "NFD error: %s\n", NFD_GetError());
  }

  return NULL;
}
