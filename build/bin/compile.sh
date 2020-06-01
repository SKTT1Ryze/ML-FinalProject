#script for compile seetaFace2 example
cd ..
cmake .. -G"Unix Makefiles" -DCMAKE_INSTALL_PREFIX=`pwd`/install -DCMAKE_BUILD_TYPE=Release -DBUILD_EXAMPLE=ON
cmake --build . --config Release
cmake --build . --config Release --target install/strip
cd bin/

