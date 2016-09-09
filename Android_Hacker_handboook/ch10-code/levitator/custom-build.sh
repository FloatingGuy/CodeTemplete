NDK="$HOME/android/dev/ndk"
PLAT="$NDK/platforms/android-8/arch-arm"
CC="$NDK/toolchains/arm-linux-androideabi-4.7/prebuilt/linux-x86/bin/arm-linux-androideabi-gcc"
CFLAGS="-I$PLAT/usr/include/"
LDFLAGS="-Wl,-rpath-link=$PLAT/usr/lib -L$PLAT/usr/lib -nostdlib $PLAT/usr/lib/crtbegin_dynamic.o -lc"
$CC $CFLAGS $* $CFLAGS $LDFLAGS

