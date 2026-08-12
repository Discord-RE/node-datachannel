import dts from 'rollup-plugin-dts';
import esbuild from 'rollup-plugin-esbuild';
import esmShim from '@rollup/plugin-esm-shim';
import replace from '@rollup/plugin-replace';

const external = (id) => {
  return !/^[./]/.test(id);
};

const bundle = (config) => ({
  ...config,
  input: 'src/index.ts',
  external: ['events','stream'] // <-- suppresses the warning for these built-in modules
});

export default [
  bundle({
    plugins: [
      replace({
        include: 'src/lib/node-datachannel.ts',
        preventAssignment: true,
        // `binding-options.js` is a CommonJS module that rollup inlines, so the
        // `require('../../binding-options')` call does not need rewriting.
        // `PACKAGE_ROOT` is the basePath passed to `pkg-prebuilds`' loadBinding.
        // From `src/lib/` the package root is two levels up, but the bundled
        // output lives in `dist/<format>/lib/` which is three levels up.
        PACKAGE_ROOT: "__dirname + '/../../../'",
      }),
      esmShim(),
      esbuild(),
    ],
    output: [
      {
        dir: 'dist/esm',
        format: 'es',
        exports: 'named',
        sourcemap: true,
        entryFileNames: '[name].mjs',
        preserveModules: true, // Keep directory structure and files
      },
      {
        dir: 'dist/cjs',
        format: 'cjs',
        exports: 'named',
        sourcemap: true,
        entryFileNames: '[name].cjs',
        preserveModules: true, // Keep directory structure and files
      },
    ],
  }),
  // types
  {
    plugins: [dts()],
    input: 'src/lib/index.ts',
    external,
    output: {
      dir: 'dist/types/lib',
      format: 'cjs',
      exports: 'named',
      preserveModules: true, // Keep directory structure and files
    },
  },
  {
    plugins: [dts()],
    input: 'src/polyfill/index.ts',
    external: (id) => {
      if (id.startsWith('../lib')) return true;
      return !/^[./]/.test(id);
    },
    output: {
      dir: 'dist/types/polyfill',
      format: 'cjs',
      exports: 'named',
      preserveModules: true, // Keep directory structure and files
    },
  },
];
