# Scout Graphical Renderer
My own barebones graphical renderer.

# TODO
- Finish bespoke opengl implementation.
- Separate graphical renderer interface into 3D and 2D interface renderers. Currently, sdl implementation breaks Liskov's substitution principle because it doesn't implement 3D related functionalities (because it can't).
