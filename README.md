# Pixapper
 A little program that takes in pixel art and outputs map and source files (in a png format).

# Notes
- The map file must have at least as many pixels of each color as the source: that is, if the source file has ten white pixels, the map file must at least have ten as well, otherwise the program will throw an error.
- Similarly, Pixapper flips the png so that, rather than providing coordinates from the top left, the output is mapped with more traditional bottom-left origin coordinates. I intend to add an option to change this in the future.