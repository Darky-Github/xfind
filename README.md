# xfind - Advanced File Search Tool

`xfind` is a powerful replacement to `find`, offering better searching with matching, text searching within files, and multi-threaded performance.

## Features

- **Fuzzy Search**: Finds files even with slight misspellings.
- **Case-Insensitive Matching**: Use `-i` to ignore case.
- **Text Search**: Use `-text` to find files containing specific words.
- **Multi-threaded Performance**: Faster searches with parallel processing.
- **Color-coded Output**: Distinguishes file types.
- **Exclude Directories**: Skips specific directories for efficiency.
- **Export Search Results**: Save results to a file.

## Installation

```sh
git clone https://github.com/Darky-Github/xfind.git
cd xfind
make
sudo make install
```

## Usage

### Basic file search
```sh
xfind filename
```

### Case-insensitive search
```sh
xfind -i filename
```

### Search inside file contents
```sh
xfind -text "keyword"
```

### Help menu
```sh
xfind -h
```

## Uninstallation
```sh
sudo make uninstall
```

## License
MIT License

---
Built for efficient and advanced file searching!
