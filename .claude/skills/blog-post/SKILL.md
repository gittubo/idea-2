---
name: blog-post
description: Write a blog post with image, internal links and signup form, and save it to WordPress as Pending. Use when a calendar row with Channel=Blog needs a draft.
---
# Blog post

1. Read `CLAUDE.md`, `brand/voice.md`, `brand/product-facts.md`. Set the row to `Drafting`.
2. Write 700–1,100 words in HTML: one H1-worthy title, H2 sections, short paragraphs.
   Include 2–4 internal links to existing posts or product pages and this signup block at the end:
   `<!-- wp:shortcode -->[kit_form id="REPLACE_ME"]<!-- /wp:shortcode -->`
3. Save the body to a temp file, then run:
   - `scripts/wp-upload-image.sh brand/photos/<photo>.jpg "<alt text>"` → media id
   - `scripts/wp-draft.sh "<title>" <body.html> <media id>`
   The script always sets `status=pending`. Never change that.
4. Update the row: `Draft link` = the printed link, `Status = Needs approval`.
