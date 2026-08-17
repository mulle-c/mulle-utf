# Fuzzing mulle-utf

This directory contains fuzzing targets for the `mulle-utf` library using `libFuzzer`.

## Prerequisites

- `clang` with `libFuzzer` support (usually included in modern clang/LLVM).
- `mulle-sde` (to provide dependency paths and the environment).

## Usage

Use the `mulle-utf-fuzz` script to build and run fuzzers.

### Build all targets

```bash
./fuzz/mulle-utf-fuzz build
```

### Run a target

```bash
./fuzz/mulle-utf-fuzz run -t 60 fuzz-utf8-information
```

If no target is specified, all targets are run sequentially for the default time (60s).

### Replay a crash

If a crash is found, the input file will be saved in `fuzz/crashes/<target>/`. You can replay it with:

```bash
./fuzz/mulle-utf-fuzz replay <target> <input_file>
```

### Minimize corpus

```bash
./fuzz/mulle-utf-fuzz minimize <target>
```

## Targets

- `fuzz-utf8-information`: Validates `mulle_utf8_information` and cross-checks lengths.
- `fuzz-utf8-to-utf16`: Round-trip UTF-8 -> UTF-16 -> UTF-8 identity check.
- `fuzz-utf8-to-utf32`: Round-trip UTF-8 -> UTF-32 -> UTF-8 identity check.
- `fuzz-utf16-utf32-roundtrip`: Round-trip UTF-16 <-> UTF-32 identity check.
- `fuzz-scan`: Consistency checks for numeric scanning across all UTF encodings.
- `fuzz-char-extra`: Extended char5/char7 tests with UTF-16 and UTF-32 source encodings.
- `fuzz-rover`: Validates the `mulle_utf_rover` iterator API for all UTF types.
- `fuzz-single-char`: Fuzzes single character stepping functions (next/previous) for all UTF types.
- `fuzz-utf16-information`: UTF-16 analysis.
- `fuzz-utf32-information`: UTF-32 analysis.
- `fuzz-char5-char7`: Round-trip encoding/decoding for char5 and char7 schemes.

## Corpus

Seeds can be added to `fuzz/corpus/<target>/`. The fuzzer will automatically use them.
You can use the existing `unicode/` directory to generate some initial seeds.
