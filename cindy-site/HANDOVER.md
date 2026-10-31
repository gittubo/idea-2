# cindyzhu.com.au redesign: handover

A "clean scrapbook" restyle of the site: paper, washi tape, stickers and handwritten
notes, with one bold accent colour (ballpoint cobalt `#2E44E6`) replacing the beige
and terracotta. It's still plain HTML and CSS, with no framework and no build step.

## What's in here

```
site/                     ← everything Netlify publishes
  styles.css              ← the ONE stylesheet: design tokens, components, pages, guide template
  index.html              ← Home
  ai-system.html          ← AI System Club
  about.html              ← About (new page, see "Needs your OK")
  contact.html            ← Contact
  design-system.html      ← reference sheet of colours, type, spacing and components (noindex)
  guides/free-content-engine.html   ← your live guide, byte-for-byte unchanged, as the proof
  fonts/                  ← self-hosted fonts + their licences
  ribbon.js               ← the moving topic ribbon (your inline script, moved into a file)
  cinema.js               ← the cinematic layer (see below)
  mark.svg                ← the peeled-sticker brand mark
  site.js, guide-kit.js, site-reveals.js   ← your existing scripts, unchanged
  cindy*.jpg, favicon.svg ← your existing images
tools/check.mjs           ← pre-publish checks (see below)
tools/original/           ← snapshots of the live pages, used by the wording check
netlify.toml              ← tells Netlify to publish site/ with no build
```

## The cinematic layer

The brief said the old site felt boring, so the pages play like scenes:

- **The desk (opening shot).** A full-height hero with soft, drifting light leaks. Your photos are fanned on the desk at different depths, they shift with the mouse and as you scroll, and the headline rises in.
- **Pasted in, drawn on.** Cards, notes and stats stick down with a small overshoot as you scroll, and the scribbles and circles draw themselves.
- **The screening room.** The waitlist ticket glows under a spotlight on a dark band with torn paper edges.
- **The film reel.** On desktop, the six "what you could build" cards become a pinned filmstrip that scrolls sideways. On phones they stack as normal cards.
- **The receipt prints**, and **the final frame** is a full-bleed cobalt duotone of your beach photo with halftone dots.
- **Everywhere, guides included:** a fine film grain, a reading-progress line under the header, and the new peeled-sticker mark as the dot in "Cindy Zhu."

All of it lives in `cinema.js` plus section 13 of `styles.css`. It only switches on when the
visitor's device allows motion. With reduced motion turned on, or no JavaScript, every page is
complete and still, and nothing is hidden. Guides stay calm for reading: they get the grain,
progress line and mark, but no pinned or moving scenes.

## Brand kit

`brand/brandkit.png` is the one-page identity board (built from `brand/brandkit.html`):
the peeled-sticker mark and how it's built, colour, type, paper pieces, image direction
and system details. The mark itself is `site/mark.svg`. The idea behind it: your guides are
"all free, all copy-paste", and a sticker is the scrapbook version of paste. The corner
peels up as if it was just stuck down. Your current favicon is unchanged. Swapping it for
the mark is your call.

## How every guide picks up the new look

Your guides already load `../styles.css`, so the restyle is entirely in that file. The
CSS targets the class names the guides already use (`guidehead`, `gtag`, `gcallout`,
`minicard`, `tip`, `steps`, `promptwrap`, `wl-card`, and so on). **No guide HTML changes,
so no guide text changes.** `guides/free-content-engine.html` is a copy of the live page,
and the check below proves it's identical.

To roll it out: replace `styles.css` in your current repo with this one, and add the
`fonts/` folder next to it. That's the whole change for all 500+ guides.

## Push it live on Netlify yourself

**Option A: new repo (cleanest).**
1. Create a repo you own on GitHub and push this folder to it (`netlify.toml` at the top level).
2. In Netlify: Add new site → Import an existing project → pick the repo.
   Leave the build command empty. Netlify reads `publish = "site"` from `netlify.toml`.
3. Every push to `main` then redeploys, and every pull request gets its own preview URL.
   That's where you leave feedback each round.

**Option B: keep your current repo.** Copy the contents of `site/` over your current site
files (keep your other guides, `media-kit.html`, `privacy`, `terms` and so on), commit and push.
Your existing Netlify settings keep working.

Your Netlify Forms waitlist keeps working: the forms keep the same `name="waitlist"`,
`form-name` and `bot-field` fields, and `site.js` still submits them in the background.

## Check before you publish

```
npm i -D playwright && npx playwright install chromium
node tools/check.mjs
```

It loads every page at 320, 360, 390, 768, 1024, 1366 and 1920 px wide and fails on any
sideways scrolling. It confirms the sample guide's HTML is unchanged. It also confirms
that every piece of visible text on the new pages already exists on your current site,
so no wording was rewritten. The last run passed.

## Licences: all fine for commercial use

| Asset | Source | Licence |
|---|---|---|
| Bricolage Grotesque (headings) | Google Fonts, self-hosted | SIL Open Font License 1.1 (`fonts/OFL-bricolagegrotesque.txt`) |
| Figtree (body) | Google Fonts, self-hosted | SIL OFL 1.1 (`fonts/OFL-figtree.txt`) |
| Shantell Sans (handwritten notes) | Google Fonts, self-hosted | SIL OFL 1.1 (`fonts/OFL-shantellsans.txt`) |
| Tape, stickers, scribbles, arrows, paperclip, torn edge, icons | Drawn for this project in CSS/inline SVG | Yours |
| Photos, favicon | Your existing files | Yours |

There are no stock images, no icon packs and no paid fonts. Emoji come from each visitor's own device.

## Needs your OK (I haven't changed these without asking)

1. **"About" in the menu.** The live site has no About page (`/about` is a 404), so I
   added one plus an "About" link in the header and footer of the new pages. Every
   sentence on it is copied from your current home, Club and contact pages. Send me
   real About copy whenever you like.
2. **Guide headers.** The guides keep their own header markup, which has no "About" link.
   If you want it there too, it's a one-line find-and-replace across the guides. That
   changes menu markup, not guide text, so I'd only do it with your go-ahead.
3. **Old font link on guides.** Guides still load your old Google Fonts link (Averia,
   Poppins, Caveat, Patrick Hand). It does no harm, but removing it makes guides load
   faster. It's another one-line find-and-replace, again only if you agree.
4. **Not restyled yet:** the guides index (`/guides`) and the media kit. The media kit uses
   its own inline styles and `chrome.css`. Both are easy follow-ups using the same tokens.

## Changing the look later

Everything starts from the tokens at the top of `styles.css`. Change `--accent` and the
whole site recolours, including buttons, links, scribbles, numbers and every guide. The
`/design-system` page shows every token and component live.
