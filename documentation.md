# say hello to the Next Generation (tm) leviathan docs

leviathan uses the lev:: namespace (lev:: was annoying to type)

this framework isnt really built for rendering 3D, but it can with enough work (similar to how gamemaker CAN, its just not exactly built with that in mind)

cba to make a proper documentation so this will have to do.  
ok so heres how it works:

### assets
similar to the monogame ContentManager, handles loading assets and storing them

- *AssetMgr* -> loads and stores assets efficiently
- *AssetLoader* -> if inherited allows you to register it in an assetmgr and let it manage the object, effectively extending the functionality of a vanilla AssetMgr

### audio
guess what? never happening (:sunglasses:)

### containers
no me gusta el std:: (i have a gcse in spanish)

- *Vector* -> dynamic array
- *String* -> string class for storing the Jason Vale Bible (Jible) (warning: will require 23 terabytes of memory and a fragment of your soul)
- *HashMap* -> map container using hashes for quick access
- *Array* -> c-style array wrapper
- *Pair* -> pair of two objects, for example a 3D spinning rat with "Free Bird" playing in the background and cheese
- *Optional* -> an optional object
- *Bitset* -> container to efficiently store bit flags by maximising the usage of a single char to store 8 bits
- *LinkedList* -> list of linked lists of linked lists of linked lists of linked lists of linked lists of linked lists of linked lists of linked lists of linked lists of linked lists of linked lists of linked lists of linked lists of linked lists of linked lists of linked lists of linked lists of linked lists of linked lists

### core
Core clas's'es'. They configure thing's s'uch as the window and actually perform the main loop of the application

- *App* -> responsible for actually starting the window
- *Config* -> pass into app to configure the window and properties about the app itself
- *Util* -> utility header
  - *useful #defines (assertions, byte conversions, etc...)*
  - *different types (s32, f64, etc...)*
  - *log::* -> log things like whether the 3D spinning rat is taking up too much memory
  - *mem::* -> wrapper around memory functions
  - *cstr::* -> wrapper around cstring (char*) functions
  - *time::* -> t i m e

### graphics
used for calculating physics obviously

- *SpriteBatch* -> renders to the screen by push-ing and pop-ing different settings. useful for rendering a spinning rat
- *Compare* -> compare your mom
- *Blend* -> blendmode settings for rendering with the spritebatch
- *Font* -> load fonts + render them
- *GfxUtil* -> utility functions for exporting math wrappers into just vertices
- *RenderPass* -> if you want to skip straigt to rendering the raw vertex data of the funny 3D spinning rat
- *Mesh* -> vertices + indices
- *Texture* -> texture for drawing
- *ArrayTexture* -> texture with layers for drawing, useful in stuff like tilemaps
- *TextureSampler* -> configures how exactly to sample a texture
- *Shader* -> you want to make sure the spinning rat has anatomically correct shading dont you?
- *ShaderBuffer* -> send a buffer of data into the shader
- *RenderTarget* -> render to a texture instead of directly to the screen
- *Material* -> shader + array of textures and samplers
- *Image* -> holds the raw byte data of the spinning rat texture

### gui
for drawing gui

- *GUIComponent* -> base class for GUIComponents
    - *GUIText* -> so you can render to the user how many times the 3D spinning rat has spun (wow!)
- *GUIConstraints* -> class containing gui constraints for x,y,w,h
  - *PixelConstraint* -> constrain component to be at a specific pixel position
  - *MousePosConstraint* -> constrain component to be at mouse

### input
input gathering

- *Key* -> key scancodes
- *Mouse* -> mouse scancodes
- *JoystickButton* -> joystick scancodes
- *Input* -> check for input so you can vary how fast the spinning rat is spinning

### io
i/o wrappers

- *Endian* -> functions for checking the endianness of a system
- *Stream* -> base class for writing to memory
  - *FileStream* -> open a file and store user data such as how many times you can make a 3D rat spin
  - *MemoryStream* -> read write to direct memory
  - *ConstMemoryStream* -> havent found a use for it yet, but incase you just wanna read memory without it changing i guess

### math
custom maths classes, obviously dont need to clarify, theyre for doing anything but playing jazz

- *Calc* -> functions for general calculations such as abs(), sin(), etc... also contains constants such as calc::RAD2DEG and calc::3D_SPINNING_RAT_VELOCITY
- *Circle* -> wrapper defining a point and a radius
- *Colour* -> generic 4byte colour class
- *Ease* -> functions for easing over a time *t*, such as a bounce
- *Line* -> wrapper defining two points in space
- *Mat3x2* -> general matrix representing a 2D transformation (actual transform + position). mathematically this isnt a 3x2 its a 2x2 (transformation) + 1x2 (position)
- *Mat4x4* -> general matrix used for things like projections
- *Polygon* -> wrapper for a set of 2D points
- *Quad* -> wrapper for 4 points in space
- *Quaternion* -> NOT FINISHED. represents a 4D hypersphere which is projected onto a 3D plane so you can render a spinning 3D rat while playing "Free Bird" without jittering
- *Rand* -> wrapper for a set of randomness functions
- *Rect<T>* -> circol
- *Range* -> wrapper over the wrapper of rand. define a min and max and sample random values in-between.
- *Timer* -> timer for timing things and making sure the 3D spinning rat from before is spinning at physically correct speed
- *Triangle* -> wrapper over 3 points in space
- *Vec2<T>* -> templated 2D vector representing a 2D point in space
- *Vec3<T>* -> templated 3D vector representing a 3D point in space
