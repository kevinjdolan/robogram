To initialize for building, do:

    mkdir build
    cd build
    cmake -DCMAKE_TOOLCHAIN_FILE=../avr-gcc-toolchain.cmake ../

Then subsequently, to build and upload:

    cd build
    make upload_robogram
