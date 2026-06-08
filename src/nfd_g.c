/*
 * nfd_g.c - Native file-open dialog ( image formats only )
 */

#include "nfd_g.h"

#include <nfd.h>
#include <stdio.h>

/*
 * Shows a native OS file-open dialog pre-filtered to supported image types.
 * The NULL final argument means the dialog opens in the OS default directory.
 *
 * Returns the selected path ( allocated by NFD ) or NULL if cancelled/error.
 * The caller is responsible for freeing the path with NFD_FreePathU8().
 */
nfdchar_t *open_dialog(void) {

  nfdchar_t *outPath = NULL;

  nfdfilteritem_t filters[] = {
      {"Images", "png,jpg,jpeg,bmp,ppm,webp,tif,tiff"}};

  nfdresult_t result = NFD_OpenDialogU8(&outPath, filters, 1, NULL);
  if (result == NFD_OKAY) {

    return outPath;
  } else if (result == NFD_ERROR) {

    fprintf(stderr, "NFD error: %s\n", NFD_GetError());
  }
  /* NFD_CANCEL is silent - user just closed the dialog. */

  return NULL;
}
