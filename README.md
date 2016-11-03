## About

This is a photobooth to put your OSX webcam on a Harry Potter style wanted poster for Sirius Black.

## Running

Install opencv with homebrew:

    $ brew install opencv2

Then run:

    $ cmake .
    $ make

And run the program with

    $ ./main

If you want GIFs, so that the GIF is recorded while holding down the space bar, set the `gif` value in `main.cpp` to true. If you want just pictures, set the `gif` value in `main.cpp` to false.
