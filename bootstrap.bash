#!/usr/bin/env bash

usage="Usage: $0 [-h] [--recreate]"

diag ()
{
  echo "$*" 1>&2
}

recreate=

while test $# -ne 0; do
  case "$1" in
    -h|--help)
      diag
      diag "$usage"
      diag
      diag "Create or recreate MinGW32 debug and release configurations using"
      diag "\`bdep init\`. Note that an IW4x installation directory is"
      diag "required."
      diag
      diag "Options:"
      diag "  --recreate   Remove existing bdep state and generated host"
      diag "               configurations before initialization."
      diag
      exit 0
      ;;
    --recreate)
      recreate=yes
      shift
      ;;
    *)
      diag "error: unknown option '$1'"
      diag "$usage"
      exit 1
      ;;
  esac
done

if ! command -v bdep >/dev/null 2>&1; then
  diag "error: bdep not found in PATH"
  exit 1
fi

# Derive project directory basename.
#
project="$(basename "$(pwd)")"

# Cache path for IW4x installation root.
#
cache="${XDG_CACHE_HOME:-$HOME/.cache}/iw4x/bootstrap-root"

# Recreate mode: remove local build2 state.
#
if test -n "$recreate"; then
  diag "Recreating configurations: removing existing state..."

  if test -d .bdep; then
    rm -rf .bdep >/dev/null 2>&1 || true
  fi

  if test -d "../${project}-mingw32-debug-host"; then
    rm -rf "../${project}-mingw32-debug-host" >/dev/null 2>&1 || true
  fi

  if test -d "../${project}-mingw32-release-host"; then
    rm -rf "../${project}-mingw32-release-host" >/dev/null 2>&1 || true
  fi
fi

# Use IW4x installation root cache if available.
#
iw4x=
if test -f "$cache"; then
  cached="$(cat "$cache")"
  if test -d "$cached"; then
    iw4x="$cached"
  fi
fi

# If cache is empty, prompt for IW4x installation directory.
#
if test -z "$iw4x"; then
  diag "Specify the absolute path to the IW4x installation root:"
  printf "> "
  read -er iw4x

  if test -z "$iw4x"; then
    diag "error: empty path is not valid"
    exit 1
  fi

  if test ! -d "$iw4x"; then
    diag "error: directory '$iw4x' does not exist"
    exit 1
  fi

  # Normalize (strip trailing slash).
  #
  iw4x="${iw4x%/}"

  # Write cache.
  #
  mkdir -p "$(dirname "$cache")" >/dev/null 2>&1 && printf "%s\n" "$iw4x" > "$cache"
fi

diag
diag "Using IW4x installation root: $iw4x"
diag "Creating @mingw32-debug and @mingw32-release configurations..."
diag

# Debug configuration.
#
bdep init -C @mingw32-debug                                          \
  config.cxx=x86_64-w64-mingw32-g++                                  \
  config.cc.coptions="-ggdb                                          \
                      -grecord-gcc-switches                          \
                      -pipe                                          \
                      -mtune=generic                                 \
                      -fasynchronous-unwind-tables                   \
                      -fno-omit-frame-pointer                        \
                      -mno-omit-leaf-frame-pointer"                  \
  config.cc.compiledb=./                                             \
  cc                                                                 \
  config.install.filter='include/@false lib/@false share/@false'     \
  config.install.root="$iw4x"                                        \
  config.install.bin="$iw4x"                                         \
  --wipe

# Release configuration.
#
bdep init -C @mingw32-release                                        \
  config.cxx=x86_64-w64-mingw32-g++                                  \
  config.cc.coptions="-O2                                            \
                      -ggdb                                          \
                      -grecord-gcc-switches                          \
                      -pipe                                          \
                      -mtune=generic                                 \
                      -fasynchronous-unwind-tables                   \
                      -fno-omit-frame-pointer                        \
                      -mno-omit-leaf-frame-pointer"                  \
  config.cc.compiledb=./                                             \
  cc                                                                 \
  config.install.filter='include/@false lib/@false share/@false'     \
  config.install.root="$iw4x"                                        \
  config.install.bin="$iw4x"                                         \
  --wipe
