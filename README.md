# DonationMining

# Cloning
Make sure git submodules are up to date:
`git submodule update --init --recursive`

Add the following cmake options to the project:
`-DCUDA_ENABLE=OFF -DOpenCL_ENABLE=OFF`

# Building for Windows
## Get Dependencies
`yay -Sy mingw-w64-qt5-base mingw-w64-boost` and always select bootstrap when possible
For 64 bit, execute `x86_64-w64-mingw32-cmake`
For 32 bit, execute `i686-w64-mingw32-cmake`
Finally, type `make`. It will always build with static linking for target Windows.