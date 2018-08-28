# About Scop
This is a 42 School project, the purpose is to write a complete .obj files viewr in C with opengl 4.
Textures and normal maps are supported

# Compile it
```
git clone --recursive https://github.com/Chr0nos/scop
cd scop
make
```

# Usage
./scop file.obj [texture] <normal_map>

# Run it
```
./scop obj/2b-nier.obj ./textures/2b_suit_dif.png ./textures/2bnor.png
```

# Keybinding
| Key              | purpose                        |
|:-----------------|:-------------------------------|
| left arrow       | move the object                |
| right arrow      | move the object                |
| up arrow         | move the object                |
| down arrow       | move the object                |
| w,asdeq          | rotate the object              |
| space            | stop/start automatic rotation  |
| l                | toggle lighting support        |
| k                | toggle normal_map              |
| mouse wheel      | zoom in/out                    |
| -                | zoom out                       |
| +                | zoom out                       |
| click + drag     | rotate the object              |
| z                | toggle faces viewing mode      |
| x                | incrase face view mode         |
| c                | decrase face view mode         |
     

# Rotation
All rotations are performed by quaternions

# Libraries
This project use GLFW 3 and SOIL2 libraries, other libs (libft, draw, libtga) are custom made by me
