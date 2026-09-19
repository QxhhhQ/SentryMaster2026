if test -d ../build; then
    echo "已删除build文件"
    rm -r ../build
fi

if test -d ../install; then
    echo "已删除install文件"
    rm -r ../install
fi

if test -d ../log; then
    echo "已删除log文件"
    rm -r ../log
fi

cd ..
colcon build --symlink-install --cmake-args -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Release --parallel-workers 1