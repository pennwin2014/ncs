/* This file is here to prevent a file conflict on multiarch systems. A
 * conflict will occur because blkid_types.h has arch-specific definitions.
 *
 * DO NOT INCLUDE THE NEW FILE DIRECTLY -- ALWAYS INCLUDE THIS ONE INSTEAD. */

#if defined(__i386__)
#include "blkid_types-i386.h"
#elif defined(__powerpc64__)
#include "blkid_types-ppc64.h"
#elif defined(__powerpc__)
#include "blkid_types-ppc.h"
#elif defined(__s390x__)
#include "blkid_types-s390x.h"
#elif defined(__s390__)
#include "blkid_types-s390.h"
#elif defined(__x86_64__)
#include "blkid_types-x86_64.h"
#elif defined(__alpha__)
#include "blkid_types-alpha.h"
#elif defined(__arm__)
#include "blkid_types-arm.h"
#elif defined(__sparc__) && defined(__arch64__)
#include "blkid_types-sparc64.h"
#elif defined(__sparc__)
#include "blkid_types-sparc.h"
#else
#error "This e2fsprogs-devel package does not work your architecture?"
#endif

