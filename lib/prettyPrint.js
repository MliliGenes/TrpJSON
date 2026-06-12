// ANSI color codes matching the C++ macros (adjust if yours differ)
const C = {
  brace:  '\x1b[33m',  // yellow
  key:    '\x1b[36m',  // cyan
  string: '\x1b[32m',  // green
  number: '\x1b[94m',  // bright blue
  bool:   '\x1b[35m',  // magenta
  null:   '\x1b[90m',  // gray
  punct:  '\x1b[37m',  // white
  reset:  '\x1b[0m',
};

const TYPE = { NULL: 0, BOOL: 1, NUMBER: 2, STRING: 3, ARRAY: 4, OBJECT: 5 };

function astValueToString(mod, ptr, indent = 0) {
  const tab = (n) => '\t'.repeat(n);

  if (!ptr) return `${C.null}null${C.reset}`;

  const type = mod._get_node_type(ptr);

  if (type === TYPE.STRING) {
    const val = mod.cwrap('get_value_string', 'string', ['number'])(ptr);
    return `${C.string}"${val}"${C.reset}`;
  }

  if (type === TYPE.NUMBER) {
    const val = mod._get_value_number(ptr);
    return `${C.number}${val}${C.reset}`;
  }

  if (type === TYPE.BOOL) {
    const val = mod._get_value_bool(ptr);
    return `${C.bool}${val ? 'true' : 'false'}${C.reset}`;
  }

  if (type === TYPE.NULL) {
    return `${C.null}null${C.reset}`;
  }

  if (type === TYPE.ARRAY) {
    const size = mod._get_array_size(ptr);
    const nextTab = tab(indent + 1);
    const lines = [];
    for (let i = 0; i < size; i++) {
      const child = mod._get_array_value_at(ptr, i);
      const comma = i < size - 1 ? `${C.punct},${C.reset}` : '';
      lines.push(`${nextTab}${astValueToString(mod, child, indent + 1)}${comma}`);
    }
    return `${C.brace}[${C.reset}\n${lines.join('\n')}\n${tab(indent)}${C.brace}]${C.reset}`;
  }

  if (type === TYPE.OBJECT) {
    const size = mod._get_object_size(ptr);
    const nextTab = tab(indent + 1);
    const getString = mod.cwrap('get_object_key_at', 'string', ['number', 'number']);
    const lines = [];
    for (let i = 0; i < size; i++) {
      const key = getString(ptr, i);
      const val = mod._get_object_value_at(ptr, i);
      const comma = i < size - 1 ? `${C.punct},${C.reset}` : '';
      lines.push(
        `${nextTab}${C.key}"${key}"${C.reset}${C.punct}: ${C.reset}${astValueToString(mod, val, indent + 1)}${comma}`
      );
    }
    return `${C.brace}{${C.reset}\n${lines.join('\n')}\n${tab(indent)}${C.brace}}${C.reset}`;
  }

  return `${C.string}"UNKNOWN_TYPE"${C.reset}`;
}

export function prettyPrint(mod, ptr) {
  if (!ptr) { return; }
  console.log(astValueToString(mod, ptr));
}