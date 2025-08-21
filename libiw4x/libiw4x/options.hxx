// CLI imposes a rigid naming scheme for generated files, restricting output to
// either an exact match of the CLI definition (iw4x.hxx) or a '-options' suffix
// variant (iw4x-options.hxx). This constraint is non-trivial: it dictates the
// organization of our source tree. To reconcile our desired file structure with
// the tool's limitations, options.hxx act as a transparent alias by including
// iw4x-options.hxx. This preserves the logical naming convention without
// violating the generator's rules.
//
// A secondary, less obvious motivation lies in visibility semantics. From a
// user-facing perspective, <libiw4x/iw4x-options.hxx> is not naturally
// accessible; that is, it resides within a location that the broader system
// does not expose directly.

#pragma once

#include <libiw4x/iw4x-options.hxx>
