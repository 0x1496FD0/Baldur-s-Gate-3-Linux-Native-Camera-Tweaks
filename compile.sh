rm -Rf build
mkdir build
cd build
cmake ..
make
mv liblinux_native_camera_tweaks.so linux_native_camera_tweaks.so
