// CLI imposes a rigid naming scheme for generated files, restricting output to
// either an exact match of the CLI definition (iw4x.hxx) or a '-options' suffix
// variant (iw4x-options.hxx). From a user-facing perspective, iw4x-options is
// not naturally accessible; that is, it resides within a location that the
// broader system does not expose directly.
//

#pragma once

#include <libiw4x/iw4x-options.hxx>
