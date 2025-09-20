# libiw4x-imgui - A C++ library

The `libiw4x-imgui` C++ library provides <SUMMARY-OF-FUNCTIONALITY>.


## Usage

To start using `libiw4x-imgui` in your project, add the following `depends`
value to your `manifest`, adjusting the version constraint as appropriate:

```
depends: libiw4x-imgui ^<VERSION>
```

Then import the library in your `buildfile`:

```
import libs = libiw4x-imgui%lib{<TARGET>}
```


## Importable targets

This package provides the following importable targets:

```
lib{<TARGET>}
```

<DESCRIPTION-OF-IMPORTABLE-TARGETS>


## Configuration variables

This package provides the following configuration variables:

```
[bool] config.libiw4x_imgui.<VARIABLE> ?= false
```

<DESCRIPTION-OF-CONFIG-VARIABLES>
