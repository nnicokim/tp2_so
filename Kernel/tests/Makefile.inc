# Detectar el compilador, usando clang por defecto si no está definido
CC ?= $(shell which gcc > /dev/null 2>&1 && echo gcc || echo clang)

# Opciones de compilación
COMPILER_FLAGS = -c -g --std=c99 -pedantic -pedantic-errors -Wall -Wextra -Werror -Wno-unused-parameter -Wno-implicit-fallthrough -D_POSIX_C_SOURCE=200809L -I./include

# Opciones de enlazado
LINKER_FLAGS = -g --std=c99 -pedantic -pedantic-errors -Wall -Wextra -Werror -Wno-unused-parameter -Wno-implicit-fallthrough -D_POSIX_C_SOURCE=200809L -lm

COMPILER = $(CC)
LINKER = $(CC)
