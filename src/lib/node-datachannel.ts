import loadBinding = require('pkg-prebuilds');
import bindingOptions = require('../../binding-options');

// `PACKAGE_ROOT` is injected at build time by the rollup `replace` plugin
// (see rollup.config.mjs) so that the path is correct both when running from
// `src/lib/` and from the bundled `dist/<format>/lib/`.
declare const PACKAGE_ROOT: string;

// Load the native binding from the prebuilds folder, falling back to a
// locally built binary in build/Release or build/Debug.
const nodeDataChannel = loadBinding(PACKAGE_ROOT, bindingOptions);
export default nodeDataChannel;
