# Build

## Requirements

- cmake >= V3.21
- [libdatachannel dependencies](https://github.com/paullouisageneau/libdatachannel/blob/master/README.md#dependencies)

## Building from source

Prebuilt binaries are shipped inside the npm package under `prebuilds/` and are
loaded at runtime via [`pkg-prebuilds`](https://github.com/Julusian/pkg-prebuilds).
If no matching prebuild is available for your platform/architecture, the install
step automatically falls back to a source build with `cmake-js`.

To force a build from source (skipping the prebuild check):

```sh
> git clone https://github.com/murat-dogan/node-datachannel.git
> cd node-datachannel
> npm install --build-from-source
```

Or, after checkout:

```sh
> npm install --ignore-scripts
> npm run compile      # cmake-js build
> npm run build:tsc    # rollup (produces dist/)
```

Other Options

```sh
# Use GnuTLS instead of OpenSSL (Default False)
> npm run install-gnu

# Use libnice instead of libjuice (Default False)
# libnice-dev packet should be installed. (eg. sudo apt install libnice-dev)
> npm run install-nice
```

Compile without Media and Websocket

```sh
npx cmake-js clean
npx cmake-js configure --CDNO_MEDIA=ON --CDNO_WEBSOCKET=ON
npx cmake-js build
```

## Producing prebuilds

After building the native binary with `cmake-js` (which outputs
`build/Release/node_datachannel.node`), copy it into the `prebuilds/` folder
with the correct naming using `pkg-prebuilds-copy`:

```sh
npx pkg-prebuilds-copy \
  --baseDir build/Release \
  --source node_datachannel.node \
  --name=node_datachannel \
  --strip \
  --napi_version=8 \
  --arch=x64
```

For Linux musl builds, also pass `--libc=musl`. The resulting `prebuilds/`
folder is what gets published to npm and is loaded at runtime by
`pkg-prebuilds`.
