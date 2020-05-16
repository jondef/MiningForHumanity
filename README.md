# DonationMining

# Cloning
Make sure git submodules are up to date:
`git submodule update --init --recursive`

Add the following cmake options to the project:
`-DCUDA_ENABLE=OFF -DOpenCL_ENABLE=OFF`

# Building for Windows
## Get Dependencies
See: https://stackoverflow.com/a/48467983/10450514
`yay -Sy mingw-w64-qt5-base mingw-w64-boost` and always select bootstrap when possible
For 64 bit, execute `x86_64-w64-mingw32-cmake`
For 32 bit, execute `i686-w64-mingw32-cmake`
Finally, type `make`. It will always build with static linking for target Windows.

# Building libbcrypt for target windows
You need to refactor /include/bcrypt/bcrypt.h to bcryptt.h for example, because the mingw-w64 headers
already have a file called bcrypt.h. ncrypt.h in mingw-w64 #include <bcrypt.h> -> this includes the one
in /include/bcrypt/bcrypt.h and not the one in the mingw-w64 headers thus giving errors.

Building for 32 bits: you'll get the following error: conflicting types for ‘ssize_t’. To solve this, you need to replace `typedef __int32 ssize_t;` with the following code block in the bcryptt.c src file, whose header was reformated in the previous step:<br/>
```
#if defined(_WIN32) && !defined(_WIN64)
typedef __int32 ssize_t;
#elif defined(_WIN32) && defined(_WIN64)
typedef __int64 ssize_t;
#endif
```

# Building xmr-stak for target windows
If host is window: make sure you use the posix version of mingw!

Linux/Win hosts: for some reason, cpu/crypto/common/VirtualMemory_unix.cpp get included even if building for target windows.
Solution: find out why it gets included instead of VirtualMemory_win.cpp. Hack is to rename the unix version to unixx and make a copy of the win.cpp with the unix's name.

Next problem: in file home_dir.hpp <Winsock2.h> and <Shlobj.h> are not found. Refactor these header files by removing the capital letters.

Next: <byteswap.h> not found in file jext.hpp. That's because _MSC_VER is not defined for some reason. Solution: add a #define _MSC_VER above the #ifdef _MSC_VER line.

Next problem: getenv_s function was not declared in file console.cpp. That's because it's only a MSVC function, not available on mingw. Solution: replace the win_exit(int code) function with the following code:
```
void win_exit(int code)
 {
 	//size_t envSize = 0;
 	//getenv_s(&envSize, nullptr, 0, "XMRSTAK_NOWAIT");
 	char* t = std::getenv("XMRSTAK_NOWAIT");
 	if(t) {
 		printer::inst()->print_str("Press any key to exit.");
 		get_key();
 	}
 	std::exit(code);
 }
```
 
Next problem: error: invalid conversion from 'std::thread::native_handle_type' {aka ‘long long unsigned int’} to ‘HANDLE’ {aka ‘void*’} [-fpermissive]
Solution: replace this: ´return SetThreadAffinityMask(h, 1ULL << cpu_id) != 0;´ with ´return SetThreadAffinityMask(reinterpret_cast<HANDLE>(h), 1ULL << cpu_id) != 0;´

Done for compilation problems :)

One more linking error: unrecognized option '-z'. Where does it come from?
Solution: go into the CMake caches, and find a link.txt and remove `-z,noexecstack`
This argument comes from the xmr-stak cmake lists file
 
 
 
