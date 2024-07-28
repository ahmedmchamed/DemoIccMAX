## Building the project
* Create a `build` directory at the top level of the project (where the top level `CMakeLists.txt` lives)
* `cd` into `build` and run `cmake .. && cmake --build .`
* This will create the `EntryPoint` executable in the same directory

## Running the project
* Ensure that you have all of your required files in the same directory that you run `EntryPoint` from
* Available command line options are:
    * `-profile=your_profile.icc`
    * `-csv=your_csv_file.csv`
    * `-deviceToPcs=...` - available values are `1` or `0` depending on whether attribute is `true` or `false` respectively
    * `-render_intent=...` - available options are:
        * `0` - Perceptual
        * `1` - Relative
        * `2` - Saturation
        * `3` - Absolute