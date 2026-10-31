# cindyzhu.com.au redesign: handover

**Direction: The Creative AI Notebook.** Clean editorial pages on notebook paper, decorated
(never crowded) with small stickers, washi tape and handwritten notes. There's one bold
accent, electric violet, with lime and bubblegum for the stickers. It's still plain HTML and
CSS on Netlify, with no framework and no build step.

## What's in here

```
site/                     ← everything Netlify publishes
  styles.css              ← the ONE stylesheet: tokens, components, pages, guide template
  index.html              ← Home
  ai-system.html          ← AI System Club
  about.html              ← About (new page, see "Needs your OK")
  contact.html            ← Contact
  design-system.html      ← live reference of colours, type, spacing and components (noindex)
  guides/free-content-engine.html   ← your live guide, byte-for-byte unchanged, as the proof
  img/                    ← new images (see "Images and licences")
  fonts/                  ← self-hosted Manrope, DM Sans, Caveat + their licences
  cinema.js               ← gentle motion: pasted-in cards, self-drawing underlines, light parallax
  ribbon.js               ← the moving topic ribbon (your inline script, moved into a file)
  site.js, guide-kit.js, site-reveals.js   ← your existing scripts, unchanged
  cindy*.jpg, favicon.svg ← your existing images
tools/check.mjs           ← pre-publish checks (see below)
tools/original/           ← snapshots of the live pages, used by the wording check
netlify.toml              ← tells Netlify to publish site/ with no build
```

**Why `styles.css` stays one file at the root, not `css/design-system.css` + `main.css` + `guide.css`:**
every one of your 500+ guides already links `../styles.css`. Splitting it would mean editing
every guide. Instead, the design-system tokens sit at the top of `styles.css` as CSS variables
(`--color-accent`, `--font-heading`, `--space-3`, `--radius-card` …), followed by components,
pages, and the guide template. URLs stay as they are (`/ai-system`, not `/ai-system-club`).

## Design system

| Token | Value | Use |
|---|---|---|
| `--color-paper` | `#F9F7F0` Notebook Paper | page background |
| `--color-ink` | `#242424` Ink | text and headings |
| `--color-accent` | `#6655E8` Electric Violet | buttons, links, club card, active nav |
| `--color-accent-deep` | `#4B3BCB` | violet text on tinted surfaces, guide headings |
| `--color-lime` | `#D8F36A` Lime Sticker | stickers, tags, highlights (never text) |
| `--color-pink` | `#FFB9D1` Bubblegum | tape, secondary details (never text) |
| `--font-heading` | Manrope | headings and interface |
| `--font-body` | DM Sans | body copy |
| `--font-handwritten` | Caveat | notes and labels only, never paragraphs |

Contrast checked against WCAG AA (4.5:1): violet on paper 4.9:1, white on violet 5.2:1,
deep violet on paper 7.0:1, ink on lime 12.5:1. The `/design-system` page shows every
token and component live.

## How every guide picks up the new look

The CSS targets the class names your guides already use (`guidehead`, `gtag`, `gcallout`,
`minicard`, `tip`, `steps`, `promptwrap`, `wl-card` …). **No guide HTML changes, so no guide
text changes.** Each guide becomes a white paper page taped to the notebook, with violet
section headings, lime topic labels, the "Hey, it's Cindy" intro as a lime sticky note, and
prompts in paper cards with a Copy button. To roll it out, replace `styles.css` in your current
repo and add the `fonts/` folder. That's the whole change for every guide.

## Your words, kept

The reference mockup used placeholder copy ("Join 500,000+ learners", "monthly workshops",
"Dream bigger…"). None of that is on your site, and two of those are factual claims, so
I didn't use any of it. Every word on the new pages is yours, moved around to fit the new
layout. For example, the taped proof card uses your real numbers from the contact page, and
the guide-card tags are the topic labels from the guides themselves. `tools/check.mjs` fails if any visible
text isn't on your current site. The only new labels are "Home" and "About" in the menu.

## Images and licences: all fine for commercial use

| Asset | Source | Licence |
|---|---|---|
| Manrope, DM Sans, Caveat | Google Fonts, self-hosted | SIL Open Font License 1.1 (`fonts/OFL-*.txt`) |
| `img/cindy-cutout.webp`, `img/cindy-contact-cutout.webp` | Your own photos, background removed with Higgsfield | Your photos |
| `img/robot-sticker.webp`, `img/thumb-*.webp` | Generated for this project with Higgsfield (GPT Image), no logos, no people | Commercial use under the Higgsfield plan used to create them. Worth confirming against your plan's terms |
| Tape, stickers, doodles, hearts, crown, torn edges, icons | Drawn for this project in CSS / inline SVG | Yours |

There are no stock photos and no third-party brand logos. The mockup's ChatGPT, Canva and
Notion marks were swapped for original illustrations, because those logos are other
companies' trademarks.

## Push it live on Netlify yourself

**Option A: new repo.** Push this folder to a GitHub repo you own. In Netlify, choose Add new site
→ Import an existing project → pick the repo. Leave the build command empty;
`netlify.toml` publishes `site/`. Each pull request then gets its own preview URL for feedback rounds.

**Option B: your current repo.** Copy the contents of `site/` over your current files (keep your
other guides, media kit, privacy and terms), commit and push.

Your Netlify Forms waitlist keeps working: same `name="waitlist"`, `form-name` and `bot-field`.

## Check before you publish

```
npm i -D playwright && npx playwright install chromium
node tools/check.mjs
```

The check loads every page at 320–1920 px and fails on sideways scrolling. It confirms the
sample guide's HTML is unchanged, and that every visible word already exists on your site. The last run passed.

## Needs your OK

1. **"About" page and menu link.** The live `/about` is a 404. The new page uses only sentences from your other pages.
2. **Guide menus.** Guides keep their own header markup (no Home/About links). Updating them
   is a one-line find-and-replace across guides. It changes the menu only, not guide text.
3. **Old font link on guides.** Guides still load your old Google Fonts link. It's harmless, but
   removing it makes guides faster. It's another one-line find-and-replace.
4. **Not restyled yet:** the guides index and media kit (it uses its own inline styles + `chrome.css`).
   Both can inherit these tokens next.
