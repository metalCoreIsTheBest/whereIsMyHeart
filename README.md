# whereIsMyHeart

A simple toy-level heart animation generator

---

> *To Miss Nie* 😘

## Prerequisite
1. a C++ compiler (`g++/clang++/...`)
2. xmake
3. ffmpeg
4. {fmt} library

## How to use
1. clone this repo: `git clone https://github.com/metalCoreIsTheBest/whereIsMyHeart.git`
1. `cd whereIsMyHeart`
1. `xmake f -m release`
1. run `xmake` to compile the program
1. write a config file (see `animation.txt` for example)
1. create a directory for the output (for example, `mkdir output`)
1. run the program (for example, `xmake run whereIsMyHeart $(realpath ./animation.txt) $(realpath ./output/)`)
1. check the `output.mp4` in your output directory
