import loadBinding = require('pkg-prebuilds');
import bindingOptions = require('../../binding-options');

// `process.env.PACKAGE_ROOT` is injected at build time by the rollup
// `replace` plugin (see rollup.config.mjs) so that the path is correct both
// when running from `src/lib/` and from the bundled `dist/<format>/lib/`.
const packageRoot = process.env.PACKAGE_ROOT || __dirname + '/../../';

// Load the native binding from the prebuilds folder, falling back to a
// locally built binary in build/Release or build/Debug.
const nodeDataChannel = loadBinding(packageRoot, bindingOptions);
export default nodeDataChannel;
