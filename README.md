# guiterm

A GUI-based terminal emulator, designed to be versatile in every way. Unlike other terminal emulators, guiterm can incorperate widgets such as buttons, text boxes, and more.

## Building

### Dependencies

I might be missing some dependecies in this list lol.

Fedora/RedHat:
```bash
sudo dnf install meson gcc SDL3-devel SDL3_ttf-devel jsoncpp-devel
```

### Bulding the damn thing

```bash
git clone https://github.com/RQuarx/guiterm
cd guiterm
meson build
cd build
ninja
```

After that the binary should be in the `build/` folder.