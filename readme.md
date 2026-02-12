## Building the project
* Create a `build` directory at the top level of the project (where the top level `CMakeLists.txt` lives)
* `cd` into `build` and run `cmake .. && cmake --build .`
* This will create the `ConvertSingle` and `ConvertBulk` executables in the same directory

## Running the project
* Available command line options are:
    * `-profile=your_profile.icc` - absolute path
    * `-input_file=` - enter either a single filename or an absolute path
    * `-deviceToPcs=...` - available values are `1` or `0` depending on whether attribute is `true` or `false` respectively
    * `-render_intent=...` - available options are:
        * `0` - Perceptual
        * `1` - Relative
        * `2` - Saturation
        * `3` - Absolute
    * `-output_to=...` - enter either a single filename or an absolute path