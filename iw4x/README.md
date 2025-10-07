# iw4x - An executable

The `iw4x` executable is a <SUMMARY-OF-FUNCTIONALITY>.

Note that the `iw4x` executable in this package provides `build2` metadata.


## Usage

To start using `iw4x` in your project, add the following build-time
`depends` value to your `manifest`, adjusting the version constraint as
appropriate:

```
depends: * iw4x ^<VERSION>
```

Then import the executable in your `buildfile`:

```
import! [metadata] <TARGET> = iw4x%exe{<TARGET>}
```


## Importable targets

This package provides the following importable targets:

```
exe{<TARGET>}
```

<DESCRIPTION-OF-IMPORTABLE-TARGETS>


## Configuration variables

This package provides the following configuration variables:

```
[bool] config.iw4x.<VARIABLE> ?= false
```

<DESCRIPTION-OF-CONFIG-VARIABLES>
