cd luajit
make
cd ..
g++ bbsfmlmain.cpp bbutil.cpp gfxmanager.cpp filemanager.cpp circle2d.cpp \
line2d.cpp point2d.cpp sensorhandler.cpp zone.cpp bbengine.cpp bblua.cpp \
gfxeventhandler.cpp rectangle.cpp stage.cpp cliprinthandler.cpp \
clipackagereporter.cpp dockitem.cpp dockshape.cpp docktext.cpp dockfader.cpp \
zipper.cpp guizipper.cpp \
./luajit/src/libluajit.a ./libarchive/libarchive.a \
-I./luajit/src -I./libarchive/include -I./stlsoft-1.9.116/include \
-I./sfml/include -L./sfml-lib -lsfml-graphics -lsfml-window -lsfml-system -ldl \
-o bbmain
cp ./scripts/bb_linux.sh ./berrybots.sh
cp ./icon.iconset/icon_128x128.png .
chmod 755 ./berrybots.sh
