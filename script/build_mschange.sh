PATH=$PSPSDK/bin:$PATH

if ![[ -d ../build ]]; then
  mkdir -p ../build
fi

pushd ../src
make
mv -f mschange.prx ../build/
make clean
popd