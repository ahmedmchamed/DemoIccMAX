## Building the project
* Create a `build` directory at the top level of the project (where the top level `CMakeLists.txt` lives)
* `cd` into `build` and run `cmake .. && cmake --build .`
* This will create the `EntryPoint` executable in the same directory

## Running the project
* Ensure that you have all of your required files in the same directory that you run `EntryPoint` from
* Available command line options are:
    * `-profile=`