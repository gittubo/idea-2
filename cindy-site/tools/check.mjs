// Pre-publish checks for the redesign. Run from the repo root:
//   node tools/check.mjs
// Needs Node 18+ and Playwright (npm i -D playwright && npx playwright install chromium).
//
// 1. No page scrolls sideways at common phone, tablet and desktop widths.
// 2. The sample guide's HTML is byte-identical to the live one (the restyle is CSS only).
// 3. Wording: every piece of visible text on the new pages already exists on the
//    current site (snapshots in tools/original/), so no copy was rewritten.
import { createServer } from 'node:http';
import { readFileSync, existsSync, statSync } from 'node:fs';
import { createHash } from 'node:crypto';
import { join, extname, dirname } from 'node:path';
import { fileURLToPath } from 'node:url';
import { createRequire } from 'node:module';

const { chromium } = createRequire(import.meta.url)('playwright');

const here = dirname(fileURLToPath(import.meta.url));
const root = join(here, '..', 'site');
const originals = join(here, 'original');

const pages = ['/', '/ai-system', '/about', '/contact', '/guides', '/media-kit', '/guides/free-content-engine'];
const widths = [320, 360, 390, 768, 1024, 1366, 1920];
// Labels that are new on purpose, listed so they stay visible in review.
const allowedNewText = ['About', 'Home', 'Send a message', 'Your name', 'Your email', 'Your message', 'Send message'];

const types = { '.html': 'text/html', '.css': 'text/css', '.js': 'text/javascript', '.jpg': 'image/jpeg', '.svg': 'image/svg+xml', '.woff2': 'font/woff2' };
const server = createServer((req, res) => {
  if (req.method === 'POST') { res.end('ok'); return; }
  const p = decodeURIComponent(req.url.split('?')[0]);
  for (const c of [p, p + '.html', join(p, 'index.html')]) {
    const f = join(root, c);
    if (f.startsWith(root) && existsSync(f) && statSync(f).isFile()) {
      res.writeHead(200, { 'Content-Type': types[extname(f)] || 'application/octet-stream' });
      res.end(readFileSync(f));
      return;
    }
  }
  res.writeHead(404); res.end();
}).listen(0);
const base = `http://localhost:${server.address().port}`;

const failures = [];
const norm = s => s.replace(/[’‘]/g, "'").replace(/\s+/g, ' ').trim();

const browser = await chromium.launch();

// 1. horizontal scroll
for (const path of pages) {
  for (const width of widths) {
    const page = await browser.newPage({ viewport: { width, height: 900 } });
    await page.goto(base + path, { waitUntil: 'networkidle' });
    const sw = await page.evaluate(() => document.documentElement.scrollWidth);
    if (sw > width) failures.push(`${path} scrolls sideways at ${width}px (scrollWidth ${sw})`);
    await page.close();
  }
}

// 2. guide untouched
const sha = f => createHash('sha256').update(readFileSync(f)).digest('hex');
if (sha(join(root, 'guides/free-content-engine.html')) !== sha(join(originals, 'guide-free-content-engine.html')))
  failures.push('guides/free-content-engine.html differs from the live guide');

// 3. wording
const page = await browser.newPage();
let corpus = '';
for (const f of ['index.html', 'ai-system.html', 'contact.html', 'media-kit.html', 'guides.html', 'guide-free-content-engine.html', 'guide-resume-unrejectable.html', 'guide-claude-full-time-designer.html']) {
  await page.setContent(readFileSync(join(originals, f), 'utf8'), { waitUntil: 'domcontentloaded' });
  corpus += ' ' + norm(await page.evaluate(() => document.body.textContent));
}
for (const path of pages.filter(p => !p.startsWith('/guides/'))) {
  await page.goto(base + path, { waitUntil: 'domcontentloaded' });
  const chunks = await page.evaluate(() => {
    const out = [];
    const walker = document.createTreeWalker(document.body, NodeFilter.SHOW_TEXT);
    while (walker.nextNode()) {
      const n = walker.currentNode;
      if (n.parentElement.closest('script,style')) continue;
      if (n.textContent.trim()) out.push(n.textContent);
    }
    return out;
  });
  for (const chunk of chunks.map(norm)) {
    if (!corpus.includes(chunk) && !allowedNewText.includes(chunk))
      failures.push(`${path}: text not on the current site: "${chunk}"`);
  }
}

await browser.close();
server.close();

if (failures.length) {
  console.log(`FAIL (${failures.length})\n- ` + failures.join('\n- '));
  process.exit(1);
}
console.log(`PASS: ${pages.length} pages x ${widths.length} widths, no sideways scroll; guide HTML unchanged; all wording matches the current site.`);
