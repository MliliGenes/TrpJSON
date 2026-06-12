// test.mjs
import { prettyPrint } from './prettyPrint.js';
import TrpJson from './trpjson.js';

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

const mod = await TrpJson();

// wrap the API
const parse    = mod.cwrap('parse_json_to_ast',   'number', ['string']);
const freeAst  = mod.cwrap('free_ast',             null,     ['number']);
const getType  = mod.cwrap('get_node_type',        'number', ['number']);
const objSize  = mod.cwrap('get_object_size',      'number', ['number']);
const objKey   = mod.cwrap('get_object_key_at',    'string', ['number', 'number']);
const objVal   = mod.cwrap('get_object_value_at',  'number', ['number', 'number']);
const arrSize  = mod.cwrap('get_array_size',       'number', ['number']);
const arrVal   = mod.cwrap('get_array_value_at',   'number', ['number', 'number']);
const getStr   = mod.cwrap('get_value_string',     'string', ['number']);
const getNum   = mod.cwrap('get_value_number',     'number', ['number']);
const getBool  = mod.cwrap('get_value_bool',       'number', ['number']);

// type enum from your TrpJsonType.hpp — adjust if values differ
const TYPE = { NULL: 0, BOOL: 1, NUMBER: 2, STRING: 3, ARRAY: 4, OBJECT: 5 };


// --- tests ---
const cases = [
  '[1, "two", false, null, {"nested": 42}]',
  '"just a string"',
  '42.5',
  'true',
  'invalid json {{{'  // should return NULL
];

function parseJson(mod, jsonStr) {
  const len = mod.lengthBytesUTF8(jsonStr) + 1;
  const ptr = mod._malloc(len);
  mod.stringToUTF8(jsonStr, ptr, len);
  const result = mod._parse_json_to_ast(ptr);
  mod._free(ptr);
  return result;
}

const parseFeed = async () => {
  const res = await fetch('https://api.rss2json.com/v1/api.json?rss_url=http%3A%2F%2Ffeeds.twit.tv%2Fbrickhouse.xml');
  const text = await res.text();

  const root = parseJson(mod, text);
  prettyPrint(mod, root);
  freeAst(root);
}
parseFeed();

// for (const json of cases) {

//   console.log(json);
  
//   const root = parseJson(mod, json);
//   prettyPrint(mod, root);
//   freeAst(root);

//   console.log('\n');
// }
