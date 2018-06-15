
exports = Module;
var requireDepth = 0;

// Invoke with makeRequireFunction(module) where |module| is the Module object
// to use as the context for the require() function.
function makeRequireFunction(mod) {
    const Module = mod.constructor;
  
    function require(path) {
      try {
        requireDepth += 1;
        return mod.require(path);
      } finally {
        requireDepth -= 1;
      }
    }
  
    // Enable support to add extra extension types.
    require.extensions = Module._extensions;
  
    require.cache = Module._cache;
  
    return require;
}

function Module(id) {
  this.id = id;
  this.exports = {};
  this.filename = null;
  this.loaded = false;
}

Module._cache = Object.create(null);
Module._extensions = Object.create(null);

Module.wrap = function(script) {
  return Module.wrapper[0] + script + Module.wrapper[1];
};

Module.wrapper = [
  '(function (exports, require, module, __filename) { ',
  '\n});'
];
// Check the cache for the requested file.
// 1. If a module already exists in the cache: return its exports object.
// 2. If the module is native: call `NativeModule.require()` with the
//    filename and return the result.
// 3. Otherwise, create a new module for the file and save it to the cache.
//    Then have it load  the file contents before returning its exports
//    object.
Module._load = function(filename,parent,isMain) {
//   if (parent) {
//     // debug('Module._load REQUEST %s parent: %s', request, parent.id);
//   }

  var cachedModule = Module._cache[filename];
  if (cachedModule) {
//    updateChildren(cachedModule, true);
    return cachedModule.exports;
  }

  // Don't call updateChildren(), Module constructor already does.
  var module = new Module(filename);

  if (isMain) {
    // process.mainModule = module;
    module.id = '.';
  }

  Module._cache[filename] = module;

  tryModuleLoad(module, filename);

  return module.exports;
};

function tryModuleLoad(module, filename) {
  var threw = true;
  try {
    module.load(filename);
    threw = false;
  } finally {
    if (threw) {
      delete Module._cache[filename];
    }
  }
}

// Given a file name, pass it to the proper extension handler.
Module.prototype.load = function(filename) {

  this.filename = filename;
  var scriptSource = NativeDevice.loadScriptSource(filename);
  Module._extensions['.js'](this, filename,scriptSource);
  this.loaded = true;
   
};

// Loads a module at the given file path. Returns that module's
// `exports` property.
Module.prototype.require = function(id) {
  if (typeof id !== 'string') {
    // throw new ERR_INVALID_ARG_TYPE('id', 'string', id);
  }
  if (id === '') {
    // throw new ERR_INVALID_ARG_VALUE('id', id,
                                    // 'must be a non-empty string');
  }
  return Module._load(id, this, /* isMain */ false);
};

// Run the file contents in the correct scope or sandbox. Expose
// the correct helper variables (require, module, exports) to
// the file.
// Returns exception, if any.
Module.prototype._compile = function(filename,content) {

  // create wrapper function
  var wrapper = Module.wrap(content);

  var compiledWrapper = NativeDevice.compileScript(wrapper);

  var require = makeRequireFunction(this);

  var result = compiledWrapper(this.exports, require, this,filename);
 
  return result;
};

// Native extension for .js
Module._extensions['.js'] = function(module, filename,content) {
  module._compile(filename,content);
};

// bootstrap main module.
Module.runMain = function() {
  // Load the main module--the command line argument.
  Module._load(name,null,true);
};

runMain = function() {
    // Load the main module--the command line argument.
    Module._load("main",null,true);
};

