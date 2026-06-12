import TrpJson from './trpjson.js';
import { readFileSync } from 'fs';

const ITERATIONS = 1000;

// ── helpers ──────────────────────────────────────────────────────────────────

function parseWasm(mod, jsonStr) {
  const len = mod.lengthBytesUTF8(jsonStr) + 1;
  const ptr = mod._malloc(len);
  mod.stringToUTF8(jsonStr, ptr, len);
  const root = mod._parse_json_to_ast(ptr);
  mod._free(ptr);
  return root;
}

function bench(label, fn, iterations) {
  // warmup
  for (let i = 0; i < 10; i++) fn();

  const times = [];
  for (let i = 0; i < iterations; i++) {
    const t0 = performance.now();
    fn();
    times.push(performance.now() - t0);
  }

  const total = times.reduce((a, b) => a + b, 0);
  const avg   = total / times.length;
  const min   = Math.min(...times);
  const max   = Math.max(...times);
  const p95   = times.sort((a, b) => a - b)[Math.floor(times.length * 0.95)];

  return { label, total, avg, min, max, p95, iterations };
}

function fmt(n) { return n.toFixed(4).padStart(10); }

function printResult(r) {
  console.log(`\n  ${r.label}`);
  console.log(`    iterations : ${r.iterations}`);
  console.log(`    total      : ${fmt(r.total)} ms`);
  console.log(`    avg        : ${fmt(r.avg)} ms`);
  console.log(`    min        : ${fmt(r.min)} ms`);
  console.log(`    max        : ${fmt(r.max)} ms`);
  console.log(`    p95        : ${fmt(r.p95)} ms`);
}

function printComparison(native, wasm) {
  const ratio = wasm.avg / native.avg;
  const faster = ratio < 1 ? `TrpJSON is ${(1/ratio).toFixed(2)}x FASTER` 
                            : `JSON.parse is ${ratio.toFixed(2)}x FASTER`;
  console.log(`\n  → ${faster} (avg)`);
  console.log(`  → throughput ratio: ${ratio.toFixed(3)}`);
}

// ── test cases ────────────────────────────────────────────────────────────────

const cases = {
  tiny: JSON.stringify({ x: 1 }),

  small: JSON.stringify({
    name: "TrpJSON", version: 1, active: true,
    tags: ["fast", "tiny", "wasm"],
    meta: { author: "sel-mlil", year: 2024 }
  }),

  medium: JSON.stringify(
    Array.from({ length: 100 }, (_, i) => ({
      id: i, name: `item_${i}`, value: Math.random(),
      enabled: i % 2 === 0, tags: [`tag_${i}`, `tag_${i+1}`]
    }))
  ),

  large: JSON.stringify(
    Array.from({ length: 1000 }, (_, i) => ({
      id: i,
      name: `user_${i}`,
      email: `user${i}@example.com`,
      score: Math.random() * 100,
      active: i % 3 !== 0,
      address: { city: `City${i}`, zip: `${10000 + i}` },
      tags: Array.from({ length: 5 }, (_, j) => `tag_${i}_${j}`)
    }))
  ),
};

// ── run ───────────────────────────────────────────────────────────────────────

const mod = await TrpJson();

console.log('═'.repeat(60));
console.log('  TrpJSON (WASM) vs JSON.parse — Benchmark');
console.log('═'.repeat(60));

for (const [name, json] of Object.entries(cases)) {
  const bytes = Buffer.byteLength(json, 'utf8');
  console.log(`\n${'─'.repeat(60)}`);
  console.log(`  case: ${name.padEnd(8)} │ ${bytes.toLocaleString()} bytes │ ${json.length.toLocaleString()} chars`);
  console.log('─'.repeat(60));

  const native = bench('JSON.parse  (native)', () => {
    JSON.parse(json);
  }, ITERATIONS);

  const wasm = bench('TrpJSON     (wasm)', () => {
    const root = parseWasm(mod, json);
    mod._free_ast(root);
  }, ITERATIONS);

  printResult(native);
  printResult(wasm);
  printComparison(native, wasm);
}

console.log(`\n${'═'.repeat(60)}`);
console.log('  Done.');
console.log('═'.repeat(60));
