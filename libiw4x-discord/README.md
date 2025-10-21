# libiw4x-discord - A C++ library

The `libiw4x-discord` C++ library provides <SUMMARY-OF-FUNCTIONALITY>.


## Usage

To start using `libiw4x-discord` in your project, add the following `depends`
value to your `manifest`, adjusting the version constraint as appropriate:

```
depends: libiw4x-discord ^<VERSION>
```

Then import the library in your `buildfile`:

```
import libs = libiw4x-discord%lib{<TARGET>}
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
[bool] config.libiw4x_discord.<VARIABLE> ?= false
```

<DESCRIPTION-OF-CONFIG-VARIABLES>
