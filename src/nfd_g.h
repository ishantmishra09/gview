/*
 * nfd_g.h - Thin wrapper around the Native File Dialog ( NFD ) library
 *
 * Isolates the file-open dialog so the rest of the code doesn't depend on NFD
 * types directly.
 */

#pragma once

#include <nfd.h>

/* Opens a native file-open dialog filtered to supported image formats.
 * Returns a heap-allocated UTF-8 path on success, or NULL is the user cancelled
 * or an error occured. Caller must free with NFD_FreePathU8(). */
nfdchar_t *open_dialog(void);
