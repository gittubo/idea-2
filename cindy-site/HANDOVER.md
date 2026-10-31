# cindyzhu.com.au redesign: handover

**Direction: The Creative AI Notebook.** Clean editorial pages on notebook paper, decorated
(never crowded) with small stickers, washi tape and handwritten notes. There's one bold
accent, electric violet, with lime and bubblegum for the stickers. It's still plain HTML and
CSS on Netlify, with no framework and no build step.

## What's in here

```
site/                     ← everything Netlify publishes
  styles.css              ← shared stylesheet: tokens at the top, components, guide template
  css/                    ← one small file per page, each linked only from that page
    club.css  guides.css  about.css  contact.css  mediakit.css
  index.html              ← Home (/)
  ai-system.html          ← AI System Club
  guides.html             ← Free guides: all 551 guides, with topic and tool filters
  about.html              ← About (new page, see "Needs your OK")
  contact.html            ← Contact, with a Netlify form (name="contact")
  media-kit.html          ← your full media kit, restyled
  design-system.html      ← live reference of colours, type, spacing and components (noindex)
  guides/free-content-engine.html   ← your live guide, byte-for-byte unchanged, as the proof
  img/                    ← new images (see "Images and licences")
  partners/               ← your own reel thumbnails from your live site
  fonts/                  ← self-hosted Manrope, DM Sans, Caveat + their licences
  cinema.js               ← gentle motion: pasted-in cards, self-drawing underlines, light parallax
  ribbon.js               ← the moving topic ribbon (your inline script, moved into a file)
  site.js, guide-kit.js, site-reveals.js   ← your existing scripts
  cindy*.jpg, favicon.svg ← your existing images
tools/check.mjs           ← pre-publish checks (see below)
tools/original/           ← snapshots of the live pages, used by the wording check
previews/                 ← screenshots of the pages, for a quick look
netlify.toml              ← tells Netlify to publish site/ with no build
```

**Why `styles.css` stays at the root:** every one of your guides already links `../styles.css`,
and guides only ever need that one file. Splitting it would mean editing every guide. The
design-system tokens sit at the top of `styles.css` as CSS variables (`--color-accent`,
`--font-heading`, `--space-3`, `--radius-card` …), followed by components and the guide template.
Styles that belong to a single page live in `site/css/` and are linked only from that page.
URLs stay as they are (`/ai-system`, not `/ai-system-club`).

**The shared look** (all in `styles.css`, so guides get it too): a crumpled paper texture
(`img/paper.webp`), a lighter paper header with a torn edge, a painted brush blob behind the
active menu item (`img/brush.webp`), a handwritten outlined button with a white arrow in a violet
circle, and a violet watercolour band (`img/band.webp`) rising into the violet footer
(`--color-footer`, `#5547E4`). The menu has a Guides dropdown and a search icon that opens the
guide library (your live site has no search).

**What is on each page:**

- **Home (`/`)**: hero with your beach photo, the proof card with your numbers, the topic
  ribbon, the AI System Club card, the closing line.
- **AI System Club (`/ai-system`)**: your club copy and waitlist form (`css/club.css`).
- **Free guides (`/guides`)**: the full library of 551 guides, rebuilt from your live page, with
  topic and tool filters (`css/guides.css`).
- **About (`/about`)**: new page, only sentences from your other pages (`css/about.css`).
- **Contact (`/contact`)**: your contact details and a message form (`css/contact.css`).
- **Media kit (`/media-kit`)**: your full live media kit, restyled (`css/mediakit.css`).
- **Guide** (`/guides/free-content-engine`): the one sample guide, unchanged.

## Design system

| Token | Value | Use |
|---|---|---|
| `--color-paper` | `#F7F1E9` Notebook Paper | page background (under the paper texture) |
| `--color-ink` | `#242424` Ink | text and headings |
| `--color-accent` | `#6655E8` Electric Violet | buttons, links, club card |
| `--color-accent-deep` | `#4B3BCB` | violet text on tinted surfaces, guide headings |
| `--color-lime` | `#D8F36A` Lime Sticker | stickers, tags, highlights (never text) |
| `--color-pink` | `#FFB9D1` Bubblegum | tape, secondary details (never text) |
| `--color-footer` | `#5547E4` Footer Violet | footer and the band above it |
| `--font-heading` | Manrope | headings and interface |
| `--font-body` | DM Sans | body copy |
| `--font-handwritten` | Caveat | notes and labels only, never paragraphs |

Contrast checked against WCAG AA (4.5:1): violet on paper 4.6:1, white on violet 5.2:1,
deep violet on paper 6.7:1, ink on lime 12.5:1, white on footer violet 6.2:1. The `/design-system` page shows every
token and component live.

## How every guide picks up the new look

The CSS targets the class names your guides already use (`guidehead`, `gtag`, `gcallout`,
`minicard`, `tip`, `steps`, `promptwrap`, `wl-card` …). **No guide HTML changes, so no guide
text changes.** Each guide becomes a white paper page taped to the notebook, with violet
section headings, lime topic labels, the "Hey, it's Cindy" intro as a lime sticky note, and
prompts in paper cards with a Copy button. To roll it out, replace `styles.css` in your current
repo and add the `fonts/` and `img/` folders (the paper, brush and band images load from there).
That's the whole change for every guide.

## Your words, kept

The reference mockup used placeholder copy ("Join 500,000+ learners", "monthly workshops",
"Dream bigger…"). None of that is on your site, and two of those are factual claims, so
I didn't use any of it. Every word on the new pages is yours, moved around to fit the new
layout. For example, the taped proof card uses your real numbers from the contact page, and
the guide-card tags are the topic labels from the guides themselves. `tools/check.mjs` fails if any visible
text isn't on your current site. The only new wording is "Home" and "About" in the menu, and the five labels on the contact form ("Send a message", "Your name", "Your email", "Your message", "Send message").

## Images and licences: all fine for commercial use

| Asset | Source | Licence |
|---|---|---|
| Manrope, DM Sans, Caveat | Google Fonts, self-hosted | SIL Open Font License 1.1 (`fonts/OFL-*.txt`) |
| `img/hero-bg.webp`, `img/hero-cutout.webp` | Your beach photo. Higgsfield extended (outpainted) the beach to the sides so it fills the wide hero, then removed the background for the die-cut outline. You are untouched, but **the extra beach around you is AI-generated** | Your photo + AI extension |
| `img/cindy-contact-cutout.webp` | Your own photo, background removed with Higgsfield | Your photo |
| `img/robot-sticker.webp`, `img/club-laptop.webp`, `img/paper.webp`, `img/band.webp`, `img/brush.webp`, `img/thumb-*.webp` (10 topics: start, marketing, career, content, design, productivity, building, life, money, power) | Generated for this project with Higgsfield (GPT Image), no logos, no people | Commercial use under the Higgsfield plan used to create them. Worth confirming against your plan's terms |
| `partners/*.jpg` | Your own reel thumbnails, taken from your live site | Yours |
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
The contact page adds a second form, `name="contact"`, which Netlify detects on the first deploy.

For the preview, the repo is `gittubo/idea-2` with base directory `cindy-site`. `netlify.toml`
sits in that folder and publishes `site/`, with no build.

## Check before you publish

```
npm i -D playwright && npx playwright install chromium
node tools/check.mjs
```

The check covers 7 paths (`/`, `/ai-system`, `/about`, `/contact`, `/guides`, `/media-kit` and
the sample guide) at 7 widths (320, 360, 390, 768, 1024, 1366 and 1920 px) and fails on sideways
scrolling. It confirms the sample guide's HTML is unchanged, and that every visible word on the
pages already exists on your live site. The only allowed new text is "Home", "About" and the five
contact labels. It does not cover `/design-system`.

## Needs your OK

1. **The five contact form labels.** "Send a message", "Your name", "Your email", "Your message"
   and "Send message" are the only new wording on the site.
2. **"About" page and menu link.** The live `/about` is a 404. The new page uses only sentences from your other pages.
3. **Guide menus.** Guides keep their own header markup (no Home/About links). Adding them
   is a one-line find-and-replace across guides. It changes the menu only, not guide text.
4. **Old font link on guides.** Guides still load your old Google Fonts link. It's harmless, but
   removing it makes guides faster. It's another one-line find-and-replace.
5. **Button label.** The header button keeps your "Join the waitlist" label. The mockup says
   "Join the Club". Say if you want it changed.
